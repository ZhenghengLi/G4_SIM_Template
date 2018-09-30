#include "OutDataFile_A.hh"

OutDataFile_A::OutDataFile_A() {
    t_file_out_ = NULL;
    t_pol_event_tree_ = NULL;
    t_raw_energy_tree_ = NULL;
}

OutDataFile_A::~OutDataFile_A() {
    close();
}

void OutDataFile_A::clear_data_() {
    // from SCI
    t_pol_event_.event_id                              = -1;
    t_pol_event_.event_time                            = -1;
    t_pol_event_.type                                  = -1;
    t_pol_event_.is_ped                                = false;
    t_pol_event_.packet_num                            = -1;
    t_pol_event_.ct_pre_is_bad                         = false;
    for (int i = 0; i < 25; i++) {
        t_pol_event_.fe_pre_is_bad[i]                  = false;
    }
    t_pol_event_.ct_time_second                        = -1;
    t_pol_event_.ct_time_wait                          = -1;
    t_pol_event_.ct_dead_ratio                         = -1;
    for (int i = 0; i < 25; i++) {
        t_pol_event_.fe_time_second[i]                 = -1;
        t_pol_event_.fe_time_wait[i]                   = -1;
        t_pol_event_.fe_dead_ratio[i]                  = -1;
        t_pol_event_.trig_accepted[i]                  = false;
        t_pol_event_.time_aligned[i]                   = false;
        t_pol_event_.raw_rate[i]                       = -1;
    }
    t_pol_event_.pkt_count                             = -1;
    t_pol_event_.lost_count                            = -1;
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 64; j++) {
            t_pol_event_.trigger_bit[i][j]             = false;
        }
    }
    t_pol_event_.trigger_n                             = -1;
    for (int i = 0; i < 25; i++) {
        t_pol_event_.multiplicity[i]                   = -1;
    }
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 64; j++) {
            t_pol_event_.energy_value[i][j]            = -1;
            t_pol_event_.channel_status[i][j]          = 0;
        }
    }
    for (int i = 0; i < 25; i++) {
        t_pol_event_.common_noise[i]                   = -1;
        t_pol_event_.compress[i]                       = -1;
        t_pol_event_.dy12_too_high[i]                  = false;
        t_pol_event_.t_out_too_many[i]                 = false;
        t_pol_event_.t_out_2[i]                        = false;
        t_pol_event_.t_out_1[i]                        = false;
    }
    // from AUX
    t_pol_event_.aux_interval                          = -123456;
    t_pol_event_.obox_mode                             = -1;
    for (int i = 0; i < 25; i++) {
        t_pol_event_.fe_hv[i]                          = 123456;
        t_pol_event_.fe_thr[i]                         = 123456;
        t_pol_event_.fe_temp[i]                        = 123456;
    }
    // from PPD
    t_pol_event_.ppd_interval                          = -123456;
    for (int i = 0; i < 3; i++) {
        t_pol_event_.wgs84_xyz[i]                      = 123456;
    }
    for (int i = 0; i < 2; i++) {
        t_pol_event_.det_z_radec[i]                    = 123456;
        t_pol_event_.det_x_radec[i]                    = 123456;
        t_pol_event_.earth_radec[i]                    = 123456;
        t_pol_event_.sun_radec[i]                      = 123456;
    }
    // raw_energy from simulation
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 64; j++) {
            t_raw_energy_[i][j] = 0;
        }
    }
}

bool OutDataFile_A::open(const char* filename) {
    if (t_file_out_ != NULL)
        return false;
    t_file_out_ = new TFile(filename, "RECREATE");
    if (t_file_out_->IsZombie())
        return false;
    t_file_out_->cd();
    t_pol_event_tree_ = new TTree("t_pol_event", "Event data");
    // from SCI
    t_pol_event_tree_->Branch("event_id",           &t_pol_event_.event_id,          "event_id/L"                 );
    t_pol_event_tree_->Branch("event_time",         &t_pol_event_.event_time,        "event_time/D"               );
    t_pol_event_tree_->Branch("type",               &t_pol_event_.type,              "type/I"                     );
    t_pol_event_tree_->Branch("is_ped",             &t_pol_event_.is_ped,            "is_ped/O"                   );
    t_pol_event_tree_->Branch("packet_num",         &t_pol_event_.packet_num,        "packet_num/I"               );
    t_pol_event_tree_->Branch("ct_pre_is_bad",      &t_pol_event_.ct_pre_is_bad,     "ct_pre_is_bad/I"            );
    t_pol_event_tree_->Branch("fe_pre_is_bad",       t_pol_event_.fe_pre_is_bad,     "fe_pre_is_bad[25]/I"        );
    t_pol_event_tree_->Branch("ct_time_second",     &t_pol_event_.ct_time_second,    "ct_time_second/D"           );
    t_pol_event_tree_->Branch("ct_time_wait",       &t_pol_event_.ct_time_wait,      "ct_time_wait/D"             );
    t_pol_event_tree_->Branch("ct_dead_ratio",      &t_pol_event_.ct_dead_ratio,     "ct_dead_ratio/F"            );
    t_pol_event_tree_->Branch("fe_time_second",      t_pol_event_.fe_time_second,    "fe_time_second[25]/D"       );
    t_pol_event_tree_->Branch("fe_time_wait",        t_pol_event_.fe_time_wait,      "fe_time_wait[25]/D"         );
    t_pol_event_tree_->Branch("fe_dead_ratio",       t_pol_event_.fe_dead_ratio,     "fe_dead_ratio[25]/F"        );
    t_pol_event_tree_->Branch("trig_accepted",       t_pol_event_.trig_accepted,     "trig_accepted[25]/O"        );
    t_pol_event_tree_->Branch("time_aligned",        t_pol_event_.time_aligned,      "time_aligned[25]/O"         );
    t_pol_event_tree_->Branch("raw_rate",            t_pol_event_.raw_rate,          "raw_rate[25]/I"             );
    t_pol_event_tree_->Branch("pkt_count",          &t_pol_event_.pkt_count,         "pkt_count/I"                );
    t_pol_event_tree_->Branch("lost_count",         &t_pol_event_.lost_count,        "lost_count/I"               );
    t_pol_event_tree_->Branch("trigger_bit",         t_pol_event_.trigger_bit,       "trigger_bit[25][64]/O"      );
    t_pol_event_tree_->Branch("trigger_n",          &t_pol_event_.trigger_n,         "trigger_n/I"                );
    t_pol_event_tree_->Branch("multiplicity",        t_pol_event_.multiplicity,      "multiplicity[25]/I"         );
    t_pol_event_tree_->Branch("energy_value",        t_pol_event_.energy_value,      "energy_value[25][64]/F"     );
    t_pol_event_tree_->Branch("channel_status",      t_pol_event_.channel_status,    "channel_status[25][64]/s"   );
    t_pol_event_tree_->Branch("common_noise",        t_pol_event_.common_noise,      "common_noise[25]/F"         );
    t_pol_event_tree_->Branch("compress",            t_pol_event_.compress,          "compress[25]/I"             );
    t_pol_event_tree_->Branch("dy12_too_high",       t_pol_event_.dy12_too_high,     "dy12_too_high[25]/O"        );
    t_pol_event_tree_->Branch("t_out_too_many",      t_pol_event_.t_out_too_many,    "t_out_too_many[25]/O"       );
    t_pol_event_tree_->Branch("t_out_2",             t_pol_event_.t_out_2,           "t_out_2[25]/O"              );
    t_pol_event_tree_->Branch("t_out_1",             t_pol_event_.t_out_1,           "t_out_1[25]/O"              );
    // from AUX
    t_pol_event_tree_->Branch("aux_interval",       &t_pol_event_.aux_interval,      "aux_interval/F"             );
    t_pol_event_tree_->Branch("obox_mode",          &t_pol_event_.obox_mode,         "obox_mode/I"                );
    t_pol_event_tree_->Branch("fe_hv",               t_pol_event_.fe_hv,             "fe_hv[25]/F"                );
    t_pol_event_tree_->Branch("fe_thr",              t_pol_event_.fe_thr,            "fe_thr[25]/F"               );
    t_pol_event_tree_->Branch("fe_temp",             t_pol_event_.fe_temp,           "fe_temp[25]/F"              );
    // from PPD
    t_pol_event_tree_->Branch("ppd_interval",       &t_pol_event_.ppd_interval,      "ppd_interval/F"             );
    t_pol_event_tree_->Branch("wgs84_xyz",           t_pol_event_.wgs84_xyz,         "wgs84_xyz[3]/D"             );
    t_pol_event_tree_->Branch("det_z_radec",         t_pol_event_.det_z_radec,       "det_z_radec[2]/D"           );
    t_pol_event_tree_->Branch("det_x_radec",         t_pol_event_.det_x_radec,       "det_x_radec[2]/D"           );
    t_pol_event_tree_->Branch("earth_radec",         t_pol_event_.earth_radec,       "earth_radec[2]/D"           );
    t_pol_event_tree_->Branch("sun_radec",           t_pol_event_.sun_radec,         "sun_radec[2]/D"             );

    t_raw_energy_tree_ = new TTree("t_raw_energy", "the real deposited energy from simulation");
    t_raw_energy_tree_->Branch("raw_energy",       t_raw_energy_,                  "raw_energy[25][64]/F"       );

    total_events_ = 0;

    return true;
}

void OutDataFile_A::write_event(const Digi::GeneralEvent_T& general_event) {
    clear_data_();

    // fill data
    t_pol_event_.event_id = general_event.event_id;
    t_pol_event_.type = general_event.type;
    t_pol_event_.is_ped = general_event.is_ped;
    t_pol_event_.packet_num = general_event.event_id;
    t_pol_event_.trigger_n = general_event.trigger_n;
    t_pol_event_.pkt_count = general_event.pkt_count;
    t_pol_event_.lost_count = 0;
    for (int i = 0; i < 25; i++) {
        if (general_event.trig_accepted[i]) {
            t_pol_event_.trig_accepted[i]  = general_event.trig_accepted[i];
            t_pol_event_.time_aligned[i]   = general_event.trig_accepted[i];
            t_pol_event_.multiplicity[i]   = general_event.multiplicity[i];
            t_pol_event_.compress[i]       = general_event.compress[i];
            t_pol_event_.raw_rate[i]       = general_event.raw_rate[i];
            t_pol_event_.t_out_too_many[i] = general_event.t_out_too_many[i];
            t_pol_event_.t_out_2[i]        = general_event.t_out_2[i];
            t_pol_event_.t_out_1[i]        = general_event.t_out_1[i];
            for (int j = 0; j < 64; j++) {
                t_pol_event_.trigger_bit[i][j] = general_event.trigger_bit[i][j];
                t_pol_event_.energy_value[i][j] = general_event.energy_adc[i][j];
            }
        }
        t_pol_event_.fe_hv[i]              = general_event.fe_hv[i];
        t_pol_event_.fe_temp[i]            = general_event.fe_temp[i];
    }
    t_pol_event_tree_->Fill();

    // fill raw energy
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 64; j++) {
            t_raw_energy_[i][j] = general_event.raw_energy[i][j];
        }
    }
    t_raw_energy_tree_->Fill();

    total_events_++;

}

void OutDataFile_A::write_tree() {
    if (t_file_out_ == NULL) {
        return;
    } else {
        t_file_out_->cd();
        t_pol_event_tree_->Write();
        t_raw_energy_tree_->Write();
    }
}

void OutDataFile_A::close() {
    if (t_file_out_ == NULL)
        return;
    delete t_pol_event_tree_;
    t_pol_event_tree_ = NULL;
    delete t_raw_energy_tree_;
    t_raw_energy_tree_ = NULL;
    t_file_out_->Close();
    delete t_file_out_;
    t_file_out_ = NULL;
}

long int OutDataFile_A::get_total_events() {
    return total_events_;
}
