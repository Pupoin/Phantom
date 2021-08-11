#ifndef PCTSIMULATION_EVENTREADER_H
#define PCTSIMULATION_EVENTREADER_H

#include "DataIO/PEvent.h"

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TGeoManager.h"

#include <iostream>
#include <iomanip>

// forward declaration
class ControlManager;

class EventReader {
public:
    static EventReader *CreateInstance();

    // Set Verbosity
    void setVerbose(int verbose) { Verbose = verbose; }

    // Getters
    [[nodiscard]] const TString &getFileName() const { return file_name; }

    [[nodiscard]] const TString &getGeomName() const { return geom_name; }

    [[nodiscard]] const TString &getTreeName() const { return tree_name; }

    [[nodiscard]] int getEventNumber() const { return event_number; }

    [[nodiscard]] int getSkipNumber() const { return skip_number; }

    [[nodiscard]] int getEventProcessedNumber() const { return event_processed_number; }

    [[nodiscard]] int getVerbose() const { return Verbose; }

    [[nodiscard]] int getCurrentEventNumber() const { return current_event_number; }

private:
    friend class ControlManager;

    // Call next entry, should call by Control Manager
    [[nodiscard]] PEvent *getEntryNext();

    // Should not call explicitly, which should be called by getEntryNext()
    [[nodiscard]] PEvent *getEvt() const { return *evt->Get(); }

    // Set input file
    void setInput(const TString &fileName, const TString &treeName, const TString &geomName = "");

private:
    EventReader();

    // Current Event info
    int event_number{-1};
    int skip_number{0};
    int event_processed_number{0};
    int current_event_number{0};

    // Root files
    TString file_name;
    TString geom_name;
    TString tree_name;

    TFile *f{};
    TFile *g{};
    TTreeReader *tree_reader{};
    TTreeReaderValue<PEvent *> *evt{};

    // Verbosity
    int Verbose{0};
public:
    // Delete Copy constructor
    EventReader(const EventReader &) = delete;

    EventReader &operator=(EventReader const &) = delete;
};

extern EventReader *pEvtReader;

#endif //PCTSIMULATION_EVENTREADER_H
