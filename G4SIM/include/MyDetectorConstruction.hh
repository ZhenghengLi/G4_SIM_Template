#ifndef MyDetectorConstruction_h
#define MyDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4Material;

class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

public:
    G4VPhysicalVolume* Construct();
    void ConstructSDandField();

private: // Materials
    bool materials_defined_;
    G4Material* Vacuum_;
    G4Material* EJ_248_;

private: // Volumes
    G4LogicalVolume* WorldLog_;
    G4LogicalVolume* ScintillatorLog_;
    G4LogicalVolume* ModuleLog_;
    G4LogicalVolume* DetectorLog_;
    G4LogicalVolume* SpaceLabLog_;

private:
    void DefineMaterials_();
    void ConstructModule_();
    void ConstructDetector_();
    void ConstructSpaceLab_();

};

#endif /* MyDetectorConstruction_h */
