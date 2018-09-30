#include "OutDataFile_B.hh"

OutDataFile_B::OutDataFile_B() {
    t_file_out_ = NULL;
    t_modules_tree_ = NULL;
    t_trigger_tree_ = NULL;
}

OutDataFile_B::~OutDataFile_B() {
    close();
}

bool OutDataFile_B::open(const char* filename) {
    if (t_file_out_ != NULL)
        return false;
    t_file_out_ = new TFile(filename, "RECREATE");
    if (t_file_out_->IsZombie())
        return false;
    t_file_out_->cd();

    t_modules_tree_ = new TTree("t_modules","physical modules packets");
    t_modules_tree_->Branch("evnt",             &t_modules_.evnt,            "evnt/L"             );
    t_modules_tree_->Branch("trigg_num",        &t_modules_.trigg_num,       "trigg_num/L"        );
    t_modules_tree_->Branch("trigger_bit",       t_modules_.trigger_bit,     "trigger_bit[64]/O"  );
    t_modules_tree_->Branch("energy_adc",        t_modules_.energy_adc,      "energy_adc[64]/F"   );
    t_modules_tree_->Branch("common_noise",     &t_modules_.common_noise,    "common_noise/F"     );
    t_modules_tree_->Branch("ct_num",           &t_modules_.ct_num,          "ct_num/I"           );
    t_modules_tree_->Branch("multiplicity",     &t_modules_.multiplicity,    "multiplicity/I"     );
    t_modules_tree_->Branch("total_n",          &t_modules_.total_n,         "total_n/I"          );
    t_modules_tree_->Branch("isped",            &t_modules_.isped,           "isped/I"            );
    t_modules_tree_->Branch("hv",               &t_modules_.hv,              "hv/D"               );
    t_modules_tree_->Branch("badch",            &t_modules_.badch,           "badch/I"            );
    t_modules_tree_->Branch("temperature",      &t_modules_.temperature,     "temperature/D"      );
    t_modules_tree_->Branch("status",           &t_modules_.status,          "status/s"           );
    t_modules_tree_->Branch("raw_rate",         &t_modules_.raw_rate,        "raw_rate/I"         );
    t_modules_tree_->Branch("dead_ratio",       &t_modules_.dead_ratio,      "dead_ratio/F"       );
    t_modules_tree_->Branch("count",            &t_modules_.count,           "count/I"            );
    t_modules_tree_->Branch("time_wait",        &t_modules_.time_wait,       "time_wait/F"        );
    t_modules_tree_->Branch("compress",         &t_modules_.compress,         "compress/I"        );
    t_modules_tree_->Branch("is_bad",           &t_modules_.is_bad,           "is_bad/I"          );


    t_trigger_tree_ = new TTree("t_trigger","physical trigger packets");
    t_trigger_tree_->Branch("pkt_start",        &t_trigger_.pkt_start,       "pkt_start/L"        );
    t_trigger_tree_->Branch("pkt_count",        &t_trigger_.pkt_count,       "pkt_count/I"        );
    t_trigger_tree_->Branch("lost_count",       &t_trigger_.lost_count,      "lost_count/I"       );
    t_trigger_tree_->Branch("is_bad",           &t_trigger_.is_bad,          "is_bad/I"           );
    t_trigger_tree_->Branch("time_second",      &t_trigger_.time_second,     "time_second/D"      );
    t_trigger_tree_->Branch("trigg_num",        &t_trigger_.trigg_num,       "trigg_num/L"        );
    t_trigger_tree_->Branch("time_wait",        &t_trigger_.time_wait,       "time_wait/D"        );
    t_trigger_tree_->Branch("tunix",            &t_trigger_.tunix,           "tunix/D"            );
    t_trigger_tree_->Branch("isped",            &t_trigger_.isped,           "isped/I"            );

    trigg_count_ = -1;
    event_count_ = 0;
    time_second_ = 0;
    evnt_count_  = -1;

    total_events_ = 0;

    return true;
}

void OutDataFile_B::write_event(const Digi::GeneralEvent_T& general_event) {
    trigg_count_++;
    t_trigger_.trigg_num = trigg_count_;
    t_trigger_.pkt_start = event_count_;
    t_trigger_.pkt_count = general_event.pkt_count;
    t_trigger_.lost_count = 0;
    t_trigger_.is_bad = 0;
    t_trigger_.time_second = time_second_;
    t_trigger_.tunix = time_second_;
    time_second_ += 0.00075;
    t_trigger_.time_wait = 0.01;
    t_trigger_.isped = general_event.is_ped;
    t_trigger_tree_->Fill();
    for (int i = 0; i < 25; i++) {
        if (general_event.trig_accepted[i]) {
            evnt_count_++;
            t_modules_.evnt = evnt_count_;
            t_modules_.trigg_num = trigg_count_;
            for (int j = 0; j < 64; j++) {
                t_modules_.trigger_bit[j] = general_event.trigger_bit[i][j];
                t_modules_.energy_adc[j]  = general_event.energy_adc[i][j];
                if (general_event.energy_adc[i][j] < 4094.0) {
                    t_modules_.badch = 0;
                } else {
                    t_modules_.badch = 256;
                }
            }
            t_modules_.common_noise = 8192;
            t_modules_.ct_num = i + 1;
            if (general_event.is_ped) {
                t_modules_.multiplicity = 64;
                t_modules_.total_n = 1600;
            } else {
                t_modules_.multiplicity = general_event.multiplicity[i];
                t_modules_.total_n = general_event.trigger_n;
            }
            t_modules_.isped = general_event.is_ped;
            t_modules_.hv = general_event.fe_hv[i];
            t_modules_.temperature = general_event.fe_temp[i];
            t_modules_.status = 14080;
            t_modules_.compress = general_event.compress[i];
            t_modules_.is_bad = 0;
            t_modules_.raw_rate = 1;
            t_modules_.dead_ratio = 0.001;
            t_modules_.count = event_count_;
            t_modules_.time_wait = 0.01;

            t_modules_tree_->Fill();
            event_count_++;
        }
    }

    total_events_++;

}

void OutDataFile_B::write_tree() {
    if (t_file_out_ == NULL) {
        return;
    } else {
        t_file_out_->cd();
        t_trigger_tree_->Write();
        t_modules_tree_->Write();
    }
}

void OutDataFile_B::close() {
    if (t_file_out_ == NULL)
        return;
    delete t_modules_tree_;
    t_modules_tree_ = NULL;
    delete t_trigger_tree_;
    t_trigger_tree_ = NULL;
    t_file_out_->Close();
    delete t_file_out_;
    t_file_out_ = NULL;
}

long int OutDataFile_B::get_total_events() {
    return total_events_;
}
