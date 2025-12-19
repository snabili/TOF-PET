#include "trackingaction.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"

MyTrackingAction::MyTrackingAction() {}

MyTrackingAction::~MyTrackingAction() {}

void MyTrackingAction::PreUserTrackingAction(const G4Track* track) {
    G4String particleName = track->GetDefinition()->GetParticleName();

    if (particleName == "gamma") {
        G4String creatorProcess = track->GetCreatorProcess() ?
            track->GetCreatorProcess()->GetProcessName() : "primary";

        G4double energy = track->GetTotalEnergy();
        G4int parentID = track->GetParentID();

        G4cout << "Gamma created: "
               << "Creator = " << creatorProcess
               << " | Energy = " << energy / keV << " keV"
               << " | Parent ID = " << parentID
               << G4endl;
    }
}

