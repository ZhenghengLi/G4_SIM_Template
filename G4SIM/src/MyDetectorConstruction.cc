#include "MyDetectorConstruction.hh"

//Unit
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//material
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4NistManager.hh"

//geometry
#include "G4Orb.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Trap.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4Sphere.hh"
#include "G4Para.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include <cstdio>
#include <cmath>

//transformation
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"

//sensitive detector
#include "G4SDManager.hh"
#include "MySensitiveDetector.hh"

//visual attribute
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//global function
#include "globals.hh"

// configure
#include "GlobalConfig.hh"

MyDetectorConstruction::MyDetectorConstruction() {
    WorldLog_          = NULL;
    ScintillatorLog_   = NULL;
    ModuleLog_         = NULL;
    DetectorLog_       = NULL;
    SpaceLabLog_       = NULL;
    materials_defined_ = false;
}

MyDetectorConstruction::~MyDetectorConstruction() {

}

G4VPhysicalVolume* MyDetectorConstruction::Construct() {
    //-----------------------------------------------------------------------------------------------------------
    //--------Define the detector geometry
    //-----------------------------------------------------------------------------------------------------------

    GlobalConfig* fGlobalConfig = GlobalConfig::Instance();

    DefineMaterials_();

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // Construct WorldPhys
    G4double world_hx = (fGlobalConfig->spacelab ? 7.5*m : 500*mm);
    G4double world_hy = (fGlobalConfig->spacelab ? 7.5*m : 500*mm);
    G4double world_hz = (fGlobalConfig->spacelab ? 7.5*m : 500*mm);
    G4Box* WorldBox = new G4Box("WorldBox", world_hx, world_hy, world_hz);
    G4LogicalVolume* WorldLog = new G4LogicalVolume(WorldBox, Vacuum_, "WorldLog");
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    if (!fGlobalConfig->primary_only) {
        // Construct Detector
        ConstructModule_();
        ConstructDetector_();
        new G4PVPlacement(NULL, G4ThreeVector(), DetectorLog_, "Detector", WorldLog, false, 0);
        // Construct SpaceLab
        if (fGlobalConfig->spacelab) {
            G4cout << "Building SpaceLab TG02 ..." << G4endl;
            ConstructSpaceLab_();
            new G4PVPlacement(NULL, G4ThreeVector(), SpaceLabLog_, "SpaceLab", WorldLog, false, 0);
        }
        //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    }

    // Return WorldPhys
    G4VPhysicalVolume* WorldPhys = new G4PVPlacement(NULL, G4ThreeVector(), WorldLog, "World", 0, false, 0);
    return WorldPhys;

}

void MyDetectorConstruction::ConstructSDandField() {
    GlobalConfig* fGlobalConfig = GlobalConfig::Instance();
    if (!fGlobalConfig->primary_only) {
        MySensitiveDetector* My_SD = new MySensitiveDetector("MySD", "MyHitsCollection");
        G4SDManager::GetSDMpointer()->AddNewDetector(My_SD);
        SetSensitiveDetector(ScintillatorLog_, My_SD);
    }
}

void MyDetectorConstruction::DefineMaterials_() {
    //-----------------------------------------------------------------------------------------------------------
    //--------Define Material
    //-----------------------------------------------------------------------------------------------------------

    GlobalConfig* fGlobalConfig = GlobalConfig::Instance();

    G4NistManager* man = G4NistManager::Instance();

    G4double density;                         // matter density
    G4double fractionmass;                    // compound proportion
    G4String name;                            // material name
    G4int ncomponents;                        // component number
    // G4int natoms;                             // atom proportion

    //Vacuum_
    Vacuum_ = man->FindOrBuildMaterial("G4_Galactic");  // for space
    // Vacuum_ = man->FindOrBuildMaterial("G4_AIR");       // for ground

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    G4Element* elC  = man->FindOrBuildElement("C");
    G4Element* elH  = man->FindOrBuildElement("H");

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // EJ_248_
    EJ_248_ = new G4Material(name = "EJ_248", density = 1.023*g/cm3, ncomponents = 2);
    EJ_248_->AddElement(elH, fractionmass = 0.08483);  // 5.18E22 atoms per cm3
    EJ_248_->AddElement(elC, fractionmass = 0.91517);  // 4.69E22 atoms per cm3
    EJ_248_->GetIonisation()->SetBirksConstant(fGlobalConfig->birks_constant*mm/MeV);
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // define other materials that are not sensitive


    materials_defined_ = true;
}

void MyDetectorConstruction::ConstructModule_() {
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // build sensitive detector: ScintillatorLog_
    if (!materials_defined_) return;
    // define ModuleBox
    G4double MBzPlane[6] = {-47.895*mm, -46.0*mm,
                            -46.0*mm, 3.642*mm,
                            3.642*mm, (3.642 + 178.0)*mm};
    G4double MBrOuter[6] = {59.6*mm / 2.0, 59.6*mm / 2,
                            55.5*mm / 2.0, 55.5*mm / 2,
                            52.5*mm / 2.0, 52.5*mm / 2};
    G4double MBrInner[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    G4Polyhedra* ModuleBox = new G4Polyhedra("ModuleBox", 45.0*deg, 360.0*deg, 4, 6, MBzPlane, MBrInner, MBrOuter);
    G4Tubs* ModuleTopHole = new G4Tubs("ModuleTopHole", 0.0*mm, 1.25*mm, 5.0*mm, 0.0*deg, 360.0*deg);
    G4VSolid* ModuleSolid = new G4SubtractionSolid("ModuleSolid",
            ModuleBox, ModuleTopHole, NULL, G4ThreeVector(0.0*mm, 0.0*mm, (3.642 + 178.0)*mm));
    ModuleLog_ = new G4LogicalVolume(ModuleSolid, Vacuum_, "ModuleLog");
    G4VisAttributes* VisAtt_ModuleLog = new G4VisAttributes(true, G4Colour(1.0, 1.0, 0, 1.0));
    VisAtt_ModuleLog->SetForceWireframe(true);
    // VisAtt_ModuleLog->SetForceSolid(true);
    ModuleLog_->SetVisAttributes(VisAtt_ModuleLog);

    // define Scintillator
    G4double SBzPlane[4] = {0.0*mm, 5.0*mm, (5.0 + 166.0)*mm, (5.0 + 166.0 + 5.0)*mm};
    G4double SBrOuter[4] = {5.0*mm / 2.0, 5.8*mm / 2.0, 5.8*mm / 2.0, 5.0*mm / 2.0};
    G4double SBrInner[4] = {0.0, 0.0, 0.0, 0.0};
    G4Polyhedra* ScintillatorBox = new G4Polyhedra("ScintillatorBox", 45.0*deg, 360.0*deg, 4, 4, SBzPlane, SBrInner, SBrOuter);
    ScintillatorLog_ = new G4LogicalVolume(ScintillatorBox, EJ_248_, "ScintillatorLog");
    G4VisAttributes* VisAtt_ScintillatorLog = new G4VisAttributes(true, G4Color(0.0, 0.66, 1.0, 0.2));
    VisAtt_ScintillatorLog->SetForceSolid(true);
    ScintillatorLog_->SetVisAttributes(VisAtt_ScintillatorLog);

    // place 64 Scintillators
    G4double BarD = 6.08*mm;
    const G4double x_start = -(BarD * 3 + BarD / 2);
    const G4double y_start = -(BarD * 3 + BarD / 2);
    for (int j = 0; j < 64; j++) {
        G4double cur_x = x_start + (7 - j % 8) * BarD;
        G4double cur_y = y_start + (j / 8) * BarD;
        new G4PVPlacement(NULL, G4ThreeVector(cur_x, cur_y, 0), ScintillatorLog_, "Scintillator", ModuleLog_, false, j);
    }
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // build nonsensitive components


}

void MyDetectorConstruction::ConstructDetector_() {
    if (ModuleLog_ == NULL) return;
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // define DetectorBox
    G4double DBzPlane[2] = {-74.0*mm, 190.0*mm};
    G4double DBrOuter[2] = {460.0*mm / 2.0, 460.0*mm / 2.0};
    G4double DBrInner[2] = {0.0, 0.0};
    G4Polyhedra* DetectorBox = new G4Polyhedra("DetectorBox", 45.0*deg, 360.0*deg, 4, 2, DBzPlane, DBrInner, DBrOuter);
    DetectorLog_ = new G4LogicalVolume(DetectorBox, Vacuum_, "DetectorLog");
    G4VisAttributes* VisAtt_DetectorLog = new G4VisAttributes(true, G4Colour(1.0, 0.0, 0.0, 1.0));
    VisAtt_DetectorLog->SetForceWireframe(true);
    DetectorLog_->SetVisAttributes(VisAtt_DetectorLog);

    // place 25 Modules
    G4double ModD = 60.0*mm;
    const G4double x_start = -2 * ModD;
    const G4double y_start = -2 * ModD;
    for (int i = 0; i < 25; i++) {
        G4double cur_x = x_start + (i / 5) * ModD;
        G4double cur_y = y_start + (4 - i % 5) * ModD;
        new G4PVPlacement(NULL, G4ThreeVector(cur_x, cur_y, 0), ModuleLog_, "Module", DetectorLog_, false, i);
    }
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // build other components in the detector but outside modules


}

void MyDetectorConstruction::ConstructSpaceLab_() {
    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    // define SpaceLab

    G4Box* SpaceLabBox = new G4Box("SpaceLabBox", 6.5*m, 6.5*m, 6.5*m);
    G4double DSBzPlane[2] = {-74.0*mm, 190.0*mm};
    G4double DSBrOuter[2] = {460.0*mm / 2.0, 460.0*mm / 2.0};
    G4double DSBrInner[2] = {0.0, 0.0};
    G4Polyhedra* DetectorSpaceBox = new G4Polyhedra("DetectorSpaceBox", 45.0*deg, 360.0*deg, 4, 2, DSBzPlane, DSBrInner, DSBrOuter);
    G4VSolid* SpaceLabWorld = new G4SubtractionSolid("SpaceLabSolid", SpaceLabBox, DetectorSpaceBox, NULL, G4ThreeVector());
    SpaceLabLog_ = new G4LogicalVolume(SpaceLabWorld, Vacuum_, "SpaceLabLog");
    G4VisAttributes* VisAtt_SpaceLabWord = new G4VisAttributes(true, G4Colour(0.0, 0.0, 1.0, 1.0));
    VisAtt_SpaceLabWord->SetForceWireframe(true);
    SpaceLabLog_->SetVisAttributes(VisAtt_SpaceLabWord);

}

