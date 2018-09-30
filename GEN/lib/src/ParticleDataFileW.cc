#include "ParticleDataFileW.hh"

using namespace std;

ParticleDataFileW::ParticleDataFileW() {
    t_file_out_ = NULL;
    t_particle_tree_ = NULL;
}

ParticleDataFileW::~ParticleDataFileW() {
    if (t_file_out_ != NULL)
        close();
}

bool ParticleDataFileW::open(const char* filename) {
    if (t_file_out_ != NULL)
        return false;
    t_file_out_ = new TFile(filename, "recreate");
    if (t_file_out_->IsZombie())
        return false;
    t_particle_tree_ = new TTree("t_particle", "Primary Particle Data");
    t_particle_tree_->Branch("ParticleName",   t_particle.ParticleName,    "ParticleName/C"    );
    t_particle_tree_->Branch("Polarization",   t_particle.Polarization,    "Polarization[3]/D" );
    t_particle_tree_->Branch("KinEnergy",     &t_particle.KinEnergy,       "KinEnergy/D"       );
    t_particle_tree_->Branch("EmitTime",      &t_particle.EmitTime,        "EmitTime/D"        );
    t_particle_tree_->Branch("EmitPosition",   t_particle.EmitPosition,    "EmitPosition[3]/D" );
    t_particle_tree_->Branch("MomDirection",   t_particle.MomDirection,    "MomDirection[3]/D" );

    return true;
}

void ParticleDataFileW::close() {
    if (t_file_out_ == NULL)
        return;
    delete t_particle_tree_;
    t_particle_tree_ = NULL;
    t_file_out_->Close();
    delete t_file_out_;
    t_file_out_ = NULL;
}

void ParticleDataFileW::fill_entry() {
    if (t_file_out_ == NULL)
        return;
    t_particle_tree_->Fill();
}

void ParticleDataFileW::write_tree() {
    if (t_file_out_ == NULL)
        return;
    t_file_out_->cd();
    t_particle_tree_->Write();
}

void ParticleDataFileW::write_meta(const char* key, const char* value) {
    if (t_file_out_ == NULL)
        return;
    t_file_out_->cd();
    TNamed(key, value).Write();
}

