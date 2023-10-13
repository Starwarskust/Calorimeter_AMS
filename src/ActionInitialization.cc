#include "ActionInitialization.hh"

ActionInitialization::ActionInitialization(G4int particlePDG, G4double energyMin, G4double energyMax, G4int runNumber)
: fParticlePDG(particlePDG),
  fEnergyMin(energyMin),
  fEnergyMax(energyMax),
  fRunNumber(runNumber)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction(fParticlePDG, fEnergyMin, fEnergyMax));
  SetUserAction(new RunAction(fRunNumber));
  SetUserAction(new SteppingAction());
}
