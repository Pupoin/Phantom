#include "PctDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4String.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"    
#include "G4ios.hh"
#include "G4GDMLParser.hh"
#include "G4SDManager.hh"
#include "PctSdTracker.hh"
#include "PctSdTof.hh"
#include "PctSdEcal.hh"

PctDetectorConstruction::PctDetectorConstruction(G4VPhysicalVolume *pWorld, G4GDMLParser *parser)
	: G4VUserDetectorConstruction(), fWorldPhys(pWorld), fParser(parser), fConstructed(false)
{
}

PctDetectorConstruction::~PctDetectorConstruction()
{
}

G4VPhysicalVolume *PctDetectorConstruction::Construct()
{
	if (!fConstructed)
	{
		// load gdml
		//ConstructSDandField();
		fConstructed = true;
	}
	return fWorldPhys;
}

void PctDetectorConstruction::ConstructSDandField()
{
	//------------------------------------------------
	// Sensitive detectors
	//------------------------------------------------
	G4SDManager *SDman = G4SDManager::GetSDMpointer();
	// retrieve auxiliary information for sensitive detector
	// we are looking for
	// sensitive detectors setting them for the volumes
	const G4GDMLAuxMapType *auxmap = fParser->GetAuxMap();
	G4cout << "Found " << auxmap->size()
		   << " volume(s) with auxiliary information."
		   << G4endl << G4endl;

	for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
		 iter != auxmap->end(); iter++)
	{
		G4LogicalVolume *currLogicalVol = iter->first;
		G4cout << "Logical volume " << currLogicalVol->GetName()
			   << " has the following list of auxiliary information: "
			   << G4endl << G4endl;
		for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
			 vit != (*iter).second.end(); vit++)
		{
			if ((*vit).type == "SensDet") // hard-coded, should be paid attention
			{
				G4cout << "Attaching sensitive detector " << (*vit).value
					   //<<  G4endl << G4endl;
					   << " to volume " << currLogicalVol->GetName()
					   <<G4endl;

				G4VSensitiveDetector *mydet = SDman->FindSensitiveDetector((*vit).value);

				if(!mydet){
					G4String SDDetectorTypeName = (*vit).value;
					if (SDDetectorTypeName.contains("Tracker"))
					{
						G4String dir = "/";
						G4String trackerChamberSDname = SDDetectorTypeName;
						PctSdTracker *aTrackerSD =
							new PctSdTracker(dir + trackerChamberSDname, trackerChamberSDname);
						SDman->AddNewDetector(aTrackerSD);
						mydet = aTrackerSD;
						SetSensitiveDetector(currLogicalVol, aTrackerSD);
					}
					if (SDDetectorTypeName.contains("Scintillator"))
					{
						G4String dir = "/";
						G4String trackerChamberSDname = SDDetectorTypeName;
						PctSdTof *aTrackerSD =
							new PctSdTof(dir + trackerChamberSDname, trackerChamberSDname);
						SDman->AddNewDetector(aTrackerSD);
						mydet = aTrackerSD;
						SetSensitiveDetector(currLogicalVol, aTrackerSD);
					}
				}
				if (mydet)
				{
					G4cout << "Activate sensitive detector"
						   << ", name=" << mydet->GetName()
						   << ", collName=" << mydet->GetCollectionName(0)
						   << ", fullName=" << mydet->GetFullPathName()
						   << G4endl;
				}
			}
		}
	}
}
