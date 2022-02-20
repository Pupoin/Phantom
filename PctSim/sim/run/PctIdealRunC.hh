#pragma once

//===================================================
//type C run:
//save data per step for each particle into a zipped RAW file
//===================================================

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4PhysicsVector.hh"

#include "PctRawWriter.hh"
//定义三个联合体来压缩数据输出
    union RUN_EVT_NTRJ_TYPE
    {
        G4long value;
        struct ITEM_TYPE 
        {
            G4int run : 16;
            G4int evt : 32;
            G4int ntrj : 16;
        } item;
    };
    union PDG_TID_PID_NPOINT_TYPE
    {
        G4long value;
        struct ITEM_TYPE
        {
            G4int pdg_id : 16;
            G4int trk_id : 16;
            G4int parent : 16;
            G4int npoints : 16;
        } item;
    } ;
    union POSITION_TYPE
    {
        G4long value;
        struct ITEM_TYPE
        {
            G4int x: 21;
            G4int y: 21;
            G4int z: 22;
        } item;
    } ;

class  PctIdealRunC : public G4Run
{
private:
    /* data */
    RUN_EVT_NTRJ_TYPE run_evt_ntrj;
    PDG_TID_PID_NPOINT_TYPE pdg_tid_pid_npoints;
    POSITION_TYPE coor_3d;
    std::vector<G4long> databuff;

public:
     PctIdealRunC ();
    ~PctIdealRunC ();

    virtual void RecordEvent(const G4Event *);
    virtual void Merge(const G4Run *);
};
