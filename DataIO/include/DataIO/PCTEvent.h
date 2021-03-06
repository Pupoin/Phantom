//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef PSIM_DEVENT_H
#define PSIM_DEVENT_H

#include <cstddef>
#include <cstdlib>
#include <map>
#include <tuple>
#include <vector>
#include <iostream>
#include <memory>

#include "DataIO/PCTXData.h"
#include "DataIO/PCTStep.h"
#include "DataIO/MCParticle.h"
#include "DataIO/PCTHit.h"
#include "DataIO/PCTDigi.h"

#include "TObject.h"
#include "TString.h"

using std::vector;
using std::map;
using std::make_pair;
using std::tuple;
using std::make_tuple;

/* Type Define */
using mcp_map = map<TString, vector<MCParticle * >>;
using step_map = map<TString, vector<PCTStep * >>;
using data_map = map<TString, vector<PCTXData * >>;
using hits_map = map<TString, vector<PCTHit* >>;
using digi_map = map<TString, vector<PCTDigi* >>;

/* enum class */
enum CleanType {
    nALL, nVector
};

enum class Phantom_DataType {
    MCParticle, ParticleStep, DetectorData, DetectorHits, DetectorDigi
};

enum class MCParticle_DataType {
    COL
};
enum class ParticleStep_DataType {
    COL
};
enum class DetectorData_DataType {
    COL
};
enum class DetectorHits_DataType {
    COL
};
enum class DetectorDigi_DataType {
    COL
};

/// class description:
/// \brief Contains all the necessary information for analysis.
class PCTEvent : public TObject {
public:
    // Constructor
    PCTEvent();

    ~PCTEvent() override {
        Initialization(nALL);

        delete mcparticle_col;
        delete step_col;
        delete data_col;
    }

    // Operators

    // Get Methods
    int getRunId() const {
        return RunID;
    }

    int getEventId() const {
        return EventID;
    }

    int getVerbose() const {
        return Verbose;
    }

    [[nodiscard]] const double *getRndm() const {
        return Rndm;
    }

    // Set Methods
    void setRunId(int runId) {
        RunID = runId;
    }

    void setEventId(int eventId) {
        EventID = eventId;
    }

    void setVerbose(int verbose) {
        Verbose = verbose;
    }

    void setRndm(const double *rndm) {
        for (int i = 0; i < 4; ++i)
            Rndm[i] = rndm[i];
    }

    // Initialization
    void Initialization(CleanType ct);

    template<class T>
    void ClearMap(T *cmap, CleanType ct);

    // Register Collections
    bool RegisterCollection(const TString &col_name, Phantom_DataType dt);

    template<class T, class VT>
    bool RegisterColMap(const TString &col_name, T *cmap);

    // Delete  Collections
    bool DeleteCollection(const TString &col_name, Phantom_DataType dt);

    template<class T, class VT>
    bool DeleteColMap(const TString &col_name, T *cmap);

    // List all collections
    template<class T>
    void ListCollection(T *, const TString &pre_str = "");

    void ListAllCollections(const TString &str = "");

    // Fetch Collection
    vector<MCParticle *> *GetData(const TString &col_name, MCParticle_DataType);

    vector<PCTXData *> *GetData(const TString &col_name, DetectorData_DataType);

    vector<PCTStep *> *GetData(const TString &col_name, ParticleStep_DataType);

    vector<PCTHit *> *GetData(const TString &col_name, DetectorHits_DataType);

    vector<PCTDigi*> *GetData(const TString &col_name, DetectorDigi_DataType);
    template<class BT, class T>
    vector<BT *> *GetDataVec(const TString &col_name, T *cmap);

    /* Print Information */
    void PrintHeader(const TString &col_name, const TString &class_type);

    void PrintDetails();

    /* Search for certain MCParticle */
    static MCParticle *SearchID(vector<MCParticle *> *mv, int ID) {
        for (auto itr : *mv) {
            if (itr->getId() == ID) return itr;
        }
        return nullptr;
    }


#ifdef MEMCK

    static void PrintObjectStatistics(const TString &str = "");

#endif

protected:
    // run number
    int RunID{0};

    // event number
    int EventID{0};

    // Random Number Seeds
    double Rndm[4]{};

    // Verbosity
    int Verbose{0};

    // Data Collections
    mcp_map *mcparticle_col;
    data_map *data_col;
    step_map *step_col;
    hits_map *hits_col;
    digi_map *digi_col;

ClassDefOverride(PCTEvent, 1);
};

// Some inline
template<class T>
void PCTEvent::ListCollection(T *in, const TString &pre_str) {
    for (auto m_in : *in) {
        printf("%s%-23s %10lu\n", pre_str.Data(), m_in.first.Data(), m_in.second.size());
    }
}

template<class T>
void PCTEvent::ClearMap(T *cmap, CleanType ct) {
    for (auto &itr : *cmap) {
        for (auto &itr2 : itr.second) { delete itr2; }
        (itr.second).clear();
    }
    if (ct == nALL) cmap->clear();
}

template<class T, class VT>
bool PCTEvent::RegisterColMap(const TString &col_name, T *cmap) {
    // Sanity Check
    if (cmap->count(col_name) != 0) {
        std::cerr << "[WARNING] ==> Key already exists. Return the existing Key: " << col_name << std::endl;
        return false;
    }

    // Insert a map
    cmap->emplace(std::pair<TString, vector<VT *>>(col_name, vector<VT *>()));

    if (Verbose > 1) {
        std::cout << "[REGISTER] : (Verbosity 2) ==> A new collection " + col_name + " has been successfully added."
                  << std::endl;
    }

    return true;
}

template<class T, class VT>
bool PCTEvent::DeleteColMap(const TString &col_name, T *cmap) {
    auto itr = cmap->find(col_name);

    if (itr != cmap->end()) {
        cmap->erase(itr);
        std::cout << "[Delete Map] ==> Collection " + col_name + " has been successfully removed." << std::endl;

        return true;
    } else
        std::cerr << "[WARNING] ==> No Key named " + col_name + "." << std::endl;

    return false;
}

template<class BT, class T>
vector<BT *> *PCTEvent::GetDataVec(const TString &col_name, T *cmap) {
    auto itr = cmap->find(col_name);
    if (itr == cmap->end()) {
        std::cerr << "[WARNING] ==> No Key named " + col_name + "." << std::endl;
        exit(EXIT_FAILURE);
    }

    return &(cmap->at(col_name));
}


#endif //PSIM_DEVENT_H
