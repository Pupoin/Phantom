#include "Core/EventReader.h"

#include <iostream>

using std::cout, std::endl;

// Required by Singleton
EventReader *pEvtReader = nullptr;

// Get Instance Class
EventReader *EventReader::CreateInstance() {
    if (pEvtReader == nullptr)
        pEvtReader = new EventReader();

    return pEvtReader;
}

EventReader::EventReader() = default;

void EventReader::setInput(const TString &fileName, const TString &treeName, const TString &geomName) {
    file_name = fileName;
    tree_name = treeName;
    geom_name = geomName == "" ? file_name : geomName;

    // Read data from data_file
    f = new TFile(TString(fileName));
    if (!f->IsOpen()) {
        std::cerr << "[READFILE ERROR] ==> File: " + fileName + " does not exist." << std::endl;
        exit(EXIT_FAILURE);
    }

    tree_reader = new TTreeReader(tree_name, f);
    evt = new TTreeReaderValue<PEvent *>(*tree_reader, "EventData");

    auto Entries = tree_reader->GetEntries();
    if (Verbose > -1) {
        cout << "======================================================================" << endl;
        std::cout << "[ READ FILE ] : (Verbosity 0)" << std::endl;
        std::cout << std::left;
        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Input File: " << std::setw(30) << file_name << std::endl;
        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Input Tree: " << std::setw(30) << tree_name << std::endl;

        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Total Event(s): " << std::setw(30) << Entries << std::endl;

        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Skip Event(s): " << std::setw(30) << skip_number << std::endl;

        int Evt = 0;
        if (event_number == -1)
            Evt = (Entries >= skip_number) ? static_cast<int>(Entries) - skip_number : 0;
        else if (Entries >= skip_number)
            Evt = (Entries >= skip_number + event_number) ? event_number : static_cast<int>(Entries) - skip_number;
        else
            Evt = 0;

        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Process Event(s): " << std::setw(30) << Evt << std::endl;
    }

    // Read Geometry
    g = new TFile(TString(geom_name));
    if (!g->IsOpen()) {
        std::cerr << "[READFILE ERROR] ==> File: " + geom_name + " does not exist." << std::endl;
        return;
    }

    std::cout << "[ READ Geometry ] ==> reading geometry from file: " << g->GetName() << std::endl;
    gGeoManager = dynamic_cast<TGeoManager *>(g->Get("DetGeoManager"));
    if (!gGeoManager) {
        std::cerr << "[ READ Geometry ] ==> No Geometry in the file..." << std::endl;
        return;
    }

}

PEvent *EventReader::getEntryNext() {
    if (tree_reader->IsInvalid()) return nullptr;

    while (event_processed_number < skip_number) {
        tree_reader->Next();
        current_event_number++;
    }

    if (!tree_reader->Next()) return nullptr;

    event_processed_number++;
    current_event_number++;

    return this->getEvt();
}