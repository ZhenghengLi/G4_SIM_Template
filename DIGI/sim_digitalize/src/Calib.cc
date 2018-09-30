#include "Calib.hh"
#include "Config.hh"

Calib::Calib() {
    for (int i = 0; i < 25; i++) {
        ped_mean[i].ResizeTo(64);
        ped_mean_temp[i].ResizeTo(64);
        intrinsic_noise[i].ResizeTo(64);
        vthr_mean[i].ResizeTo(64);
        vthr_mean_corr[i].ResizeTo(64);
        vthr_width[i].ResizeTo(64);
        vthr_width_corr[i].ResizeTo(64);
        xtalk[i].ResizeTo(64, 64);
        ce_vs_hv_p0[i].ResizeTo(64);
        ce_vs_hv_p1[i].ResizeTo(64);
        pedestal_mean[i].ResizeTo(64);
        gain[i].ResizeTo(64);
        photon_xtalk[i].ResizeTo(64, 64);
        gain_true[i].ResizeTo(64);
        nonlin_p0[i].ResizeTo(64);
        nonlin_p1[i].ResizeTo(64);
        nonlin_p2[i].ResizeTo(64);
    }
    common_noise.ResizeTo(25);
    gain_temp_slope.ResizeTo(25);
    reference_temp.ResizeTo(25);
}

Calib::~Calib() {

}

bool Calib::read_calib(const Config& config_obj) {
    // read all the calibration parameters from calibration files

    // pedestal
    TFile* ped_temp_file = new TFile(config_obj.ped_temp_filename.c_str(), "read");
    if (ped_temp_file->IsZombie()) {
        cout << "ped_temp_file open failed: " << config_obj.ped_temp_filename << endl;
        return false;
    }
    TH1F* h1ped_mean[25];
    TH1F* h1ped_mean_temp[25];
    for (int i = 0; i < 25; i++) {
        h1ped_mean[i] = static_cast<TH1F*>(ped_temp_file->Get(Form("h1ped_mean_%02d", i + 1)));
        if (h1ped_mean[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1ped_mean_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                ped_mean[i](j) = h1ped_mean[i]->GetBinContent(j + 1);
            }
        }
        h1ped_mean_temp[i] = static_cast<TH1F*>(ped_temp_file->Get(Form("h1ped_mean_temp_%02d", i + 1)));
        if (h1ped_mean_temp[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1ped_mean_temp_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                ped_mean_temp[i](j) = h1ped_mean_temp[i]->GetBinContent(j + 1);
            }
        }
    }
    ped_temp_file->Close();
    delete ped_temp_file;

    // noise
    TFile* noise_file = new TFile(config_obj.noise_filename.c_str(), "read");
    if (noise_file->IsZombie()) {
        cout << "noise_file open failed: " << config_obj.noise_filename << endl;
        return false;
    }
    TH1F* h1common_noise;
    TH1F* h1intrinsic_noise[25];
    h1common_noise = static_cast<TH1F*>(noise_file->Get("h1common_noise"));
    if (h1common_noise == NULL) {
        cout << "cannot find TH1F h1common_noise" << endl;
        return false;
    } else {
        for (int i = 0; i < 25; i++) {
            common_noise(i) = h1common_noise->GetBinContent(i + 1);
        }
    }
    for (int i = 0; i < 25; i++) {
        h1intrinsic_noise[i] = static_cast<TH1F*>(noise_file->Get(Form("h1intrinsic_noise_%02d", i + 1)));
        if (h1intrinsic_noise[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1intrinsic_noise_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                intrinsic_noise[i](j) = h1intrinsic_noise[i]->GetBinContent(j + 1);
            }
        }
    }
    noise_file->Close();
    delete noise_file;

    // threshold
    TFile* threshold_file = new TFile(config_obj.threshold_filename.c_str(), "read");
    if (threshold_file->IsZombie()) {
        cout << "threshold_file open failed: " << config_obj.threshold_filename << endl;
        return false;
    }
    TH1F* h1vthr_mean[25];
    TH1F* h1vthr_width[25];
    for (int i = 0; i < 25; i++) {
        h1vthr_mean[i] = static_cast<TH1F*>(threshold_file->Get(Form("h1vthr_mean_%02d", i + 1)));
        if (h1vthr_mean[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1vthr_mean_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                vthr_mean[i](j) = h1vthr_mean[i]->GetBinContent(j + 1);
            }
        }
    }
    for (int i = 0; i < 25; i++) {
        h1vthr_width[i] = static_cast<TH1F*>(threshold_file->Get(Form("h1vthr_width_%02d", i + 1)));
        if (h1vthr_width[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1vthr_width_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                vthr_width[i](j) = h1vthr_width[i]->GetBinContent(j + 1);
            }
        }
    }
    threshold_file->Close();
    delete threshold_file;

    // threshold_corr
    TFile* threshold_corr_file = new TFile(config_obj.threshold_corr_filename.c_str(), "read");
    if (threshold_corr_file->IsZombie()) {
        cout << "threshold_corr_file open failed: " << config_obj.threshold_corr_filename << endl;
        return false;
    }
    TH1F* h1vthr_mean_corr[25];
    for (int i = 0; i < 25; i++) {
        h1vthr_mean_corr[i] = static_cast<TH1F*>(threshold_corr_file->Get(Form("h1vthr_mean_corr_%02d", i + 1)));
        if (h1vthr_mean_corr[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1vthr_mean_corr_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                vthr_mean_corr[i](j) = h1vthr_mean_corr[i]->GetBinContent(j + 1);
            }
        }
    }
    TH1F* h1vthr_width_corr[25];
    for (int i = 0; i < 25; i++) {
        h1vthr_width_corr[i] = static_cast<TH1F*>(threshold_corr_file->Get(Form("h1vthr_width_corr_%02d", i + 1)));
        if (h1vthr_width_corr[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1vthr_width_corr_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                vthr_width_corr[i](j) = h1vthr_width_corr[i]->GetBinContent(j + 1);
            }
        }
    }
    threshold_corr_file->Close();
    delete threshold_corr_file;

    // re-calculate the threshold width, because the threshold is measured before common noise subtraction
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 64; j++) {

            // the threshold correction between data and simulation
            // vthr_mean[i](j) += vthr_mean_corr[i](j);
            // vthr_width[i](j) += vthr_width_corr[i](j);

            // vthr_width[i](j) = 1.0;

            // only when the threshold is calulcated using data before common noise subtraction
            // double square_width = vthr_width[i](j) * vthr_width[i](j) - common_noise(i) * common_noise(i);
            // if (square_width < 0) {
            //     vthr_width[i](j) = 1.0;
            // } else {
            //     vthr_width[i](j) = TMath::Sqrt(square_width);
            // }
        }
    }

    // crosstalk
    TFile* crosstalk_file = new TFile(config_obj.crosstalk_filename.c_str(), "read");
    if (crosstalk_file->IsZombie()) {
        cout << "crosstalk_file open failed: " << config_obj.crosstalk_filename << endl;
        return false;
    }
    TH2F* h2xtalk[25];
    for (int i = 0; i < 25; i++) {
        h2xtalk[i] = static_cast<TH2F*>(crosstalk_file->Get(Form("h2xtalk_%02d", i + 1)));
        if (h2xtalk[i] == NULL) {
            cout << "cannot find TH2F " << Form("h2xtalk_%02d", i + 1) << endl;
            return false;
        } else {
            for (int jx = 0; jx < 64; jx++) {
                for (int jy = 0; jy < 64; jy++) {

                    // without any protection
                    xtalk[i](jx, jy) = h2xtalk[i]->GetBinContent(jy + 1, 64 - jx);

                    // with protection, actually this is not needed
                    // if (abs(jx - jy) == 0 || // itself
                    //         abs(jx - jy) == 1  || abs(jx - jy) == 7  || abs(jx - jy) == 8  || abs(jx - jy) == 9  || // first neighbour
                    //         abs(jx - jy) == 2  || abs(jx - jy) == 6  || abs(jx - jy) == 10 || abs(jx - jy) == 14 || // second neighbour
                    //         abs(jx - jy) == 15 || abs(jx - jy) == 16 || abs(jx - jy) == 17 || abs(jx - jy) == 18    // second neighbour
                    //         ) {
                    //     xtalk[i](jx, jy) = h2xtalk[i]->GetBinContent(jy + 1, 64 - jx);
                    // } else {
                    //     // force the crosstalk factor of non-neighbour bars be 0.0 to avoid bad crosstalk factor
                    //     xtalk[i](jx, jy) = 0.0;
                    // }
                }
            }
        }
    }
    crosstalk_file->Close();
    delete crosstalk_file;

    // ce vs. hv
    TFile* gain_hv_file = new TFile(config_obj.gain_hv_filename.c_str(), "read");
    if (gain_hv_file->IsZombie()) {
        cout << "gain_hv_file open failed: " << config_obj.gain_hv_filename << endl;
        return false;
    }
    TH1F* h1ce_hv_p0[25];
    TH1F* h1ce_hv_p1[25];
    for (int i = 0; i < 25; i++) {
        h1ce_hv_p0[i] = static_cast<TH1F*>(gain_hv_file->Get(Form("h1ce_hv_p0_%02d", i + 1)));
        if (h1ce_hv_p0[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1ce_hv_p0_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                ce_vs_hv_p0[i](j) = h1ce_hv_p0[i]->GetBinContent(j + 1);
            }
        }
        h1ce_hv_p1[i] = static_cast<TH1F*>(gain_hv_file->Get(Form("h1ce_hv_p1_%02d", i + 1)));
        if (h1ce_hv_p1[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1ce_hv_p1_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                ce_vs_hv_p1[i](j) = h1ce_hv_p1[i]->GetBinContent(j + 1);
            }
        }
    }
    gain_hv_file->Close();
    delete gain_hv_file;

    // gain temp mean slope in module level
    TFile* gain_temp_file = new TFile(config_obj.gain_temp_filename.c_str(), "read");
    if (gain_temp_file->IsZombie()) {
        cout << "gain_temp_file open failed: " << config_obj.gain_temp_filename << endl;
        return false;
    }
    TH1F* h1gain_temp_slope = static_cast<TH1F*>(gain_temp_file->Get("h1gain_temp_slope"));
    if (h1gain_temp_slope == NULL) {
        cout << "cannot find TH1F h1gain_temp_slope." << endl;
        return false;
    } else {
        for (int i = 0; i < 25; i++) {
            gain_temp_slope(i) = h1gain_temp_slope->GetBinContent(i + 1);
        }
    }
    TH1F* h1reference_temp = static_cast<TH1F*>(gain_temp_file->Get("h1reference_temp"));
    if (h1reference_temp == NULL) {
        cout << "cannot find TH1F h1reference_temp." << endl;
        return false;
    } else {
        for (int i = 0; i < 25; i++) {
            reference_temp(i) = h1reference_temp->GetBinContent(i + 1);
        }
    }
    gain_temp_file->Close();
    delete gain_temp_file;
    gain_temp_file = NULL;

    // nonlinearity
    TFile* nonlin_file = new TFile(config_obj.nonlin_filename.c_str(), "read");
    if (nonlin_file->IsZombie()) {
        cout << "nonlin_file open failed: " << config_obj.nonlin_filename << endl;
        return false;
    }
    TH1F* h1nonlin_p0[25];
    TH1F* h1nonlin_p1[25];
    TH1F* h1nonlin_p2[25];
    for (int i = 0; i < 25; i++) {
        h1nonlin_p0[i] = static_cast<TH1F*>(nonlin_file->Get(Form("h1nonlin_p0_%02d", i + 1)));
        if (h1nonlin_p0[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1nonlin_p0_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                nonlin_p0[i](j) = h1nonlin_p0[i]->GetBinContent(j + 1);
            }
        }
        h1nonlin_p1[i] = static_cast<TH1F*>(nonlin_file->Get(Form("h1nonlin_p1_%02d", i + 1)));
        if (h1nonlin_p1[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1nonlin_p1_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                nonlin_p1[i](j) = h1nonlin_p1[i]->GetBinContent(j + 1);
            }
        }
        h1nonlin_p2[i] = static_cast<TH1F*>(nonlin_file->Get(Form("h1nonlin_p2_%02d", i + 1)));
        if (h1nonlin_p2[i] == NULL) {
            cout << "cannot find TH1F " << Form("h1nonlin_p2_%02d", i + 1) << endl;
            return false;
        } else {
            for (int j = 0; j < 64; j++) {
                nonlin_p2[i](j) = h1nonlin_p2[i]->GetBinContent(j + 1);
            }
        }
    }
    nonlin_file->Close();
    delete nonlin_file;

    return true;
}

void Calib::generate_ped_gain(const Config& config_obj) {
    // generate the pedestal and gain vector according to the temperature and HV

    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 64; j++) {
            // pedestal
            pedestal_mean[i](j) = ped_mean[i](j) + ped_mean_temp[i](j) * (config_obj.temp[i] - 25);
            // gain
            double log10ce = ce_vs_hv_p0[i](j) + ce_vs_hv_p1[i](j) * TMath::Log10(config_obj.hv[i]);
            double temp_offset_ratio = 1.0 + gain_temp_slope(i) * (config_obj.temp[i] - reference_temp(i));
            gain[i](j) = TMath::Power(10, log10ce) / 470.0 * temp_offset_ratio;
        }
    }

}

void Calib::uncouple_xtalk_gain() {
    // uncouple crosstalk and gain
    // input: gain, xtalk
    // output: photon_xtalk, gain_true

    TMatrixD xtalk_tr(64, 64);
    TVectorD gain_inv(64);
    TVectorD gain_true_inv(64);
    for (int i = 0; i < 25; i++) {
        for (int jx = 0; jx < 64; jx++) {
            for (int jy = 0; jy < 64; jy++) {
                xtalk_tr(jx, jy) = xtalk[i](jy, jx);
            }
        }
        xtalk_tr.Invert();
        for (int j = 0; j < 64; j++) {
            gain_inv(j) = 1.0 / gain[i](j);
        }
        gain_true_inv = xtalk_tr * gain_inv;
        for (int j = 0; j < 64; j++) {
            gain_true[i](j) = 1.0 / gain_true_inv(j);
        }
    }
    for (int i = 0; i < 25; i++) {
        for (int jx = 0; jx < 64; jx++) {
            for (int jy = 0; jy < 64; jy++) {
                photon_xtalk[i](jx, jy) = xtalk[i](jx, jy) * gain[i](jy) / gain_true[i](jx);
            }
        }
    }
}
