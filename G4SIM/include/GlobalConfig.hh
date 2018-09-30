#ifndef GlobalConfig_HH
#define GlobalConfig_HH

#include "globals.hh"

class ConfigMessenger;

class GlobalConfig {
private:
    ConfigMessenger* fConfigMessenger_;

public:
    GlobalConfig();
    ~GlobalConfig();

public:
    static GlobalConfig* Instance();

private:
    static GlobalConfig* fgInstance_;

public:
    // config parameter list
    G4double hit_threshold;  // lowest energy with unit
    G4double bar_threshold;
    G4String output_directory;
    G4double polarisation_angle;
    G4double incidence_theta;
    G4double incidence_phi;
    G4int    event_verbose;
    G4bool   primary_only;
    G4double birks_constant;
    G4bool   spacelab;
    G4int    phys_verbose;
    G4bool   simout_more;
    G4bool   phys_more;
    G4double antenna_angle_ud;
    G4double antenna_angle_lr;

public:
    void print_config();

};

#endif
