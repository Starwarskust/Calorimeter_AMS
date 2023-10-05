#ifndef PrimaryGeneratorAction_hh
#define PrimaryGeneratorAction_hh

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4AnalysisManager.hh"
#include "Randomize.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(G4int particlePDG, G4double energyMin, G4double energyMax);
    ~PrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event *anEvent) override;

  private:
    G4ParticleGun *fParticleGun;
    G4int fParticlePDG;
    G4double fEnergyMin;
    G4double fEnergyMax;
};

#endif
