#ifndef SensitiveDetector_hh
#define SensitiveDetector_hh

#include "G4VSensitiveDetector.hh"

#include "G4AnalysisManager.hh"

class SensitiveDetector : public G4VSensitiveDetector
{
  public:
    SensitiveDetector(G4String name);
    ~SensitiveDetector();
    virtual void Initialize(G4HCofThisEvent *hitCollection) override;
    virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;
    virtual void EndOfEvent(G4HCofThisEvent *hitCollection) override;

  private:
    std::vector<G4double> calorimeterResponse;
};

#endif
