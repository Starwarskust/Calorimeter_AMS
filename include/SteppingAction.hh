#ifndef SteppingAction_hh
#define SteppingAction_hh

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"

#include "G4VProcess.hh"

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction();
    ~SteppingAction() override;
    void UserSteppingAction(const G4Step *step) override;

  private:
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
};

#endif
