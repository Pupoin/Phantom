//
// Created by Zhang Yulei on 8/11/21.
//

#ifndef PCTSIMULATION_PANALYZER_H
#define PCTSIMULATION_PANALYZER_H

#include "DataIO/PEvent.h"

#include <iostream>
#include <string>

using std::cout, std::endl;
using std::string;

// Abstract class of analyzers
class PAnalyzer {
public:
    PAnalyzer() = default;

    virtual ~PAnalyzer() = default;

    /*
     * Workflow of an algo:
     *      begin of run -> [loop of evt: process -> check  ] -> end of run
     */

    virtual void Begin() = 0;

    virtual void ProcessEvt(PEvent *) = 0;

    virtual void CheckEvt(PEvent *) = 0;

    virtual void End() = 0;

private:
    string name;
    string description;

    int Verbose{0};

};


#endif //PCTSIMULATION_PANALYZER_H
