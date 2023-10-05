#include "PhysicsList.hh"

PhysicsList::PhysicsList()
: FTFP_BERT(0)
{
  RegisterPhysics(new G4OpticalPhysics());
  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList()
{}

// void PhysicsList::SetCuts()
// {
//   SetCutValue(0.1*mm, "proton");
//   SetCutValue(0.1*mm, "e-");
//   SetCutValue(0.1*mm, "e+");
//   SetCutValue(0.1*mm, "gamma");      
// }
