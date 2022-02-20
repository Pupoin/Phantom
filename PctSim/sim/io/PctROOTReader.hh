
#pragma once

#include "g4root.hh"
#include "PctDataType.hh"

class PctROOTReader
{
private:
    static PctROOTReader* pReaderHandle;
    std::vector<G4int>  data;
    int num;
    PCT::READER::PctEvent *fevt;

public:
    static PctROOTReader *Instance();

    ~PctROOTReader()
    {
        data.clear();
        delete G4AnalysisReader::Instance();
    };

    bool open(const char *fileName)
    {
        G4AnalysisReader *analysisReader = G4AnalysisReader::Instance();
        analysisReader->SetFileName(fileName);
        G4int ntupleId = analysisReader->GetNtuple("evt");
        bool isSucc = false;
        if( ntupleId >= 0){
            analysisReader->SetNtupleIColumn("num", num);
            analysisReader->SetNtupleIColumn("data", data);
            isSucc = true;
        }
        return isSucc;
    }
    PCT::READER::PctEvent *next()
    {
        G4AnalysisReader *analysisReader = G4AnalysisReader::Instance();
        if (!analysisReader->GetNtupleRow())
        {
            return NULL;
        }
        G4cout << "load " << num << " integers, real size #"<<data.size() << G4endl;
        // decode current event
        int idx =0;
        fevt->run_id = data[idx] >> 20;
        fevt->evt_id = data[idx++] & 0b111111111111;

        int ntrj = data[idx++];
        fevt->tracks.resize(ntrj);
        for (size_t ip = 0; ip < ntrj; ++ip)
        {
            fevt->tracks[ip].pdg_code = data[idx ++];     // pdg
            fevt->tracks[ip].track_id = data[idx ++];     // track id
            fevt->tracks[ip].parent_id = data[idx ++];    // parent

            fevt->tracks[ip].points.resize(data[idx ++]); // number of points
            for (size_t pos = 0; pos < fevt->tracks[ip].points.size(); ++pos)
            {
                fevt->tracks[ip].points[pos].x = data[idx++] / 100.;
                fevt->tracks[ip].points[pos].y = data[idx++] / 100.;
                fevt->tracks[ip].points[pos].z = data[idx++] / 100.;
            }
        }
        //int nColl = data[idx++];

        return fevt;
    }
    int close()
    {
        G4AnalysisManager::Instance()->CloseFile();
    }

private:
    PctROOTReader()
    {
        fevt = new PCT::READER::PctEvent();
    }
    PctROOTReader(const PctROOTReader &);
    PctROOTReader &operator=(const PctROOTReader &);
};
