#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1); // number of particles per events

	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	//G4ParticleDefinition *particle = particleTable->FindParticle("proton");
	G4ParticleDefinition *particle = particleTable->FindParticle("geantino");
	//G4ParticleDefinition *particle = particleTable->FindParticle("gamma");

	G4ThreeVector pos(0.,0.,-1.*cm);
	G4ThreeVector mom(0.,0.,1.);

	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(0.6335 * MeV);
	fParticleGun->SetParticleDefinition(particle);

	
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();
	//if (particle==G4Geantino::Geantino())
	if (particle->GetParticleName()=="geantino")
	{
		G4int Z = 9;
		G4int A = 18;

		G4double charge = 0.*eplus;
		G4double energy = 0.6335 * MeV;

		G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z,A,energy);
		fParticleGun->SetParticleDefinition(ion);
		fParticleGun->SetParticleCharge(charge);
	}
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
