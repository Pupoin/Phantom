
#pragma once

#include "g4root.hh"

class PctROOTWriter
{
private:
    static PctROOTWriter* pWriterHandle;
    std::vector<G4int>  pDataBuff;

public:
    static PctROOTWriter *Instance();

    ~PctROOTWriter(){
        //pDataBuff.clear();
    };

    bool open(const char *fileName)
    {
	    G4AnalysisManager* pAnaMgr = G4AnalysisManager::Instance();
	    pAnaMgr->SetCompressionLevel(9);
	    pAnaMgr->SetNtupleMerging(true);
	    return pAnaMgr->OpenFile(fileName);
    }
    bool book()
    {
	    G4AnalysisManager* pAnaMgr = G4AnalysisManager::Instance();
	    //pAnaMgr->SetFirstNtupleId(0);
	    pAnaMgr->CreateNtuple("evt", "data for all events");
	    pAnaMgr->CreateNtupleIColumn("num");
	    pAnaMgr->CreateNtupleIColumn("data", pDataBuff);
	    pAnaMgr->FinishNtuple();
	    return true;
    }
    size_t write(std::vector<G4int>& dataSource)
    {
	    G4AnalysisManager* pAnaMgr = G4AnalysisManager::Instance();
	    G4int nSize = dataSource.size();
	    pDataBuff.swap(dataSource);//swap for writing
	    pAnaMgr->FillNtupleIColumn(0, nSize);
	    pAnaMgr->AddNtupleRow();
	    dataSource.swap(pDataBuff);//restore
	    return nSize;
    }

    int flush()
    {
	    return G4AnalysisManager::Instance()->Write();
    }
    int close()
    {
	    return G4AnalysisManager::Instance()->CloseFile();
    }



private:
    PctROOTWriter()  {}
    PctROOTWriter(const PctROOTWriter &);
    PctROOTWriter &operator=(const PctROOTWriter &);
};
