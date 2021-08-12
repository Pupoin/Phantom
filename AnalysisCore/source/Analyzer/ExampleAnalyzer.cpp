//
// Created by Avencast on 8/12/2021.
//

#include "Analyzer/ExampleAnalyzer.h"

ExampleAnalyzer::ExampleAnalyzer() {
    // Basic setup
    // It's necessary if you want to correctly register your analyzer
    name = "ExampleAnalyzer";
    description = "An example of how to use analyzer";

    // Register Parameters
    RegisterParameter("IntParameter", &parameter_1, 1, "An int var", "int");
    RegisterParameter("DoubleParameter", &parameter_2, 0.5, "An double var", "double");
    RegisterParameter("VecStrParameter", &parameter_3, {"e", "x"}, "vector of string", "vector<string>");

}

void ExampleAnalyzer::Begin() {
    // Test for parameters
    cout << "Parameter 1: " << parameter_1 << endl;
    cout << "Parameter 2: " << parameter_2 << endl;
    for (const auto &p: parameter_3) {
        cout << "-- " << p << endl;
    }

}

void ExampleAnalyzer::ProcessEvt(PEvent *evt) {
    cout << evt->getEventId() << endl;
}

void ExampleAnalyzer::CheckEvt(PEvent *) {

}

void ExampleAnalyzer::End() {

}
