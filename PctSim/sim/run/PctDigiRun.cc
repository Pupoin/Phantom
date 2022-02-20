#include "PctDigiRun.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4AutoLock.hh"
#include "g4root.hh"
#include "G4VTrajectoryPoint.hh"
#include <mutex>
#include <thread>
#include "PctRawWriter.hh"
#include "PctROOTWriter.hh"
#include "G4SystemOfUnits.hh"
#include "PctRunOpt.hh"
#include "G4VHit.hh"
#include "PctSdVHit.hh"

namespace {
	G4Mutex PctRecordEventMutexPctDigiRun = G4MUTEX_INITIALIZER;
}

PctDigiRun::PctDigiRun():
	G4Run(/* args */)
{
	databuff.reserve(10000);
}

PctDigiRun::~PctDigiRun()
{
	databuff.clear();
}
void PctDigiRun::Merge(const G4Run *aRun)
{
	G4Run::Merge(aRun);
}

void PctDigiRun::RecordEvent(const G4Event *evt)
{
	//G4cout<<"PctDigiRun::"<<__FUNCTION__<<", run = "<<this->GetRunID()<<", event="<<evt->GetEventID()<<G4endl;
	G4Run::RecordEvent(evt);

	databuff.clear();

	G4HCofThisEvent *pHitColl = evt->GetHCofThisEvent();
	G4int nHitColl = 0;
	if (pHitColl)
	{
		nHitColl = pHitColl->GetNumberOfCollections();
		//G4cout << "There are " << nHitColl << " hits collections." << G4endl;
		databuff.push_back(nHitColl); // number of hits collections

		for (G4int i = 0; i < nHitColl; i++)
		{
			G4VHitsCollection *ithHitColl = pHitColl->GetHC(i);
			if (ithHitColl)
			{
				G4int collID = ithHitColl->GetColID();
				G4String collName = ithHitColl->GetName();
				G4String sdName = ithHitColl->GetSDname();
				size_t nHits = ithHitColl->GetSize();
				//G4cout << "The " << i << " collections#" << collID
				//	   << " name=" << collName
				//	   << " sdname=" << sdName
				//	   << " has " << nHits << " hits." << G4endl;

				databuff.push_back(collID);
				databuff.push_back(nHits);
				for (size_t j = 0; j < nHits; ++j)
				{
					PctSdVHit* aHit = (PctSdVHit*)ithHitColl->GetHit(j);
					databuff.push_back(aHit->GetID());
					databuff.push_back(G4int(100*aHit->GetX()));
					databuff.push_back(G4int(100*aHit->GetY()));
					databuff.push_back(G4int(100*aHit->GetZ()));
					databuff.push_back(G4int(100*aHit->GetTime()));
					databuff.push_back(G4int(100*aHit->GetEnergy()));
					//aHit->Print();
				}
				//G4cout << "----------------------------------------" << G4endl;
			}
		}
	}
#ifdef USE_ROOT_IO
	PctROOTWriter *pWriter = PctROOTWriter::Instance();
	G4AutoLock lock(&PctRecordEventMutexPctDigiRun);
	pWriter->write(databuff);
	lock.unlock();
#else
	PctRawWriter *pWriter = PctRawWriter::Instance();
	G4AutoLock lock(&PctRecordEventMutexPctDigiRun);
	pWriter->write(databuff);
	lock.unlock();
#endif
}
