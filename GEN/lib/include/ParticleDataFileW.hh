#ifndef ParticleDataFileW_hh
#define ParticleDataFileW_hh

#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TNamed.h"

using namespace std;

class ParticleDataFileW {
private:
    TFile* t_file_out_;
    TTree* t_particle_tree_;

public:
    struct Particle_T {
        Char_t    ParticleName[80];
        Double_t  Polarization[3];
        Double_t  KinEnergy;
        Double_t  EmitTime;
        Double_t  EmitPosition[3];
        Double_t  MomDirection[3];
    };

public:
    Particle_T t_particle;

public:
    ParticleDataFileW();
    ~ParticleDataFileW();

    bool open(const char* filename);
    void close();

    void fill_entry();
    void write_tree();
    void write_meta(const char* key, const char* value);

};

#endif
