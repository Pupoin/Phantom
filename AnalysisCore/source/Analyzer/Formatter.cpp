//
// Created by Zhang Yulei on 8/25/21.
//

#include "Analyzer/Formatter.h"
#include "Core/GeometryHelper.h"

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
    //(x1,y1,z1,t1,dE1) (x2, y2,z3,t2,dE2) .... (xn,yn,zn,tn,dEn) (px py pz, Px,Py,Pz, depth, Z)
    double px = 0, py = 0, pz = 0, Px = 0, Py = 0, Pz = 0, depth = 0;
    int Z = input_Z;

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
                        det_strs.at(idx_z) = Form(targeted_str.c_str(), hit->getX(), hit->getY(),
                                                  hit->getZ(), hit->getE(), hit->getT());
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

        for (const auto &str: det_strs) out_file << str;
    }

    // Find Truth Information
    auto proton_steps = evt->GetData("Init_Particle_Step", ParticleStep_DataType::COL);
    PCTStep *prev_step = nullptr;
    TVector3 depth_in, depth_out;
    for (auto step : *proton_steps) {
        std::string tar_name = "Target";
        if (prev_step) {
            // Record the momentum of proton while entering the target
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
            }
        }
        prev_step = step;
    }
    // Calculate the incident depth
    depth = (depth_out - depth_in).z();

    out_file << Form("%6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6d", px, py, pz, Px, Py, Pz, depth, Z);
    out_file << "\n";
}

void Formatter::CheckEvt(PCTEvent *) {

}

void Formatter::End() {
    out_file.close();

    std::cout << Form("File: %s has been save to disk.", out_file_name.c_str()) << std::endl;
}