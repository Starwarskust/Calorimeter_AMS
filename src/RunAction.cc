#include "RunAction.hh"

RunAction::RunAction(G4int runNumber)
: fRunNumber(runNumber)
{}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  std::string filename = "../data/output_" + std::to_string(fRunNumber) + ".csv";
  analysisManager->OpenFile(filename);

  analysisManager->CreateNtuple("Ntuple1", "PMT response");
  for (G4int i = 0; i < 1296; i++)
    analysisManager->CreateNtupleDColumn(std::to_string(i));
  analysisManager->FinishNtuple();

  analysisManager->CreateNtuple("Ntuple2", "Primary particle parameters");
  analysisManager->CreateNtupleDColumn("Ekin");
  analysisManager->CreateNtupleDColumn("X");
  analysisManager->CreateNtupleDColumn("Y");
  analysisManager->CreateNtupleDColumn("theta");
  analysisManager->CreateNtupleDColumn("phi");
  analysisManager->CreateNtupleDColumn("Z_end");
  analysisManager->CreateNtupleSColumn("LastProcessPreStep");
  analysisManager->CreateNtupleSColumn("LastProcessPostStep");
  analysisManager->FinishNtuple();

  // analysisManager->CreateNtuple("Ntuple3", "Tracks");
  // analysisManager->CreateNtupleIColumn("TrackID");
  // analysisManager->CreateNtupleIColumn("ParticlePDG");
  // analysisManager->CreateNtupleDColumn("X");
  // analysisManager->CreateNtupleDColumn("Y");
  // analysisManager->CreateNtupleDColumn("Z");
  // analysisManager->FinishNtuple();
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
