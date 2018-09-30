#include "MyPrimaryGeneratorAction.hh"
#include "GlobalConfig.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "ParticleDataFileR.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction(
        G4bool the_gps_flag,
        ParticleDataFileR* theParticleDataFileR) {

    gpsFlag_            = the_gps_flag;
    fParticleDataFileR_ = theParticleDataFileR;
    fParticleTable_     = G4ParticleTable::GetParticleTable();

    fParticleSource_ = new G4GeneralParticleSource();
    fParticleGun_    = new G4ParticleGun();
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction() {
    delete fParticleSource_;
    delete fParticleGun_;
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    GlobalConfig* fGlobalConfig = GlobalConfig::Instance();
    if (gpsFlag_) {
        fParticleSource_->GeneratePrimaryVertex(anEvent);
    } else {
        // read paticle data from root file
        if (fParticleDataFileR_ == NULL) {
            return;
        }
        if (fParticleDataFileR_->next()) {
            anEvent->SetEventID(fParticleDataFileR_->get_id());

            G4ParticleDefinition* fParticleDefinition = fParticleTable_->FindParticle(fParticleDataFileR_->t_particle.ParticleName);
            if (fParticleDefinition == NULL) {
                G4ExceptionDescription msg;
                msg << "Cannot find particle: " << fParticleDataFileR_->t_particle.ParticleName;
                G4Exception("MyPrimaryGeneratorAction::GeneratePrimaries", "MyCode0004", FatalException, msg);
            }
            fParticleGun_->SetParticleDefinition(fParticleDefinition);

            PolarizationVec_.setX(fParticleDataFileR_->t_particle.Polarization[0]);
            PolarizationVec_.setY(fParticleDataFileR_->t_particle.Polarization[1]);
            PolarizationVec_.setZ(fParticleDataFileR_->t_particle.Polarization[2]);
            PolarizationVec_.rotateZ(fGlobalConfig->polarisation_angle);
            PolarizationVec_.rotateY(fGlobalConfig->incidence_theta);
            PolarizationVec_.rotateZ(fGlobalConfig->incidence_phi);
            fParticleGun_->SetParticlePolarization(PolarizationVec_);

            EmitPositionVec_.setX(fParticleDataFileR_->t_particle.EmitPosition[0] * cm);
            EmitPositionVec_.setY(fParticleDataFileR_->t_particle.EmitPosition[1] * cm);
            EmitPositionVec_.setZ(fParticleDataFileR_->t_particle.EmitPosition[2] * cm);
            EmitPositionVec_.rotateY(fGlobalConfig->incidence_theta);
            EmitPositionVec_.rotateZ(fGlobalConfig->incidence_phi);
            fParticleGun_->SetParticlePosition(EmitPositionVec_);

            MomDirectionVec_.setX(fParticleDataFileR_->t_particle.MomDirection[0]);
            MomDirectionVec_.setY(fParticleDataFileR_->t_particle.MomDirection[1]);
            MomDirectionVec_.setZ(fParticleDataFileR_->t_particle.MomDirection[2]);
            MomDirectionVec_.rotateY(fGlobalConfig->incidence_theta);
            MomDirectionVec_.rotateZ(fGlobalConfig->incidence_phi);
            fParticleGun_->SetParticleMomentumDirection(MomDirectionVec_);

            fParticleGun_->SetParticleEnergy(fParticleDataFileR_->t_particle.KinEnergy * keV);
            fParticleGun_->SetParticleTime(fParticleDataFileR_->t_particle.EmitTime * second);

            fParticleGun_->GeneratePrimaryVertex(anEvent);
        }
    }
}
