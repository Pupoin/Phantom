//
// Created by Zhang Yulei on 8/25/21.
//

#include "Analyzer/Formatter.h"
#include "Core/GeometryHelper.h"

#include <sstream>

Formatter::Formatter() {
    // Basic setup
    // It's necessary if you want to correctly register your analyzer
    name = "Formatter";
    description = "Format data to output txt file";

    // Register Parameters, default value matters
    // RegisterParameter(variable_name, variable address, default value, description, variable_type)
    RegisterParameter("output_file", &out_file_name, std::string("formatter.out"), "output file from formatter",
                      "std::string");
    RegisterParameter("input_Z", &input_Z, 1, "injected target Z number",
                      "int");
}

void Formatter::Begin() {
    out_file.open(out_file_name, std::ios::out | std::ios::trunc);
}

void Formatter::ProcessEvt(PCTEvent *evt) {
    //(x1,y1,z1,t1,dE1) (x2, y2,z3,t2,dE2) .... (xn,yn,zn,tn,dEn) m (x1,y1,z1,dE1) ... (xm,ym,zm,dEm) (px py pz, Px,Py,Pz, depth, Z)
    double px = 0, py = 0, pz = 0, Px = 0, Py = 0, Pz = 0, depth = 0;
    int Z = input_Z;
    out_string = "";

    auto Contain = [](const std::string &str, const std::string &tar) {
        return str.find(tar) != std::string::npos;
    };
    std::string targeted_str = "% 6.3f % 6.3f % 6.3f %6.3f %6.3f ";
    // Find Formatted Detector Hit
    for (const auto &detector : pGeo->getDetInfo()) {
        auto det_name = detector.first;
        auto det_size = detector.second.size();

        // Exclude Target
        if (det_name == "Target") continue;

        // Loop each detector cell
        auto hit_col = evt->GetData(det_name, DetectorHit_DataType::COL);

        std::vector<std::string> det_strs;
        if (Contain(det_name.Data(), "Tracker")) {
            det_strs = std::vector<std::string>(3, Form(targeted_str.c_str(), 0, 0, 0, 0, 0));
            for (auto hit : *hit_col) {
                // If any of the truth step hit includes the initial proton hit, record it
                if (std::any_of(hit->getTruth().begin(), hit->getTruth().end(),
                                [](PCTTruth t) { return t.trackID == 1; })) {
                    int idx_z = detector.second.at(hit->getCellId()).idx_z;
                    if (det_strs.at(idx_z) == Form(targeted_str.c_str(), 0, 0, 0, 0, 0)) {
                        // using center of each cell
//                        det_strs.at(idx_z) = Form(targeted_str.c_str(), hit->getX(), hit->getY(),
//                                                  hit->getZ(), hit->getE(), hit->getT());
                        // using truth information
                        det_strs.at(idx_z) = Form(targeted_str.c_str(), hit->getTruth().front().X,
                                                  hit->getTruth().front().Y, hit->getTruth().front().Z,
                                                  hit->getTruth().front().E, hit->getTruth().front().T);
                    }
                }
            }
        } else {
            det_strs = std::vector<std::string>(det_size, Form(targeted_str.c_str(), 0, 0, 0, 0, 0));
            for (auto hit : *hit_col) {
                det_strs.at(hit->getCellId() - 1) = Form(targeted_str.c_str(), hit->getX(), hit->getY(),
                                                         hit->getZ(), hit->getE(), hit->getT());
            }
        }

        for (const auto &str: det_strs) out_string.append(str);
    }

    // Find Truth Information
    int tar_count = 0;
    targeted_str = "% 6.3f % 6.3f % 6.3f %6.3f ";
    auto record_target_step = [&tar_count, targeted_str](PCTStep *step, PCTStep *prev_step) {
        tar_count++;
        return Form(targeted_str.c_str(), step->getX(), step->getY(), step->getZ(), prev_step->getE() - step->getE());
    };
    std::vector<std::string> step_strs;
    auto proton_steps = evt->GetData("Init_Particle_Step", ParticleStep_DataType::COL);
    PCTStep *prev_step = nullptr;
    TVector3 depth_in, depth_out;
    bool end_p = false;
    for (auto step : *proton_steps) {
        std::string tar_name = "Target";
        if (end_p) break;
        if (prev_step) {
            // Record the momentum of proton while entering the target
            if (Contain(step->getPVName(), tar_name))
                step_strs.emplace_back(record_target_step(step, prev_step));

            if (Contain(step->getPVName(), tar_name) && Contain(prev_step->getPVName(), "World")) {
                depth_in.SetXYZ(step->getX(), step->getY(), step->getZ());
                px = step->getPx();
                py = step->getPy();
                pz = step->getPz();
            }
            if (Contain(step->getPVName(), "World") && Contain(prev_step->getPVName(), tar_name)) {
                depth_out.SetXYZ(prev_step->getX(), prev_step->getY(), prev_step->getZ());
                Px = prev_step->getPx();
                Py = prev_step->getPy();
                Pz = prev_step->getPz();
                end_p = true;

                step_strs.emplace_back(record_target_step(step, prev_step));
            }
            if (Contain(step->getPVName(), "FrontTracker")) {
                depth_in.SetXYZ(0, 0, 0);
                px = step->getPx();
                py = step->getPy();
                pz = step->getPz();
            }
            if (!end_p && !Contain(step->getPVName(), "FrontTracker") &&
                !Contain(step->getPVName(), tar_name) &&
                !Contain(step->getPVName(), "World")) {
                depth_in.SetXYZ(0, 0, 0);
                Px = step->getPx();
                Py = step->getPy();
                Pz = step->getPz();

                end_p = true;
            }

        }
        prev_step = step;
    }
    // Calculate the incident depth
    depth = (depth_out - depth_in).z();

    out_string.append(std::to_string(tar_count) + " ");
    for (const auto &str: step_strs) out_string.append(str);
    out_string.append(Form("%6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6d\n", px, py, pz, Px, Py, Pz, depth, Z));

    out_file << out_string;
}

void Formatter::CheckEvt(PCTEvent *evt) {
//    // Check if the output string format is correct
//    int n_count = 0;
//
//    std::stringstream s_stream(out_string); //create string stream from the string
//    while (s_stream.good()) {
//        string substr;
//        getline(s_stream, substr, ' '); //get first string delimited by space
//        if (substr.empty()) continue;
//        substr.erase(remove(substr.begin(), substr.end(), ' '), substr.end());
//        n_count++;
//    }
//
//    const int target_n = 5 * (3 + 3 + 3 + 42) + 8;
//    if (n_count != target_n)
//        std::cerr << "Event " << evt->getEventId() << ": count number is wrong (" << n_count << ")" << endl;
}

void Formatter::End() {
    out_file.close();

    std::cout << Form("File: %s has been save to disk.", out_file_name.c_str()) << std::endl;
}