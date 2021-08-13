//
// Created by Avencast on 8/12/2021.
//

#include "Analyzer/ExampleAnalyzer.h"

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
}

void ExampleAnalyzer::CheckEvt(PCTEvent *) {

}

void ExampleAnalyzer::End() {

}
