//
// Created by Zhang Yulei on 8/25/21.
//

#ifndef PCTSIMULATION_FORMATTER_H
#define PCTSIMULATION_FORMATTER_H

#include "Analyzer/PCTAnalyzer.h"

#include <iostream>
#include <fstream>

class Formatter : public PCTAnalyzer {
public:
    Formatter();

    // Override virtual function of base class
    void Begin() override;

    void ProcessEvt(PCTEvent *event) override;

    void CheckEvt(PCTEvent *event) override;

    void End() override;

private:
    std::string out_file_name;
    int input_Z{};

    std::ofstream out_file;
};


#endif //PCTSIMULATION_FORMATTER_H
