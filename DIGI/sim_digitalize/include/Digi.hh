#ifndef Digi_HH
#define Digi_HH

#include <iostream>
#include <vector>
#include "RootInc.hh"
#include "EventReader.hh"
#include "Calib.hh"
#include "Config.hh"

class Digi {
private:
    TRandom3 digi_random_;
    Long64_t pre_s_count_;
    Long64_t pre_c_count_;

    Int_t raw_rate_counter_[25];

public:
    // This structure is the output of energy response and selection.
    // Then it can be converted to A structure and B structure.
    struct GeneralEvent_T {
        Long64_t  event_id;
        Float_t   raw_energy[25][64];
        // from energyresponse
        Bool_t    is_ped;
        Bool_t    trigger_bit[25][64];
        Float_t   energy_adc[25][64];
        Int_t     multiplicity[25];
        Int_t     trigger_n;
        Bool_t    trig_accepted[25];
        Int_t     pkt_count;
        Int_t     compress[25];
        Int_t     raw_rate[25];
        // from selection
        Bool_t    t_out_too_many[25];
        Bool_t    t_out_2[25];
        Bool_t    t_out_1[25];
        Int_t     type;
        // from config_obj
        Float_t   fe_hv[25];
        Float_t   fe_temp[25];
    };

public:
    GeneralEvent_T general_event;

private:
    double get_measured_maxADC_(const double maxADC, const double p0, const double p1, const double p2);

public:
    Digi();
    ~Digi();

    void set_seed(int seed = 0);
    void clear_event();
    void energy_response(const vector<EventReader::HitsCol_T>& hitscol_vec, const Calib& calib_obj, const Config& config_obj);
    bool event_selection(const Config& config_obj);
    void generate_ped_event(const Calib& calib_obj, const Config& config_obj);

};


#endif
