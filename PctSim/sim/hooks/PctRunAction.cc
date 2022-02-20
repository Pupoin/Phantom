
#include "PctRunAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4ios.hh"
#include "G4Threading.hh"
#include "PctIdealRunA.hh"
#include "PctIdealRunB.hh"
#include "PctDigiRun.hh"
#include "PctFullRun.hh"
//#include "PctFullRunX.hh"
#include "g4root.hh"
#include "G4PhysicsVector.hh"
#include "PctRawWriter.hh"
#include "PctROOTWriter.hh"
#include "PctRunOpt.hh"
//#include "PctRootManager.hh"

	PctRunAction::PctRunAction()
:G4UserRunAction(), isFileOK(false)
{;}


PctRunAction::~PctRunAction()
{;}

namespace {
	G4Mutex PctGenerateRunMutex = G4MUTEX_INITIALIZER;
}

G4Run* PctRunAction::GenerateRun(){
	//return new PctIdealRunA();
	//return new PctDigiRun();
	return new PctFullRun();
	//return new PctFullRunX();
}

void PctRunAction::BeginOfRunAction(const G4Run* aRun)
{
	//reset run number
	G4AutoLock lock(&PctGenerateRunMutex);
	G4Run *bRun = const_cast<G4Run*>(aRun);
	bRun->SetRunID(PctRunOpt::OptRunNumber++);
	lock.unlock();

	// G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
	// G4RunManager::GetRunManager()->SetRandomNumberStore(true);
	//G4cout <<"PctRunAction::"<< __FUNCTION__ << "::RUN ID=" << aRun->GetRunID() << ", " << G4Threading::G4GetThreadId() << G4endl;

#ifdef USE_ROOT_IO
	PctROOTWriter *pWriter = PctROOTWriter::Instance();
	isFileOK = pWriter->open(PctRunOpt::OptOutputFile.c_str());
	if (isFileOK)
	{
		pWriter->book();
	}
	else
	{
		G4cout << "Fail to open file PctSimu.root for writing." << G4endl;
	}
#endif
#ifdef USE_RAW_IO
	if (IsMaster())
	{
		PctRawWriter *pWriter = PctRawWriter::Instance();
		isFileOK = pWriter->open(PctRunOpt::OptOutputFile.c_str(), "w");
		if (!isFileOK)
		{
			std::cerr << "Fail to open file PctSimu.raw for writing." << std::endl;
		}
	}
#endif
#ifdef USE_ROOT_MANAGER
	if (IsMaster())
	{
		PctRootManager* pWriter = PctRootManager::Instance();
		isFileOK = pWriter->open(PctRunOpt::OptOutputFile);
		if (!isFileOK)
		{
			std::cerr << "Fail to open file PctSimu.root for writing." << std::endl;
		}
	}
#endif
	//G4cout <<"PctRunAction::"<< __FUNCTION__ << "::RUN ID=" << aRun->GetRunID() << ", " << G4Threading::G4GetThreadId()<<" done." << G4endl;
}

// serilize data here
void PctRunAction::EndOfRunAction(const G4Run *aRun)
{
	if (IsMaster())
		G4cout << __FUNCTION__
			   << "::Master RUN ID=" << aRun->GetRunID()
			   << ", " << G4Threading::G4GetThreadId()
			   << ", " << aRun->GetNumberOfEvent() << G4endl;
	else
		G4cout << __FUNCTION__
			   << "::Worker RUN ID=" << aRun->GetRunID()
			   << ", " << G4Threading::G4GetThreadId()
			   << ", " << aRun->GetNumberOfEvent() << G4endl;


#ifdef USE_ROOT_IO
	//保存一个run的信息
	//随机数种子，运行日期，run相关的配置，事例数据
	if (isFileOK)
	{
		PctROOTWriter *pWriter = PctROOTWriter::Instance();
		pWriter->flush();
		pWriter->close();
		// delete pWriter;
	}
#endif
#ifdef USE_RAW_IO
	if (IsMaster() && isFileOK)
	{
		PctRawWriter *pWriter = PctRawWriter::Instance();
		pWriter->flush();
		pWriter->close();
		delete pWriter;
	}
#endif
#ifdef USE_ROOT_MANAGER
	if (IsMaster() && isFileOK)
	{
		PctRootManager* pWriter = PctRootManager::Instance();
		pWriter->flush();
		delete pWriter;
	}
#endif
	//G4cout <<"PctRunAction::"<<__FUNCTION__<<" done"<<G4endl;
}
