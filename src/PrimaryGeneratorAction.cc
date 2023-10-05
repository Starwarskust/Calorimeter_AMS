#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(G4int particlePDG, G4double energyMin, G4double energyMax)
: fParticleGun(0),
  fParticlePDG(particlePDG),
  fEnergyMin(energyMin),
  fEnergyMax(energyMax)
{
  fParticleGun = new G4ParticleGun();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  G4ParticleDefinition *particle = 0;
  if (G4ParticleTable::GetParticleTable()->FindParticle(fParticlePDG))
    particle = G4ParticleTable::GetParticleTable()->FindParticle(fParticlePDG);
  else if (G4IonTable::GetIonTable()->GetIon(fParticlePDG))
    particle = G4IonTable::GetIonTable()->GetIon(fParticlePDG);
  else
    std::cerr << "Error: particle was not found in G4ParticleTable and G4IonTable" << std::endl;

  G4double Ekin = CLHEP::RandFlat::shoot(fEnergyMin, fEnergyMax); // MeV
  G4double X = CLHEP::RandFlat::shoot(-200., 200.); // mm
  G4double Y = CLHEP::RandFlat::shoot(-200., 200.); // mm
  G4double theta = asin(CLHEP::RandFlat::shoot());
  G4double phi = CLHEP::RandFlat::shoot(CLHEP::twopi);

  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(1, 0, Ekin);
  analysisManager->FillNtupleDColumn(1, 1, X);
  analysisManager->FillNtupleDColumn(1, 2, Y);
  analysisManager->FillNtupleDColumn(1, 3, theta);
  analysisManager->FillNtupleDColumn(1, 4, phi);

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(Ekin);
  fParticleGun->SetParticlePosition(G4ThreeVector(X*mm, Y*mm, 0.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}
