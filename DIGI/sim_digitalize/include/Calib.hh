#ifndef Calib_HH
#define Calib_HH

#include <iostream>
#include "RootInc.hh"

using namespace std;

class Config;

class Calib {
public:
    // pedestal
    TVectorF ped_mean[25];         // pedestal at 25 degree
    TVectorF ped_mean_temp[25];    // slope of the linear relaionship between pedestal and temperature
    // noise
    TVectorF common_noise;
    TVectorF intrinsic_noise[25];
    // threshold
    TVectorF vthr_mean[25];
    TVectorF vthr_mean_corr[25];
    TVectorF vthr_width[25];
    TVectorF vthr_width_corr[25];
    // crosstalk
    TMatrixF xtalk[25];
    // ce vs. hv, compton edge at 470keV
    TVectorF ce_vs_hv_p0[25];
    TVectorF ce_vs_hv_p1[25];
    // gain temp mean slope in module level
    TVectorF gain_temp_slope;
    TVectorF reference_temp;

public:
    TVectorF pedestal_mean[25];
    TVectorF gain[25];           // ADC/keV
    TMatrixF photon_xtalk[25];   // the uncoupled photon xtalk
    TVectorF gain_true[25];      // the uncoupled gain
    TVectorF nonlin_p0[25];
    TVectorF nonlin_p1[25];
    TVectorF nonlin_p2[25];

public:
    Calib();
    ~Calib();

    bool read_calib(const Config& config_obj);
    void generate_ped_gain(const Config& config_obj);
    void uncouple_xtalk_gain();

};

#endif
