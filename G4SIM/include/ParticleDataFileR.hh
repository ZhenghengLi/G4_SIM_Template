#ifndef PARTICLEDATAFILE_HH
#define PARTICLEDATAFILE_HH

#include <iostream>
#include "TFile.h"
#include "TTree.h"

using namespace std;

class ParticleDataFileR {
private:
    TFile* t_file_in_;
    TTree* t_particle_tree_;

    Long64_t cur_entry_;
    bool     reach_end_;

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
    ParticleDataFileR();
    ~ParticleDataFileR();

    bool open(const char* filename);
    void close();

    void     set_start();
    bool     next();
    Long64_t get_id();

};

#endif
