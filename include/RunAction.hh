#ifndef RunAction_hh
#define RunAction_hh

#include "G4UserRunAction.hh"

#include "G4AnalysisManager.hh"

class RunAction : public G4UserRunAction
{
  public:
    RunAction(G4int runNumber);
    ~RunAction();
    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void   EndOfRunAction(const G4Run*) override;

  private:
    G4int fRunNumber;
};

#endif
