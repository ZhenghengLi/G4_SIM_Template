#ifndef MyActionInitialization_h
#define MyActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"
#include "ParticleDataFileR.hh"

class MyActionInitialization: public G4VUserActionInitialization {
private:
    ParticleDataFileR*  fParticleDataFileR_;
    G4bool   gps_flag_;
    G4String output_file_;
    G4bool   fixed_filename_;

public:
    MyActionInitialization(G4bool the_gps_flag = true, G4String the_output_file = "output.root", G4bool fixed_name = false);
    ~MyActionInitialization();

    ParticleDataFileR* GetParticleDataFileR();

public:
    void Build() const;
    void BuildForMaster() const;
};

#endif /* MyActionInitialization_h */
