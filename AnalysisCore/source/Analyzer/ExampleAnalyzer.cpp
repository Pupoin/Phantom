//
// Created by Avencast on 8/12/2021.
//

#include "Analyzer/ExampleAnalyzer.h"

#include "Core/GeometryHelper.h"

ExampleAnalyzer::ExampleAnalyzer() {
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
    cout << distance << endl;
}

void ExampleAnalyzer::CheckEvt(PCTEvent *) {

}

void ExampleAnalyzer::End() {

}
