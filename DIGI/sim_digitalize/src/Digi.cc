#include "Digi.hh"
#include "Constants.hh"
#include <iomanip>
#include <cmath>

Digi::Digi() {
    set_seed(0);
    clear_event();

    pre_s_count_ = 0;
    pre_c_count_ = 0;

    for (int i = 0; i < 25; i++) {
        raw_rate_counter_[i] = 0;
    }
}

Digi::~Digi() {

}

void Digi::set_seed(int seed) {
    digi_random_.SetSeed(seed);
}

void Digi::clear_event() {
    general_event.event_id = 0;
    general_event.is_ped = false;
    general_event.type = 0;
    general_event.trigger_n = 0;
    general_event.pkt_count = 0;
    for (int i = 0; i < 25; i++) {
        general_event.trig_accepted[i] = false;
        general_event.multiplicity[i] = 0;
        general_event.compress[i] = -1;
        general_event.raw_rate[i] = -1;
        general_event.t_out_too_many[i] = false;
        general_event.t_out_1[i] = false;
        general_event.t_out_2[i] = false;
        general_event.fe_hv[i] = 0;
        general_event.fe_temp[i] = 100;
        for (int j = 0; j < 64; j++) {
            general_event.trigger_bit[i][j] = false;
            general_event.energy_adc[i][j] = -1;
            general_event.raw_energy[i][j] = 0;
        }
    }
}

void Digi::energy_response(const vector<EventReader::HitsCol_T>& hitscol_vec, const Calib& calib_obj, const Config& config_obj) {
    // energy -> ADC
    if (hitscol_vec.size() < 1) return;

    general_event.event_id = hitscol_vec[0].EventID;
    general_event.is_ped = false;

    double bar_optical_photon[25][64];
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 64; j++) {
            general_event.raw_energy[i][j] = 0;
            bar_optical_photon[i][j] = 0;
        }
    }
    for (size_t i = 0; i < hitscol_vec.size(); i++) {
        int mod_id = hitscol_vec[i].ModID;
        int bar_id = hitscol_vec[i].BarID;
        double energy = hitscol_vec[i].EnergyVis;
        // ....
        general_event.raw_energy[mod_id][bar_id] += energy;
        bar_optical_photon[mod_id][bar_id] += energy;
    }

    // ...
    // digitization process
    // ...

    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 64; j++) {
            general_event.energy_adc[i][j] = bar_optical_photon[i][j];
        }
    }
}

bool Digi::event_selection(const Config& config_obj) {
    // decide if this event is acceptable
    return true;
}

void Digi::generate_ped_event(const Calib& calib_obj, const Config& config_obj) {
    general_event.is_ped = true;
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 64; j++) {
            general_event.energy_adc[i][j] = digi_random_.Gaus(0, 25);
        }
    }
}

double Digi::get_measured_maxADC_(const double maxADC, const double p0, const double p1, const double p2) {
    return maxADC;
}
