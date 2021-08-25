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
}

void Formatter::Begin() {
    out_file.open(out_file_name, std::ios::out | std::ios::trunc);
}

void Formatter::ProcessEvt(PCTEvent *evt) {
    //cout << evt->getEventId() << endl;
    out_file << Form("%d ", evt->getEventId());
    //(x1,y1,z1,t1,dE1) (x2, y2,z3,t2,dE2) .... (xn,yn,zn,tn,dEn) (px py pz, Px,Py,Pz, depth, Z)
    double px = 0, py = 0, pz = 0, Px = 0, Py = 0, Pz = 0, depth = 0;
    int Z = 0;

    // Find Formatted Detector Hit
    for (const auto &detector : pGeo->getDetInfo()) {
        auto det_name = detector.first;
        auto det_size = detector.second.size();

        // Exclude Target
        if (det_name == "Target") continue;

        // Loop each detector cell
        auto hit_col = evt->GetData(det_name, DetectorHit_DataType::COL);
        for (auto hit : *hit_col) {
            out_file << Form("%6.3f %6.3f %6.3f %6.3f %6.3f ", hit->getX(), hit->getY(), hit->getZ(), hit->getE(),
                             hit->getT());
        }
    }

    // Find Truth Information
    auto proton_steps = evt->GetData("Init_Particle_Step", ParticleStep_DataType::COL);
    PCTStep *prev_step = nullptr;
    for (auto step : *proton_steps) {
        // Record the momentum of proton while entering the target
        auto Contain = [](const std::string &str, const std::string &tar) {
            return str.empty() || str.find(tar) != std::string::npos;
        };
        std::string tar_name = "Target";
        if (prev_step) {
            if (Contain(step->getPVName(), tar_name) && Contain(prev_step->getPVName(), "World")) {
                px = prev_step->getPx();
                py = prev_step->getPy();
                pz = prev_step->getPz();
            }
            if (Contain(step->getPVName(), "World") && Contain(prev_step->getPVName(), tar_name)) {
                Px = prev_step->getPx();
                Py = prev_step->getPy();
                Pz = prev_step->getPz();
            }
        }
        prev_step = step;
    }

    out_file << Form("%6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f %6d", px, py, pz, Px, Py, Pz, depth, Z);
    out_file << "\n";
}

void Formatter::CheckEvt(PCTEvent *) {

}

void Formatter::End() {
    out_file.close();

    std::cout << Form("File: %s has been save to disk.", out_file_name.c_str()) << std::endl;
}