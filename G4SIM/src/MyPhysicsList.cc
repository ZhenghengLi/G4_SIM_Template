#include "MyPhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4IonQMDPhysics.hh"
#include "G4IonElasticPhysics.hh"

#include "G4DataQuestionaire.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"

#include "GlobalConfig.hh"

MyPhysicsList::MyPhysicsList(G4int ver) {

    GlobalConfig* fGlobalConfig = GlobalConfig::Instance();

    G4DataQuestionaire it(photon, neutron, radioactive);

    // Default Cut Value  (1.0 mm is the Geant4 default)
    defaultCutValue = 0.1*mm;

    // Verbose Level
    SetVerboseLevel(ver);

    // EM Physics
    RegisterPhysics(new G4EmLivermorePolarizedPhysics(ver));

    if (fGlobalConfig->phys_more) {
        // Synchroton Radiation & GN Physics
        RegisterPhysics(new G4EmExtraPhysics(ver));

        // Decays
        RegisterPhysics(new G4DecayPhysics(ver));

        //G4RadioactiveDecayPhysics
        RegisterPhysics(new G4RadioactiveDecayPhysics(ver));

        // Hadron Elastic scattering
        RegisterPhysics(new G4HadronElasticPhysics(ver));

        // Hadron Physics
        RegisterPhysics(new G4HadronPhysicsQGSP_BERT(ver));

        // Stopping Physics
        RegisterPhysics(new G4StoppingPhysics(ver));

        // Ion Physics
        RegisterPhysics( new G4IonQMDPhysics(ver));
        RegisterPhysics( new G4IonElasticPhysics(ver));

        // Neutron
        // RegisterPhysics( new G4NeutronTrackingCut(ver));
    }

}

MyPhysicsList::~MyPhysicsList() {

}

void MyPhysicsList::SetCuts() {

    SetCutsWithDefault();
    //Set proton cut value to 0 for producing low energy recoil nucleus
    SetCutValue(0, "proton");

}

