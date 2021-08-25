//
// Created by Zhang Yulei on 8/25/21.
//

#include "Analyzer/Formatter.h"


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
    out_file << Form("%d \n", evt->getEventId());
}

void Formatter::CheckEvt(PCTEvent *) {

}

void Formatter::End() {
    out_file.close();

    std::cout << Form("File: %s has been save to disk.", out_file_name.c_str()) << std::endl;
}