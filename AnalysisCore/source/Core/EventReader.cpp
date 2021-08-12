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

    tree = f->Get<TTree>(treeName);
    tree->SetMakeClass(1);
    tree->SetBranchAddress("EventData", &evt);

    auto Entries = tree->GetEntries();
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

        if (event_number == -1)
            event_number = (Entries >= skip_number) ? static_cast<int>(Entries) - skip_number : 0;
        else if (Entries >= skip_number)
            event_number = (Entries >= skip_number + event_number) ? event_number : static_cast<int>(Entries) -
                                                                                    skip_number;
        else
            event_number = 0;

        std::cout << std::setw(5) << " " << std::setw(30);
        std::cout << "==> Process Event(s): " << std::setw(30) << event_number << std::endl;
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
    if (!tree) return nullptr;

    while (event_processed_number < skip_number) {
        current_event_number++;
    }

    if (event_processed_number >= event_number) return nullptr;

    tree->GetEntry(current_event_number);

    event_processed_number++;
    current_event_number++;

    return this->getEvt();
}