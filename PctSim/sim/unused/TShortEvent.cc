#include "TShortEvent.hh"


G4Allocator<TShortEvent>*& anEventAllocator()
{
  G4ThreadLocalStatic G4Allocator<TShortEvent>* _instance = nullptr;
  return _instance;
}


 TShortEvent:: TShortEvent(/* args */)
{
}

 TShortEvent::~ TShortEvent()
{
}