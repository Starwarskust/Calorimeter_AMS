#include "G4RunManager.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

// #include "G4UImanager.hh"
// #include "G4UIExecutive.hh"
// #include "G4VisExecutive.hh"

int main(int argc, char *argv[])
{
  // Read input
  // Input example: ./calorimeter 2212 250 260 0 1 $(date +%s)
  if (argc != 7) {
    G4cout << "Wrong number of input parameters" << G4endl;
    return 0;
  }
  G4int particlePDG = atoi(argv[1]); // PDG code of particle
  G4double energyMin = atoi(argv[2]); // MeV
  G4double energyMax = atoi(argv[3]); // MeV
  G4int runNumber = atoi(argv[4]); // run number
  G4int numberOfEvents = atoi(argv[5]); // number of events
  G4long seed = atoi(argv[6]); // seed
  G4cout << "Input particlePDG: " << particlePDG << "\n"
         << "Input energyMin: " << energyMin << " MeV" << "\n"
         << "Input energyMax: " << energyMax << " MeV" << "\n"
         << "Input runNumber: " << runNumber << "\n"
         << "Input numberOfEvents: " << numberOfEvents << "\n"
         << "Input seed: " << seed << G4endl;

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  CLHEP::HepRandom::setTheSeed(seed + runNumber);

  // Construct RunManager and initialize G4 kernel
  G4RunManager *runManager = new G4RunManager;
  runManager->SetUserInitialization(new DetectorConstruction());
  runManager->SetUserInitialization(new PhysicsList());
  runManager->SetUserInitialization(new ActionInitialization(particlePDG, energyMin, energyMax, runNumber));
  runManager->Initialize();

  // Get the pointer to the User Interface manager
  // G4UImanager *UImanager = G4UImanager::GetUIpointer();
  // UImanager->ApplyCommand("/tracking/verbose 0");
  // G4VisManager *visManager = new G4VisExecutive();
  // visManager->Initialize();
  // G4UIExecutive *UI = new G4UIExecutive(argc, argv);
  // UImanager->ApplyCommand("/control/execute vis.mac");
  // UI->SessionStart();
  // delete UI;

  // Run 1 particle
  runManager->BeamOn(numberOfEvents);

  // Job termination
  delete runManager;

  return 0;
}
