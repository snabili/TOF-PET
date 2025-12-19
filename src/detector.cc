#include "detector.hh"

std::ofstream outFile("tof_hits.csv", std::ios::app); 

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    fTotalEnergyDeposited = 0.;
}

MySensitiveDetector::~MySensitiveDetector()
{
    outFile.close();
}

void MySensitiveDetector::Initialize(G4HCofThisEvent *)
{
    fTotalEnergyDeposited = 0.;
}



G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    G4Track* track = aStep->GetTrack();
    G4String particleName   = aStep->GetTrack()->GetDefinition()->GetParticleName();
    //if (track->GetGlobalTime() > 1e6 * ns) return false; // Time cutoff: to eliminate fake longlived particles 
    G4String creatorProcess = track->GetCreatorProcess() ? track->GetCreatorProcess()->GetProcessName() : "primary";

    G4cout << "creatorProcess: " << creatorProcess << G4endl;

    if (particleName=="gamma" && creatorProcess == "annihil")
    {
        G4double gammaEnergy = track->GetTotalEnergy();
        G4cout << "Gamma true energy: " << gammaEnergy / keV << " keV" << G4endl;
    }

    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    G4double fMomPhotonMag = momPhoton.mag();

    G4double fWlen = (1.239841939 * eV / fMomPhotonMag) * 1E+03;


    G4double energyDeposited = aStep->GetTotalEnergyDeposit();
    G4double time = aStep->GetPreStepPoint()->GetGlobalTime();

    if (energyDeposited > 0. && aStep->GetTrack()->GetDefinition()->GetParticleType() != "optical")
    {
        fTotalEnergyDeposited += energyDeposited;
        G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();
        G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

        // std::cout << "Hit: " << particleName
        //           << " | Energy deposit: " << energyDeposited / keV << " keV"
        //           << " | Time: " << time  << " ns"
        //           << " | Position: " << pos << std::endl;

        outFile << std::fixed << std::setprecision(3)
                << particleName << ","
                << energyDeposited / keV << ","
                << time / ns << ","
                << pos.x() / mm << ","
                << pos.y() / mm << ","
                << pos.z() / mm << "\n";
    }

    
    G4ThreeVector pos       = aStep->GetPreStepPoint()->GetPosition();

    analysisManager->FillNtupleIColumn(0, 0, eventID);
    analysisManager->FillNtupleDColumn(0, 1, pos.x());
    analysisManager->FillNtupleDColumn(0, 2, pos.y());
    analysisManager->FillNtupleDColumn(0, 3, pos.z());
    analysisManager->FillNtupleDColumn(0, 4, time / ns);
    analysisManager->FillNtupleDColumn(0, 5, energyDeposited / keV);
    analysisManager->FillNtupleSColumn(0, 6, particleName);
    analysisManager->FillNtupleDColumn(0, 7, fWlen);
    analysisManager->AddNtupleRow(0);


    // only if G4OpticalPhysics are included in the physicslists
    if (aStep->GetTrack()->GetDefinition()->GetParticleType() == "optical") {
        // Handle optical photon timing and position
        G4cout << "Optical Photons " << G4endl;
    }

    if (aStep->GetTrack()->GetDefinition()->GetParticleName()=="gamma")
    {
        
        G4String creatorProcess = track->GetCreatorProcess() ? track->GetCreatorProcess()->GetProcessName() : "primary";
        G4int parentID = track->GetParentID();

        // G4cout << "Gamma from: " << creatorProcess
        //     << " | Parent ID: " << parentID
        //     << " | Time: " << track->GetGlobalTime() / ns << " ns" << G4endl;
    }
    

    return true;

}
