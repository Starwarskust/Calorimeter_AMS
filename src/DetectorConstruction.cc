#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  G4NistManager *nist = G4NistManager::Instance();
  G4bool checkOverlaps = false;
  G4int nSuperLayer = 9;

  G4double density = 0.;
  G4int nElem  = 0;
  G4int nAtom = 0;
  G4double fracMass = 100.;

  //--------------------------------------- World --------------------------------------------------

  G4double world_sizeXY = 700.*mm;
  G4double world_sizeZ  = 400.*mm;
  G4Box *world_solid = new G4Box("world_solid", world_sizeXY/2., world_sizeXY/2., world_sizeZ/2.);

  G4Material *world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4LogicalVolume *world_log = new G4LogicalVolume(world_solid, world_mat, "world_log");

  G4VPhysicalVolume *world_phys =
    new G4PVPlacement(0,                  // rotation
                      G4ThreeVector(),    // position
                      world_log,          // logical volume
                      "world_phys",       // name
                      0,                  // mother volume
                      false,              // boolean operation
                      0,                  // copy number
                      checkOverlaps);     // overlaps checking

  //---------------------------------------- Foil --------------------------------------------------

  G4double Foil_sizeXY = 648.0*mm;
  G4double Foil_sizeZ  = 166.5*mm;
  G4Box *Foil_solid = new G4Box("Foil_solid", Foil_sizeXY/2., Foil_sizeXY/2., Foil_sizeZ/2.);

  G4Material *Foil_mat = nist->FindOrBuildMaterial("G4_Pb");
  G4LogicalVolume *Foil_log = new G4LogicalVolume(Foil_solid, Foil_mat, "Foil_log");

  G4ThreeVector Foil_position(0., 0., Foil_sizeZ/2.);
  new G4PVPlacement(0,                 // rotation
                    Foil_position,     // position
                    Foil_log,          // logical volume
                    "Foil_phys",       // name
                    world_log,         // mother volume
                    false,             // boolean operation
                    0,                 // copy number
                    checkOverlaps);    // overlaps checking

  //------------------------------------ Fiber + Glue ----------------------------------------------

  std::vector<G4double> photonEnergy = {1.239841939*eV/0.6, 1.239841939*eV/0.4}; // from 400 nm to 600 nm

  //---- Fiber core ----

  G4double Fiber_core_sizeRin = 0.*mm;
  G4double Fiber_core_sizeRout = 0.48*mm;
  G4Tubs *Fiber_core_solid = new G4Tubs("Fiber_core_solid", Fiber_core_sizeRin, Fiber_core_sizeRout, Foil_sizeXY/2., 0.*deg, 360.*deg);

  G4Material *Fiber_core_mat = new G4Material("Polystylene", density = 1.050*g/cm3, nElem = 2); // C8H8
  Fiber_core_mat->AddElement(nist->FindOrBuildElement("H"), nAtom = 8);
  Fiber_core_mat->AddElement(nist->FindOrBuildElement("C"), nAtom = 8);

  std::vector<G4double> Fiber_core_rindex = {1.59, 1.59};
  std::vector<G4double> Fiber_core_absorption = {3.5*m, 3.5*m};
  std::vector<G4double> Fiber_core_emission_E = { // from 412 nm to 545 nm
    2.274*eV, 2.293*eV, 2.312*eV, 2.331*eV, 2.350*eV, 2.369*eV, 2.387*eV, 2.406*eV, 2.425*eV, 2.444*eV,
    2.463*eV, 2.482*eV, 2.500*eV, 2.519*eV, 2.538*eV, 2.557*eV, 2.576*eV, 2.595*eV, 2.613*eV, 2.632*eV,
    2.651*eV, 2.670*eV, 2.689*eV, 2.708*eV, 2.726*eV, 2.745*eV, 2.764*eV, 2.783*eV, 2.802*eV, 2.821*eV,
    2.839*eV, 2.858*eV, 2.877*eV, 2.896*eV, 2.915*eV, 2.934*eV, 2.952*eV, 2.971*eV, 2.990*eV, 3.009*eV};
  std::vector<G4double> Fiber_core_emission = { // from 412 nm to 545 nm
    0.0078, 0.0249, 0.0357, 0.0462, 0.0529, 0.0635, 0.0796, 0.0990, 0.1202, 0.1428,
    0.1663, 0.1969, 0.2334, 0.2538, 0.2731, 0.2931, 0.3295, 0.4008, 0.5061, 0.5639,
    0.6307, 0.6829, 0.7055, 0.7176, 0.7278, 0.7388, 0.7808, 0.8793, 0.9643, 0.9998,
    0.9669, 0.8968, 0.7672, 0.5107, 0.2444, 0.1193, 0.0504, 0.0220, 0.0079, 0.0010};
  G4MaterialPropertiesTable *Fiber_core_mpt = new G4MaterialPropertiesTable();
  Fiber_core_mpt->AddProperty("RINDEX", photonEnergy, Fiber_core_rindex);
  Fiber_core_mpt->AddConstProperty("SCINTILLATIONYIELD", 8000./MeV); // between 7000 and 10000 [doi.org/10.1088/1748-0221/17/12/P12020]
  Fiber_core_mpt->AddConstProperty("RESOLUTIONSCALE", 1.0);
  Fiber_core_mpt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.4*ns);
  Fiber_core_mpt->AddProperty("SCINTILLATIONCOMPONENT1", Fiber_core_emission_E, Fiber_core_emission);
  Fiber_core_mpt->AddProperty("ABSLENGTH", photonEnergy, Fiber_core_absorption);
  Fiber_core_mat->SetMaterialPropertiesTable(Fiber_core_mpt);
  G4LogicalVolume *Fiber_core_log = new G4LogicalVolume(Fiber_core_solid, Fiber_core_mat, "Fiber_core_log");

  //---- Fiber cladding ----

  G4double Fiber_cladding_sizeRin = 0.48*mm;
  G4double Fiber_cladding_sizeRout = 0.5*mm;
  G4Tubs *Fiber_cladding_solid = new G4Tubs("Fiber_cladding_solid", Fiber_cladding_sizeRin, Fiber_cladding_sizeRout, Foil_sizeXY/2., 0.*deg, 360.*deg);

  G4Material *Fiber_cladding_mat = new G4Material("PMMA", density = 1.190*g/cm3, nElem = 3); // С5H8O2
  Fiber_cladding_mat->AddElement(nist->FindOrBuildElement("H"), nAtom = 8);
  Fiber_cladding_mat->AddElement(nist->FindOrBuildElement("C"), nAtom = 5);
  Fiber_cladding_mat->AddElement(nist->FindOrBuildElement("O"), nAtom = 2);

  std::vector<G4double> Fiber_cladding_rindex = {1.49, 1.49};
  G4MaterialPropertiesTable *Fiber_cladding_mpt = new G4MaterialPropertiesTable();
  Fiber_cladding_mpt->AddProperty("RINDEX", photonEnergy, Fiber_cladding_rindex);
  Fiber_cladding_mat->SetMaterialPropertiesTable(Fiber_cladding_mpt);
  G4LogicalVolume *Fiber_cladding_log = new G4LogicalVolume(Fiber_cladding_solid, Fiber_cladding_mat, "Fiber_cladding_log");

  //---- Glue ----

  G4double Glue_sizeRin = 0.5*mm;
  G4double Glue_sizeRout = 0.562*mm;
  G4Tubs *Glue_solid = new G4Tubs("Glue_solid", Glue_sizeRin, Glue_sizeRout, Foil_sizeXY/2., 0.*deg, 360.*deg);

  // !!! ПЛОТНОСТЬ ЭТОГО ВЕЩЕСТВА НЕИЗВЕСТНА И ВЫБРАНА КАК У Hardener !!!
  G4Material *C7H20NO3 = new G4Material("Polyoxypropylediamine", density = 0.95*g/cm3, nElem = 4); // C7H20NO3
  C7H20NO3->AddElement(nist->FindOrBuildElement("H"), nAtom = 20);
  C7H20NO3->AddElement(nist->FindOrBuildElement("C"), nAtom = 7);
  C7H20NO3->AddElement(nist->FindOrBuildElement("N"), nAtom = 1);
  C7H20NO3->AddElement(nist->FindOrBuildElement("O"), nAtom = 3);

  G4Material *C6H15NO3 = new G4Material("Triethanolamine", density = 1.124*g/cm3, nElem = 4); // C6H15NO3
  C6H15NO3->AddElement(nist->FindOrBuildElement("H"), nAtom = 15);
  C6H15NO3->AddElement(nist->FindOrBuildElement("C"), nAtom = 6);
  C6H15NO3->AddElement(nist->FindOrBuildElement("N"), nAtom = 1);
  C6H15NO3->AddElement(nist->FindOrBuildElement("O"), nAtom = 3);

  G4Material *C6H20N3 = new G4Material("Aminoethylpiperazine", density = 0.984*g/cm3, nElem = 3); // C6H15N3
  C6H20N3->AddElement(nist->FindOrBuildElement("H"), nAtom = 15);
  C6H20N3->AddElement(nist->FindOrBuildElement("C"), nAtom = 6);
  C6H20N3->AddElement(nist->FindOrBuildElement("N"), nAtom = 3);

  // !!! ПЛОТНОСТЬ ЭТОГО ВЕЩЕСТВА НЕИЗВЕСТНА И ВЫБРАНА КАК У Hardener !!!
  G4Material *C4H10N2 = new G4Material("Diethylenediamine", density = 0.95*g/cm3, nElem = 3); // C4H10N2
  C4H10N2->AddElement(nist->FindOrBuildElement("H"), nAtom = 10);
  C4H10N2->AddElement(nist->FindOrBuildElement("C"), nAtom = 4);
  C4H10N2->AddElement(nist->FindOrBuildElement("N"), nAtom = 2);

  G4Material *hardener = new G4Material("Hardener", 0.95*g/cm3, nElem = 4);
  hardener->AddMaterial(C7H20NO3, fracMass = 90.0*perCent);
  hardener->AddMaterial(C6H15NO3, fracMass =  7.0*perCent);
  hardener->AddMaterial(C6H20N3,  fracMass =  1.5*perCent);
  hardener->AddMaterial(C4H10N2,  fracMass =  1.5*perCent);

  G4Material *epoxyResin = new G4Material("EpoxyResin", density = 1.14*g/cm3, nElem = 3); // C2H4O
  epoxyResin->AddElement(nist->FindOrBuildElement("H"), nAtom = 4);
  epoxyResin->AddElement(nist->FindOrBuildElement("C"), nAtom = 2);
  epoxyResin->AddElement(nist->FindOrBuildElement("O"), nAtom = 1);

  // !!! ПЛОТНОСТЬ ЭТОГО ВЕЩЕСТВА НЕИЗВЕСТНА И ВЫЧИСЛЕНА!!!
  G4Material *Glue_mat = new G4Material("Glue_mat", density = 1.0868*g/cm3, nElem = 2); // C2H4O
  Glue_mat->AddMaterial(epoxyResin, fracMass = 72*perCent);
  Glue_mat->AddMaterial(hardener,   fracMass = 28*perCent);
  G4LogicalVolume *Glue_log = new G4LogicalVolume(Glue_solid, Glue_mat, "Glue_log");

  //---- Assembly ----

  G4AssemblyVolume *assemblyDetector = new G4AssemblyVolume();
  G4Transform3D Tr = G4Transform3D();
  assemblyDetector->AddPlacedVolume(Fiber_core_log, Tr);
  assemblyDetector->AddPlacedVolume(Fiber_cladding_log, Tr);
  assemblyDetector->AddPlacedVolume(Glue_log, Tr);
  for (G4int iSuperLayer = 0; iSuperLayer < nSuperLayer; iSuperLayer++) {
    G4int alongX = 1 - iSuperLayer % 2; // if 1 then fiber tubes lies along x
    G4RotationMatrix Fiber_rotation(0., 90.*deg, alongX*90.*deg);
    for (G4int iLayer = 0; iLayer < 10; iLayer++) {
      for (G4int iFiber = 0; iFiber < 479; iFiber++) {
        G4double shift = -324 + 1.0125 + 0.675*(iLayer%2) + 1.35*iFiber;
        G4ThreeVector Fiber_position((1-alongX)*shift*mm, alongX *shift*mm,
                                     (-83.25 + 1.465 + iLayer*1.73 + iSuperLayer*18.5)*mm);
        Tr = G4Transform3D(Fiber_rotation, Fiber_position);
        assemblyDetector->MakeImprint(Foil_log, Tr);
      }
    }
  }

  //----------------------------------------- PMT --------------------------------------------------

  G4double PMT_sizeXY = 9*mm;
  G4double PMT_sizeZ  = 1*mm;
  G4Box *PMT_solid = new G4Box("PMT_solid", PMT_sizeXY/2., PMT_sizeXY/2., PMT_sizeZ/2.);

  G4Material *PMT_mat = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
  std::vector<G4double> PMT_rindex = {1.0, 1.0};
  G4MaterialPropertiesTable *PMT_mpt = new G4MaterialPropertiesTable();
  PMT_mpt->AddProperty("RINDEX", photonEnergy, PMT_rindex);
  PMT_mat->SetMaterialPropertiesTable(PMT_mpt);
  PMT_log = new G4LogicalVolume(PMT_solid, PMT_mat, "PMT_log");

  for (G4int iSuperLayer = 0; iSuperLayer < nSuperLayer; iSuperLayer++) {
    G4int alongX = 1 - iSuperLayer % 2; // if 1 then fiber tubes lies along x
    G4RotationMatrix *PMT_rotation = new G4RotationMatrix(alongX*CLHEP::halfpi, CLHEP::halfpi, 0.);
    for (G4int iLine = 0; iLine < 2; iLine++) {
      for (G4int iPMT = 0; iPMT < 36; iPMT++) {
        G4int side = pow(-1., iPMT % 2);
        for (G4int iAnode = 0; iAnode < 2; iAnode++) {
          G4int PMT_index = iSuperLayer*144 + iLine*72 + iPMT*2 + iAnode;
          G4double shift = -324 + 4.5 + 18*iPMT + 9*iAnode;
          G4ThreeVector PMT_position = G4ThreeVector();
          if (alongX == 1) {
            PMT_position = G4ThreeVector(side*(324*mm + 0.5*PMT_sizeZ),
                                         shift*mm,
                                         (4.75 + iLine*9 + iSuperLayer*18.5)*mm);
          } else {
            PMT_position = G4ThreeVector(shift*mm,
                                         side*(324*mm + 0.5*PMT_sizeZ),
                                         (4.75 + iLine*9 + iSuperLayer*18.5)*mm);
          }
          new G4PVPlacement(PMT_rotation,      // rotation
                            PMT_position,      // position
                            PMT_log,           // logical volume
                            "PMT",             // name
                            world_log,         // mother volume
                            false,             // boolean operation
                            PMT_index,         // copy number
                            checkOverlaps);    // overlaps checking
        }
      }
    }
  }

  //--------------------------------------- Return -------------------------------------------------

  return world_phys;
}

void DetectorConstruction::ConstructSDandField()
{
  SensitiveDetector *SD = new SensitiveDetector("SensitiveDetector");
  G4SDManager::GetSDMpointer()->AddNewDetector(SD);
  PMT_log->SetSensitiveDetector(SD);
}
