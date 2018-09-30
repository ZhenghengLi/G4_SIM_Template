#include <fstream>
#include <sstream>
#include <cstdlib>
#include "Config.hh"
#include "RootInc.hh"

using namespace std;

Config::Config() {
    clear();
}

Config::~Config() {

}

void Config::clear() {
    ped_temp_filename.clear();
    noise_filename.clear();
    threshold_filename.clear();
    threshold_corr_filename.clear();
    crosstalk_filename.clear();
    gain_hv_filename.clear();
    nonlin_filename.clear();

    pre_c = 0;
    pre_s = 0;
    too_many_mean = 0;
    too_many_width = 0;

    for (int i = 0; i < 25; i++) {
        temp[i] = 100;
        hv[i] = 0;
    }
}

bool Config::parse(const char* config_fn) {
    ifstream config_file;
    config_file.open(config_fn);
    if (!config_file.is_open()) {
        cout << "config file open failed." << endl;
        return false;
    }

    stringstream ss;
    string oneline;
    string key, value;

    while (true) {
        key = "";
        value = "";
        getline(config_file, oneline);
        if (config_file.eof()) break;
        ss.clear();
        ss.str(oneline);
        ss >> key >> value;
        if (key[0] == '#' || value[0] == '#') continue;
        if (key == "" || value == "") continue;

        // calibraion files
        if (key == "Pedestal_File") {
            ped_temp_filename = value;
        } else if (key == "Noise_File") {
            noise_filename = value;
        } else if (key == "Threshold_File") {
            threshold_filename = value;
        } else if (key == "Threshold_Correction_File") {
            threshold_corr_filename = value;
        } else if (key == "Crosstalk_File") {
            crosstalk_filename = value;
        } else if (key == "Gain_HV_File") {
            gain_hv_filename = value;
        } else if (key == "Gain_Temp_File") {
            gain_temp_filename = value;
        } else if (key == "Nonlinearity_File") {
            nonlin_filename = value;
        // setting
        } else if (key == "pre_c") {
            pre_c = atoi(value.c_str());
        } else if (key == "pre_s") {
            pre_s = atoi(value.c_str());
        } else if (key == "too_many_mean") {
            too_many_mean = atof(value.c_str());
        } else if (key == "too_many_width") {
            too_many_width = atof(value.c_str());
        // constants
        } else if (key == "Intrinsic_Res") {
            intrinsic_res = atof(value.c_str());
        } else if (key == "Xtalk_Res") {
            xtalk_res = atof(value.c_str());
        // temperature
        } else if (key == "temp_01") {
            temp[0] = atof(value.c_str());
        } else if (key == "temp_02") {
            temp[1] = atof(value.c_str());
        } else if (key == "temp_03") {
            temp[2] = atof(value.c_str());
        } else if (key == "temp_04") {
            temp[3] = atof(value.c_str());
        } else if (key == "temp_05") {
            temp[4] = atof(value.c_str());
        } else if (key == "temp_06") {
            temp[5] = atof(value.c_str());
        } else if (key == "temp_07") {
            temp[6] = atof(value.c_str());
        } else if (key == "temp_08") {
            temp[7] = atof(value.c_str());
        } else if (key == "temp_09") {
            temp[8] = atof(value.c_str());
        } else if (key == "temp_10") {
            temp[9] = atof(value.c_str());
        } else if (key == "temp_11") {
            temp[10] = atof(value.c_str());
        } else if (key == "temp_12") {
            temp[11] = atof(value.c_str());
        } else if (key == "temp_13") {
            temp[12] = atof(value.c_str());
        } else if (key == "temp_14") {
            temp[13] = atof(value.c_str());
        } else if (key == "temp_15") {
            temp[14] = atof(value.c_str());
        } else if (key == "temp_16") {
            temp[15] = atof(value.c_str());
        } else if (key == "temp_17") {
            temp[16] = atof(value.c_str());
        } else if (key == "temp_18") {
            temp[17] = atof(value.c_str());
        } else if (key == "temp_19") {
            temp[18] = atof(value.c_str());
        } else if (key == "temp_20") {
            temp[19] = atof(value.c_str());
        } else if (key == "temp_21") {
            temp[20] = atof(value.c_str());
        } else if (key == "temp_22") {
            temp[21] = atof(value.c_str());
        } else if (key == "temp_23") {
            temp[22] = atof(value.c_str());
        } else if (key == "temp_24") {
            temp[23] = atof(value.c_str());
        } else if (key == "temp_25") {
            temp[24] = atof(value.c_str());
        // HV
        } else if (key == "HV_01") {
            hv[0] = atof(value.c_str());
        } else if (key == "HV_02") {
            hv[1] = atof(value.c_str());
        } else if (key == "HV_03") {
            hv[2] = atof(value.c_str());
        } else if (key == "HV_04") {
            hv[3] = atof(value.c_str());
        } else if (key == "HV_05") {
            hv[4] = atof(value.c_str());
        } else if (key == "HV_06") {
            hv[5] = atof(value.c_str());
        } else if (key == "HV_07") {
            hv[6] = atof(value.c_str());
        } else if (key == "HV_08") {
            hv[7] = atof(value.c_str());
        } else if (key == "HV_09") {
            hv[8] = atof(value.c_str());
        } else if (key == "HV_10") {
            hv[9] = atof(value.c_str());
        } else if (key == "HV_11") {
            hv[10] = atof(value.c_str());
        } else if (key == "HV_12") {
            hv[11] = atof(value.c_str());
        } else if (key == "HV_13") {
            hv[12] = atof(value.c_str());
        } else if (key == "HV_14") {
            hv[13] = atof(value.c_str());
        } else if (key == "HV_15") {
            hv[14] = atof(value.c_str());
        } else if (key == "HV_16") {
            hv[15] = atof(value.c_str());
        } else if (key == "HV_17") {
            hv[16] = atof(value.c_str());
        } else if (key == "HV_18") {
            hv[17] = atof(value.c_str());
        } else if (key == "HV_19") {
            hv[18] = atof(value.c_str());
        } else if (key == "HV_20") {
            hv[19] = atof(value.c_str());
        } else if (key == "HV_21") {
            hv[20] = atof(value.c_str());
        } else if (key == "HV_22") {
            hv[21] = atof(value.c_str());
        } else if (key == "HV_23") {
            hv[22] = atof(value.c_str());
        } else if (key == "HV_24") {
            hv[23] = atof(value.c_str());
        } else if (key == "HV_25") {
            hv[24] = atof(value.c_str());
        }
    }

    // parameters validation checking
    if (ped_temp_filename == "") return false;
    if (noise_filename == "") return false;
    if (threshold_filename == "") return false;
    if (threshold_corr_filename == "") return false;
    if (crosstalk_filename == "") return false;
    if (gain_hv_filename == "") return false;
    if (gain_temp_filename == "") return false;
    if (nonlin_filename == "") return false;
    if (pre_c < 1) return false;
    if (pre_s < 1) return false;
    if (too_many_mean < 1) return false;
    if (too_many_width <= 0) return false;
    if (intrinsic_res < 0) return false;
    if (xtalk_res < 0) return false;
    for (int i = 0; i < 25; i++) {
        if (temp[i] > 50) return false;
        if (hv[i] < 1) return false;
    }

    return true;
}

void Config::print() {
    cout << "# Calibration Files" << endl;
    cout << "Pedestal_File               " << ped_temp_filename << endl;
    cout << "Noise_File                  " << noise_filename << endl;
    cout << "Threshold_File              " << threshold_filename << endl;
    cout << "Threshold_Correction_File   " << threshold_corr_filename << endl;
    cout << "Crosstalk_File              " << crosstalk_filename << endl;
    cout << "Gain_HV_File                " << gain_hv_filename << endl;
    cout << "Gain_Temp_file              " << gain_temp_filename << endl;
    cout << "Nonlinearity_File           " << nonlin_filename << endl;
    cout << endl;
    cout << "# Setting" << endl;
    cout << "pre_c            " << pre_c << endl;
    cout << "pre_s            " << pre_s << endl;
    cout << "too_many_mean    " << too_many_mean << endl;
    cout << "too_many_width   " << too_many_width << endl;
    cout << endl;
    cout << "# Constants" << endl;
    cout << "Intrinsic_Res    " << intrinsic_res << endl;
    cout << "Xtalk_Res        " << xtalk_res << endl;
    cout << endl;
    cout << "# Temperature" << endl;
    for (int i = 0; i < 25; i++) {
        cout << Form("temp_%02d       ", i + 1) << temp[i] << endl;
    }
    cout << endl;
    cout << "# HV" << endl;
    for (int i = 0; i < 25; i++) {
        cout << Form("HV_%02d         ", i + 1) << hv[i] << endl;
    }

}
