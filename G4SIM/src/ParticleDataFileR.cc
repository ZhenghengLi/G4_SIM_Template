#include "ParticleDataFileR.hh"

using namespace std;

ParticleDataFileR::ParticleDataFileR() {
    t_file_in_ = NULL;
    t_particle_tree_ = NULL;
}

ParticleDataFileR::~ParticleDataFileR() {
    if (t_file_in_ != NULL) close();
}

bool ParticleDataFileR::open(const char* filename) {
    cout << "open particle data file: " << filename << endl;
    if (t_file_in_ != NULL) {
        cout << "file " << filename << " has been opened." << endl;
        return false;
    }
    t_file_in_ = new TFile(filename, "read");
    if (t_file_in_->IsZombie()) {
        cout << "file " << filename << " open failed." << endl;
        return false;
    }
    t_particle_tree_ = static_cast<TTree*>(t_file_in_->Get("t_particle"));
    if (t_particle_tree_ == NULL) {
        cout << "cannot find TTree t_particle" << endl;
        return false;
    }
    t_particle_tree_->SetBranchAddress("ParticleName",      t_particle.ParticleName      );
    t_particle_tree_->SetBranchAddress("Polarization",      t_particle.Polarization      );
    t_particle_tree_->SetBranchAddress("KinEnergy",        &t_particle.KinEnergy         );
    t_particle_tree_->SetBranchAddress("EmitTime",         &t_particle.EmitTime          );
    t_particle_tree_->SetBranchAddress("EmitPosition",      t_particle.EmitPosition      );
    t_particle_tree_->SetBranchAddress("MomDirection",      t_particle.MomDirection      );

    set_start();

    return true;
}

void ParticleDataFileR::close() {
    if (t_file_in_ == NULL)
        return;
    t_file_in_->Close();
    delete t_file_in_;
    t_file_in_ = NULL;
    t_particle_tree_ = NULL;
}

void ParticleDataFileR::set_start() {
    cur_entry_ = -1;
    reach_end_ = false;
}

bool ParticleDataFileR::next() {
    if (t_file_in_ == NULL)
        return false;
    if (reach_end_)
        return false;
    cur_entry_++;
    if (cur_entry_ < t_particle_tree_->GetEntries()) {
        t_particle_tree_->GetEntry(cur_entry_);
    } else {
        reach_end_ = true;
        return false;
    }
    return true;
}

Long64_t ParticleDataFileR::get_id() {
    return cur_entry_;
}
