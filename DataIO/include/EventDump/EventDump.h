//
// Created by Zhang Yulei on 7/31/21.
//

#ifndef PHANTOM_EventDump_H
#define PHANTOM_EventDump_H

#include <utility>
#include <iostream>
#include <iomanip>

#include "DataIO/PCTEvent.h"

#include "TFile.h"
#include "TTree.h"

using namespace std;

class EventDump {
public:
    EventDump(TString fName, TString tName, long long eventnumber);

    ~EventDump() = default;

    bool GetEntry(long long  i);

    void DumpEvent();

private:
    TString f_name;
    TString t_name;
    TFile *f{};
    TTree *t{};

    TBranch *b_evt{};
    PCTEvent *evt{};

    long long event_number{};
    long long total_entries{};

    int verbose{};
};

EventDump::EventDump(TString fName, TString tName, long long eventnumber)
        : f_name(std::move(fName)), t_name(std::move(tName)), event_number(eventnumber) {
    f = new TFile(TString(f_name));
    if (!f->IsOpen()) {
        std::cerr << "[READFILE ERROR] ==> File: " + f_name + " does not exist." << std::endl;
        exit(EXIT_FAILURE);
    }

    t = f->Get<TTree>(t_name.Data());
    if (!t) {
        std::cerr << "[Error] -- No tree " << t_name << " found" << std::endl;
        exit(EXIT_FAILURE);
        ;
    }
    total_entries = t->GetEntries();
    if (event_number >= total_entries) {
        cerr << " [Read Tree] ==> Specified entry " << event_number << " exceeds the total number of entries "
             << total_entries << endl;

        event_number = total_entries -1;
    }

    t->SetBranchAddress("EventData", &evt, &b_evt);

    if (verbose > -1) {
        cout << "======================================================================" << endl;
        std::cout << "[ READ FILE ] : (Verbosity 0)" << std::endl << left;
        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Input File: " << std::setw(30) << f_name << std::endl;
        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Input Tree: " << std::setw(30) << t_name << std::endl;

        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Total Event(s): " << std::setw(30) << total_entries << std::endl;

        cout << "----------------------------------------------------------------------" << endl;
        if (event_number < 0)
            cout << " ==> Print the information of all events." << endl;
        else
            std::cout << "==> Print detailed information for event: " << event_number << std::endl;
        cout << "======================================================================" << endl;
    }
}


bool EventDump::GetEntry(long long i) {
    if (!t) return false;
    Long64_t ientry = t->LoadTree(i);
    if (ientry < 0) return false;
    t->GetEntry(i);
    return true;
}

void EventDump::DumpEvent() {
    if (event_number < 0) {
        for (long long i = 0; i < total_entries; ++i) {
            GetEntry(i);
            evt->ListAllCollections(Form("Event: %lld", i));
            cout<<endl;
        }
    }
    else {
        GetEntry(event_number);
        evt->ListAllCollections(Form("Event: %lld", event_number));

        /* Print detailed information */
        evt->PrintDetails();
    }
}

#endif //PHANTOM_EventDump_H
