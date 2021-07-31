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

#include "DataIO/PHit.h"
#include "DataIO/PStep.h"
#include "DataIO/MCParticle.h"

#include "TObject.h"
#include "TString.h"

using std::vector;
using std::map;
using std::make_pair;
using std::tuple;
using std::make_tuple;

/* Type Define */
using mcp_map = map<TString, vector<MCParticle * >>;
using step_map = map<TString, vector<PStep * >>;
using hit_map = map<TString, vector<PHit * >>;

/* enum class */
enum CleanType {
    nALL, nVector
};

enum class Phantom_DataType {
    MCParticle, ParticleStep, DetectorHit
};

enum class MCParticle_DataType {
    COL
};
enum class ParticleStep_DataType {
    COL
};
enum class DetectorHit_DataType {
    COL
};

/// class description:
/// \brief Contains all the necessary information for analysis.
class PEvent : public TObject {
public:
    // Constructor
    PEvent();

    ~PEvent() override {
        Initialization(nALL);
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
    void ListCollection(T *);

    void ListAllCollections(const TString &str);

    // Fetch Collection
    vector<MCParticle *> *GetData(const TString &col_name, MCParticle_DataType);

    vector<PHit *> *GetData(const TString &col_name, DetectorHit_DataType);

    vector<PStep *> *GetData(const TString &col_name, ParticleStep_DataType);

    template<class BT, class T>
    vector<BT *> *GetDataVec(const TString &col_name, T *cmap);

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
    hit_map *hit_col;
    step_map *step_col;

ClassDefOverride(PEvent, 1);
};

// Some inline
template<class T>
void PEvent::ListCollection(T *in) {
    for (auto m_in : *in) {
        printf("%-20s %lu", m_in.first.Data(), m_in.second.size());
    }
}

template<class T>
void PEvent::ClearMap(T *cmap, CleanType ct) {
    for (auto &itr : *cmap) {
        for (auto &itr2 : itr.second) { delete itr2; }
        (itr.second).clear();
    }
    if (ct == nALL) cmap->clear();
}

template<class T, class VT>
bool PEvent::RegisterColMap(const TString &col_name, T *cmap) {
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
bool PEvent::DeleteColMap(const TString &col_name, T *cmap) {
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
vector<BT *> *PEvent::GetDataVec(const TString &col_name, T *cmap) {
    auto itr = cmap->find(col_name);
    if (itr == cmap->end()) {
        std::cerr << "[WARNING] ==> No Key named " + col_name + "." << std::endl;
        exit(EXIT_FAILURE);
    }

    return &(cmap->at(col_name));
}


#endif //PSIM_DEVENT_H
