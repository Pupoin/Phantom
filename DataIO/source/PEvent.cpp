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
    Printf("[All Collections] %s", str.Data());
    Printf("================================================================");
    Printf("Col. Type                     Col. Name                     size");
    Printf("----------------------------------------------------------------");
    ListCollection(mcparticle_col, Form("%-30s", "MCParticle"));
    ListCollection(step_col, Form("%-30s", "ParticleStep: PStep"));
    ListCollection(hit_col, Form("%-30s", "DetectorHit: PHit"));
    Printf("----------------------------------------------------------------");
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

void PEvent::PrintDetails() {
    for (const auto &m : *mcparticle_col) {
        Printf("\n");
        PrintHeader(m.first, "MCParticle");
        for (auto v : m.second) {
            std::cout << *v << std::endl;
        }
    }

    for (const auto &m : *step_col) {
        Printf("\n");
        PrintHeader(m.first, "ParticleStep(PStep)");
        for (auto v : m.second) {
            std::cout << *v << std::endl;
        }
    }
    for (const auto &m : *hit_col) {
        Printf("\n");
        PrintHeader(m.first, "DetectorHit(PHit)");
        for (auto v : m.second) {
            std::cout << *v << std::endl;
        }
    }
}

void PEvent::PrintHeader(const TString &col_name, const TString &class_type) {
    Printf("==>  Collection Type: %s, Collection Name: %s", col_name.Data(), class_type.Data());

    if (class_type == "ParticleStep(PStep)") {
        Printf("%s", std::string(140, '=').data());
        Printf("|  %-5s  |  %3s%-25s  |  %-8s%-38s  |   %-21s   %-15s  |", "id", "", "Position (x, y, z) [mm]", "",
               "4-Momentum (px, py, pz, E) [MeV]", "Physical Volume", "Process");
        Printf("%s", std::string(140, '-').data());
    }

    if (class_type == "DetectorHit(PHit)") {
        Printf("%s", std::string(76, '=').data());
        Printf("|   %-5s  |  %3s%-25s  |  %16s  | %s |", "id", "", "Position (x, y, z) [mm]", "E [MeV], T [ns]", "Cell ID");
        Printf("%s", std::string(76, '-').data());
    }

    if (class_type == "MCParticle") {
        Printf("%s", std::string(186, '=').data());
        Printf("| %-5s | %-8s  %-8s | %12s%-58s | %3s%-25s | %3s%-25s | %-15s %-8s |", "id", "Name", "PDG", "","Momentum (px, py, pz), E, Mass, ERemain [MeV]" ,  "","Vertex (x, y, z) [mm]", "","End (x, y, z) [mm]", "Create Process", "ParentID");
        Printf("%s", std::string(186, '-').data());
    }

}





