//
// Created by Avencast on 8/12/2021.
//

#include "Analyzer/ExampleAnalyzer.h"

#include "Core/GeometryHelper.h"

#include "TCanvas.h"
#include "TH1D.h"

ExampleAnalyzer::ExampleAnalyzer() {
    // IMPORTANT
    //

    // Basic setup
    // It's necessary if you want to correctly register your analyzer
    name = "ExampleAnalyzer";
    description = "An example of how to use analyzer";

    // Register Parameters, default value matters
    // RegisterParameter(variable_name, variable address, default value, description, variable_type)
    RegisterParameter("IntParameter", &parameter_1, 1, "An int var", "int");
    RegisterParameter("DoubleParameter", &parameter_2, 0.5, "An double var", "double");
    RegisterParameter("VecStrParameter", &parameter_3, {"e", "x"}, "vector of string", "vector<string>");

}

void ExampleAnalyzer::Begin() {
    hists.push_back(new TH1D("deltaE", "deltaE", 100, 0., 20));
    hists.push_back(new TH1D("E_diff", "E_diff", 100, 0., 20.));
}

void ExampleAnalyzer::ProcessEvt(PCTEvent *evt) {
    //cout << evt->getEventId() << endl;

    /* Example of how to use GeometryHelper Class to calculate geometry information */
    // Get The position of Scintillator with copy number of 2
    TVector3 position = pGeo->GetPosOfVolume("Scintillator", 2);

    // Get The length of Scintillator with copy number of 2
    TVector3 size = pGeo->GetSizeOfVolume("Telescope", 2);

    // Get The distance of one point to the surface
    TVector3 point = {110., 50., 2.1};
    double distance = pGeo->GetDisToSuf(point, "Scintillator", 1, surface::z_plus);

    // Find Truth Information
    auto proton_steps = evt->GetData("Init_Particle_Step", ParticleStep_DataType::COL);

    PCTStep *prev_step = nullptr;
    double E_diff = 0;
    double E_total = 0.;
    auto Contain = [](const std::string &str, const std::string &tar) {
        return str.find(tar) != std::string::npos;
    };
    for (auto step : *proton_steps) {
        std::string tar_name = "Target";
        if (prev_step) {
            if (step->getPVName() == "Target_Region") {
                E_total += step->getDeltaE();
            }
            // Record the momentum of proton while entering the target
            if (Contain(step->getPVName(), tar_name) && Contain(prev_step->getPVName(), "World")) {
//                cout << evt->getEventId() << ": " << step->getE() << Form(" (%6.3f) ", E_total);
                E_diff = step->getE();
            }
            if (Contain(step->getPVName(), "World") && Contain(prev_step->getPVName(), tar_name)) {
                E_diff -= prev_step->getE();
//                cout << " " << prev_step->getE() << Form(" (%6.3f) ", E_total);
            }
        }
        prev_step = step;
    }
//    cout << Form(" (%6.3f) \n", E_diff);
    if (E_total > 0) hists.at(0)->Fill(E_total);
    if (E_diff > 0) hists.at(1)->Fill(E_diff);
}

void ExampleAnalyzer::CheckEvt(PCTEvent *) {

}

void ExampleAnalyzer::End() {
    TCanvas c("c", "c", 1000, 800);
    hists.at(0)->Draw();
    c.SaveAs("hist0.png");
    hists.at(1)->Draw();
    c.SaveAs("hist1.png");
}
