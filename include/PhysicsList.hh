#ifndef PhysicsList_hh
#define PhysicsList_hh

#include "FTFP_BERT.hh"

#include "G4SystemOfUnits.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

class PhysicsList : public FTFP_BERT
{
  public:
    PhysicsList();
    ~PhysicsList();
    // virtual void SetCuts() override;
};

#endif
