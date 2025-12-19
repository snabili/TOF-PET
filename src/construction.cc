#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	xWorld = 3. * m;
	yWorld = 3. * m;
	zWorld = 3. * m;

	DefineMaterials();
	
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager *nist = G4NistManager::Instance();

	worldMat = nist->FindOrBuildMaterial("G4_AIR");
	leadMat  = nist->FindOrBuildMaterial("G4_Pb");
    //detMat   = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
	detMat = nist->FindOrBuildMaterial("G4_BGO");

	// Define Fluorine-18
    G4Isotope *F18 = new G4Isotope("F18", 9, 18, 18.000938 * g / mole);
    G4Element *elF18 = new G4Element("Fluorine-18", "F18", 1);
    elF18->AddIsotope(F18, 100.0 * perCent);
    matF18 = new G4Material("F18Source", 1.51 * g / cm3, 1);
    matF18->AddElement(elF18, 100.0 * perCent);

}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{	
	G4bool checkOverlaps = true;

	G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    // Fluorine source
    G4double sourceRadius = 1. * mm;

    solidSource = new G4Sphere("solidSource", 0.0, sourceRadius, 0.0, 360. * deg, 0.0, 180. * deg);
    logicSource = new G4LogicalVolume(solidSource, matF18, "logicSource");
    physSource = new G4PVPlacement(0, G4ThreeVector(0., 0., -1. * cm), logicSource, "physSource", logicWorld, 0, checkOverlaps);
    G4VisAttributes *sourceVisAtt = new G4VisAttributes(G4Colour::Red());//(1.0, 0.0, 1.0, 0.5));
    sourceVisAtt->SetForceSolid(true);
    logicSource->SetVisAttributes(sourceVisAtt);

    // G4double leadThickness = 2. * mm;
    // G4double leadSize = 10. * cm;
    // solidLead = new G4Box("solidLead", 0.5 * leadSize, 0.5 * leadSize, 0.5 * leadThickness);
    // logicLead = new G4LogicalVolume(solidLead, leadMat, "logicLead");
    // physLead = new G4PVPlacement(0, G4ThreeVector(0., 0., 5. * cm), logicLead, "physLead", logicWorld, false, checkOverlaps);
    // G4VisAttributes *leadVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.5));
    // leadVisAtt->SetForceSolid(false);
    // logicLead->SetVisAttributes(leadVisAtt);

    // G4double detectorSize = 10.0 * cm;
	// solidDetector = new G4Box("solidDetector", 0.5 * detectorSize, 0.5 * detectorSize, 0.5 * detectorSize);
    // logicDetector = new G4LogicalVolume(solidDetector, detMat, "logicDetector");
    // physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0. , 10.5 * cm), logicDetector, "physDetector", logicWorld, false, checkOverlaps);
    

	G4double detectorDiameter = 78. * cm;
	G4double detectorLength = 2. * m;
	solidDetector = new G4Tubs("solidDetector", detectorDiameter/2, detectorDiameter/2 + 5. * cm, detectorLength / 2, 0., 360. * deg);
    logicDetector = new G4LogicalVolume(solidDetector, detMat, "logicDetector"); 
	physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0. , 0.), logicDetector, "physDetector", logicWorld, false, checkOverlaps);
	G4VisAttributes *detVisAtt = new G4VisAttributes(G4Colour::Yellow());//(1.0, 1.0, 0.0, 0.5));
    detVisAtt->SetForceSolid(false);
    logicDetector->SetVisAttributes(detVisAtt);
	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
	logicDetector->SetSensitiveDetector(sensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);

	// Time cutoff: to eliminate not realistic long lived particles 
    G4UserLimits* limits = new G4UserLimits();
    limits->SetUserMaxTime(1e3 * ns);  // Limit to 1000 ns
    logicDetector->SetUserLimits(limits);
}
