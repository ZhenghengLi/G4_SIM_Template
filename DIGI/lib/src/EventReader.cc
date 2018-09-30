#include "EventReader.hh"

using namespace std;

EventReader::EventReader() {
    t_file_in_ = NULL;
    t_keyword_tree_ = NULL;
    t_primary_tree_ = NULL;
    t_hitscol_tree_ = NULL;
    simout_more_ = false;
}

EventReader::~EventReader() {
    if (t_file_in_ != NULL) close();
}

bool EventReader::open(const char* filename) {
    if (t_file_in_ != NULL) return false;
    t_file_in_ = new TFile(filename, "read");
    if (t_file_in_->IsZombie()) return false;

    t_keyword_tree_ = static_cast<TTree*>(t_file_in_->Get("Keyword"));
    if (t_keyword_tree_ != NULL) {
        t_keyword_tree_->SetBranchAddress("RunID",            &Keyword.RunID            );
        t_keyword_tree_->SetBranchAddress("NumberOfEvents",   &Keyword.NumberOfEvents   );
        t_keyword_tree_->SetBranchAddress("TotalHits",        &Keyword.TotalHits        );
        t_keyword_tree_->SetBranchAddress("TotalEvents",      &Keyword.TotalEvents      );
    }

    t_primary_tree_ = static_cast<TTree*>(t_file_in_->Get("Primary"));
    if (t_primary_tree_ == NULL) return false;
    t_hitscol_tree_ = static_cast<TTree*>(t_file_in_->Get("HitsCol"));
    if (t_hitscol_tree_ == NULL) return false;

    simout_more_ = false;
    if (t_primary_tree_->GetNbranches() > 2 && t_hitscol_tree_->GetNbranches() > 9) {
        simout_more_ = true;
    }

    // necessary
    t_primary_tree_->SetBranchAddress("EventID",           &Primary.EventID          );
    t_primary_tree_->SetBranchAddress("HitsCount",         &Primary.HitsCount        );
    // optional
    if (simout_more_) {
        t_primary_tree_->SetBranchAddress("PName_Primary",      Primary.PName_Primary    );
        t_primary_tree_->SetBranchAddress("X_Primary",         &Primary.X_Primary        );
        t_primary_tree_->SetBranchAddress("Y_Primary",         &Primary.Y_Primary        );
        t_primary_tree_->SetBranchAddress("Z_Primary",         &Primary.Z_Primary        );
        t_primary_tree_->SetBranchAddress("Time_Primary",      &Primary.Time_Primary     );
        t_primary_tree_->SetBranchAddress("Energy_Primary",    &Primary.Energy_Primary   );
        t_primary_tree_->SetBranchAddress("POLX_Primary",      &Primary.POLX_Primary     );
        t_primary_tree_->SetBranchAddress("POLY_Primary",      &Primary.POLY_Primary     );
        t_primary_tree_->SetBranchAddress("POLZ_Primary",      &Primary.POLZ_Primary     );
        t_primary_tree_->SetBranchAddress("Theta_Primary",     &Primary.Theta_Primary    );
        t_primary_tree_->SetBranchAddress("Phi_Primary",       &Primary.Phi_Primary      );
    }

    // necessary
    t_hitscol_tree_->SetBranchAddress("EventID",           &cur_hit_.EventID         );
    t_hitscol_tree_->SetBranchAddress("TrackID",           &cur_hit_.TrackID         );
    t_hitscol_tree_->SetBranchAddress("ParticleCode",      &cur_hit_.ParticleCode    );
    t_hitscol_tree_->SetBranchAddress("BarID",             &cur_hit_.BarID           );
    t_hitscol_tree_->SetBranchAddress("ModID",             &cur_hit_.ModID           );
    t_hitscol_tree_->SetBranchAddress("LocPosX",           &cur_hit_.LocPosX         );
    t_hitscol_tree_->SetBranchAddress("LocPosY",           &cur_hit_.LocPosY         );
    t_hitscol_tree_->SetBranchAddress("LocPosZ",           &cur_hit_.LocPosZ         );
    t_hitscol_tree_->SetBranchAddress("EnergyVis",         &cur_hit_.EnergyVis       );
    t_hitscol_tree_->SetBranchAddress("EnergyDep",         &cur_hit_.EnergyDep       );
    // optional
    if (simout_more_) {
        t_hitscol_tree_->SetBranchAddress("ParticleName",       cur_hit_.ParticleName    );
        t_hitscol_tree_->SetBranchAddress("PreGlobalTime",     &cur_hit_.PreGlobalTime   );
        t_hitscol_tree_->SetBranchAddress("StepStatus",        &cur_hit_.StepStatus      );
        t_hitscol_tree_->SetBranchAddress("PreMomTheta",       &cur_hit_.PreMomTheta     );
        t_hitscol_tree_->SetBranchAddress("PreMomPhi",         &cur_hit_.PreMomPhi       );
        t_hitscol_tree_->SetBranchAddress("PostMomTheta",      &cur_hit_.PostMomTheta    );
        t_hitscol_tree_->SetBranchAddress("PostMomPhi",        &cur_hit_.PostMomPhi      );
        t_hitscol_tree_->SetBranchAddress("PreKinEnergy",      &cur_hit_.PreKinEnergy    );
        t_hitscol_tree_->SetBranchAddress("PostKinEnergy",     &cur_hit_.PostKinEnergy   );
        t_hitscol_tree_->SetBranchAddress("ProcessName",        cur_hit_.ProcessName     );
        t_hitscol_tree_->SetBranchAddress("DeltaTime",         &cur_hit_.DeltaTime       );
    }

    keyword_start();
    primary_start();
    event_start();

    return true;
}

void EventReader::close() {
    if (t_file_in_ == NULL) return;
    t_file_in_->Close();
    delete t_file_in_;
    t_file_in_ = NULL;
    t_keyword_tree_ = NULL;
    t_primary_tree_ = NULL;
    t_hitscol_tree_ = NULL;
}

void EventReader::keyword_start() {
    keyword_cur_entry_ = -1;
    keyword_reach_end_ = false;
}

bool EventReader::keyword_next() {
    if (t_keyword_tree_ == NULL) return false;
    if (keyword_reach_end_) return false;
    keyword_cur_entry_++;
    if (keyword_cur_entry_ < t_keyword_tree_->GetEntries()) {
        t_keyword_tree_->GetEntry(keyword_cur_entry_);
        return true;
    } else {
        keyword_reach_end_ = true;
        return false;
    }
}

void EventReader::primary_start() {
    primary_cur_entry_ = -1;
    primary_reach_end_ = false;
}

bool EventReader::primary_next() {
    if (t_primary_tree_ == NULL) return false;
    if (primary_reach_end_) return false;
    primary_cur_entry_++;
    if (primary_cur_entry_ < t_primary_tree_->GetEntries()) {
        t_primary_tree_->GetEntry(primary_cur_entry_);
        return true;
    } else {
        primary_reach_end_ = true;
        return false;
    }
}

void EventReader::event_start() {
    if (t_hitscol_tree_ == NULL) return;
    hitscol_cur_entry_ = 0;
    hitscol_reach_end_ = false;
    if (hitscol_cur_entry_ < t_hitscol_tree_->GetEntries()) {
        t_hitscol_tree_->GetEntry(hitscol_cur_entry_);
    } else {
        hitscol_reach_end_ = true;
    }
}

bool EventReader::event_next() {
    if (t_hitscol_tree_ == NULL) return false;
    if (hitscol_reach_end_) return false;
    HitsCol.clear();
    HitsCol.push_back(cur_hit_);
    while (true) {
        hitscol_cur_entry_++;
        if (hitscol_cur_entry_ < t_hitscol_tree_->GetEntries()) {
            t_hitscol_tree_->GetEntry(hitscol_cur_entry_);
            if (HitsCol.front().EventID == cur_hit_.EventID) {
                HitsCol.push_back(cur_hit_);
            } else {
                break;
            }
        } else {
            hitscol_reach_end_ = true;
            break;
        }
    }
    return true;
}

Long64_t EventReader::keyword_get_tot_entries() {
    if (t_keyword_tree_ == NULL)
        return 0;
    else
        return t_keyword_tree_->GetEntries();
}

Long64_t EventReader::keyword_get_cur_entry() {
    if (t_keyword_tree_ == NULL)
        return 0;
    else
        return keyword_cur_entry_;
}

Long64_t EventReader::primary_get_tot_entries() {
    if (t_primary_tree_ == NULL)
        return 0;
    else
        return t_primary_tree_->GetEntries();
}

Long64_t EventReader::primary_get_cur_entry() {
    if (t_primary_tree_ == NULL)
        return 0;
    else
        return primary_cur_entry_;
}

Long64_t EventReader::hitscol_get_tot_entries() {
    if (t_hitscol_tree_ == NULL)
        return 0;
    else
        return t_hitscol_tree_->GetEntries();
}

Long64_t EventReader::hitscol_get_cur_entry() {
    if (t_hitscol_tree_ == NULL)
        return 0;
    else
        return hitscol_cur_entry_;
}

void EventReader::primary_deactive_all() {
    t_primary_tree_->SetBranchStatus("*", false);
}

void EventReader::primary_active(const char* branch) {
    t_primary_tree_->SetBranchStatus(branch, true);
}

void EventReader::hitscol_deactive_all() {
    t_hitscol_tree_->SetBranchStatus("*", false);
}

void EventReader::hitscol_active(const char* branch) {
    t_hitscol_tree_->SetBranchStatus(branch, true);
}

bool EventReader::get_simout_more() {
    return simout_more_;
}
