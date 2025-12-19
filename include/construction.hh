#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Sphere.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();
	G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}

	virtual G4VPhysicalVolume *Construct();

private:
 	virtual void ConstructSDandField();

	G4double xWorld, yWorld, zWorld;
	G4Box *solidLead;//, *solidDetector;
	G4Tubs *solidDetector;
	//G4Tubs *solidScintillator;
	G4LogicalVolume *logicWorld, *logicSource, *logicLead, *logicDetector;
	G4VPhysicalVolume *physWorld, *physSource, *physLead, *physDetector;
	G4Material *worldMat, *leadMat, *detMat, *matF18;
	G4Sphere *solidSource;
	G4Isotope *F18;
	G4Element *elF18;

	G4OpticalSurface *mirrorSurface;

	G4GenericMessenger *fMessenger;
	G4LogicalVolume *fScoringVolume;
	void DefineMaterials();
};
#endif
