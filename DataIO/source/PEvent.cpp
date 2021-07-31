//
// Created by Zhang Yulei on 9/19/20.
//

#include "DataIO/PEvent.h"

#include "TObjectTable.h"
#include "TROOT.h"


PEvent::PEvent() {
    mcparticle_col = new mcp_map();
    step_col = new step_map();
    hit_col = new hit_map();

}

void PEvent::Initialization(CleanType ct) {
    // clean constant variables

    // Clean maps
    ClearMap(mcparticle_col, ct);
    ClearMap(step_col, ct);
    ClearMap(hit_col, ct);
}

vector<MCParticle *> *PEvent::GetData(const TString &col_name, MCParticle_DataType) {
    return GetDataVec<MCParticle>(col_name, mcparticle_col);
}

vector<PStep *> *PEvent::GetData(const TString &col_name, ParticleStep_DataType) {
    return GetDataVec<PStep>(col_name, step_col);
}

vector<PHit *> *PEvent::GetData(const TString &col_name, DetectorHit_DataType) {
    return GetDataVec<PHit>(col_name, hit_col);
}

bool PEvent::RegisterCollection(const TString &col_name, Phantom_DataType datatype) {
    bool result = false;
    switch (datatype) {
        case Phantom_DataType::MCParticle:
            result = RegisterColMap<mcp_map, MCParticle>(col_name, mcparticle_col);
            break;
        case Phantom_DataType::ParticleStep:
            result = RegisterColMap<step_map, PStep>(col_name, step_col);
            break;
        case Phantom_DataType::DetectorHit:
            result = RegisterColMap<hit_map, PHit>(col_name, hit_col);
            break;
    }
    return result;
}

bool PEvent::DeleteCollection(const TString &col_name, Phantom_DataType datatype) {
    bool result = false;
    switch (datatype) {
        case Phantom_DataType::MCParticle:
            result = DeleteColMap<mcp_map, MCParticle>(col_name, mcparticle_col);
            break;
        case Phantom_DataType::ParticleStep:
            result = DeleteColMap<step_map, PStep>(col_name, step_col);
            break;
        case Phantom_DataType::DetectorHit:
            result = DeleteColMap<hit_map, PHit>(col_name, hit_col);
            break;
    }
    return result;
}

void PEvent::ListAllCollections(const TString &str) {
    Printf("\n[Memory Check] %s", str.Data());
    Printf("Collection Name               size");
    Printf("==================================");
    ListCollection(mcparticle_col);
    ListCollection(step_col);
    ListCollection(hit_col);
    Printf("==================================");
}


#ifdef MEMCK

void PEvent::PrintObjectStatistics(const TString &str) {

    std::vector<std::string> Mem_Collections = {"PEvent", "PStep", "MCParticle", "PHit"};

    gObjectTable->UpdateInstCount();

    unsigned int n, h, si, ncum = 0, hcum = 0, scum = 0, tcum = 0, thcum = 0;
    Printf("\n[Memory Check] %s", str.Data());
    Printf("class                       count    on heap     size(bytes)    total size(KB)    heap size(KB)");
    Printf("===============================================================================================");
    for (auto cl_name: Mem_Collections) {
        auto cl = gROOT->GetClass(cl_name.data());
        if (cl == nullptr) continue;
        n = cl->GetInstanceCount();
        h = cl->GetHeapInstanceCount();
        si = cl->Size();
        if (n > 0) {
            Printf("%-24s %8d%11d%16d%18.2f%17.2f", cl->GetName(), n, h, si, n * si / 1000., h * si / 1000.);
            ncum += n;
            hcum += h;
            scum += si;
            tcum += n * si;
            thcum += h * si;
        }
    }
    Printf("-----------------------------------------------------------------------------------------------");
    Printf("Total:                   %8d%11d%16d%18.2f%17.2f", ncum, hcum, scum, tcum / 1000., thcum / 1000.);
    Printf("===============================================================================================");
}


#endif





