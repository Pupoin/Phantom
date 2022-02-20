//created by hujf@scnu.edu.cn @ 2021.10.22
//
#ifndef PctDetectorConstruction_h
#define PctDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4GDMLParser.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4String;

class PctDetectorConstruction : public G4VUserDetectorConstruction
{
	public:
		//we create a detector with the gdml file only
		PctDetectorConstruction(G4VPhysicalVolume* pWorld, G4GDMLParser* parser);
		virtual ~PctDetectorConstruction();

	public:
		virtual G4VPhysicalVolume* Construct();

		G4LogicalVolume* FindVolume(const G4String& name){
			return fParser->GetVolume(name);
		}

//we will provide more functions for this detector interface

	private:
		virtual void ConstructSDandField();

	private:
		G4VPhysicalVolume* fWorldPhys;
		G4bool fConstructed;
		G4GDMLParser* fParser;
		//G4String fGdmlFile;
};

#endif

