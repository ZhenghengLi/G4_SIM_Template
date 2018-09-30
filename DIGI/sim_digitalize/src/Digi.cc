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
}

void Digi::energy_response(const vector<EventReader::HitsCol_T>& hitscol_vec, const Calib& calib_obj, const Config& config_obj) {
    // energy -> ADC
    if (hitscol_vec.size() < 1) return;

    general_event.event_id = hitscol_vec[0].EventID;
    general_event.is_ped = false;
}

bool Digi::event_selection(const Config& config_obj) {
    // decide if this event is acceptable
    return true;
}

void Digi::generate_ped_event(const Calib& calib_obj, const Config& config_obj) {
    general_event.is_ped = true;
}

double Digi::get_measured_maxADC_(const double maxADC, const double p0, const double p1, const double p2) {
    return maxADC;
}
