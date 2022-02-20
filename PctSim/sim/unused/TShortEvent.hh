#pragma once

#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "evtdefs.hh"
#include <vector>

class TShortEvent
{
public:
    G4int ID;
    G4int NPV;//number of primary vertices
    std::vector<G4ThreeVector> PrimaryVertices;
    G4double T0;//event creation time
    //
    

    
public:
     TShortEvent(/* args */);
    ~ TShortEvent();

    inline void *operator new(std::size_t);
    inline void operator delete(void* anEvent);

};


extern G4EVENT_DLL G4Allocator<TShortEvent>*& anEventAllocator();

inline void* TShortEvent::operator new(std::size_t)
{ 
  if (!anEventAllocator()) anEventAllocator() = new G4Allocator<TShortEvent>;
  return (void*)anEventAllocator()->MallocSingle();
}

inline void TShortEvent::operator delete(void* anEvent)
{
  anEventAllocator()->FreeSingle((TShortEvent*)anEvent);
}