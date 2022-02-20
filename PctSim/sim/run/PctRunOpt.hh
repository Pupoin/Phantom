#pragma once
#include "G4Types.hh"
#include "G4String.hh"
//
class PctRunOpt 
{
    public:
    //tracking options
    static G4bool OptTrackingPrimaryAndSecondary;
    static G4bool OptTrackingPrimaryOnly;
    static G4bool OptTrackingSecondary;
    static G4String OptOutputFile;

    //run options
    static G4int OptRunNumber;
    static G4bool OptUseRawIO;
    //
};