#include "ConfigMessenger.hh"
#include "GlobalConfig.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4SystemOfUnits.hh"

ConfigMessenger::ConfigMessenger(GlobalConfig* theGlobalConfig) {
    fGlobalConfig_ = theGlobalConfig;
    // set command
    fGlobalConfigDir_ = new G4UIdirectory("/polar/GlobalConfig/set/");
    fGlobalConfigDir_->SetGuidance("GlobalConfig Control.");

    fGlobalConfigHitThrCmd_ = new G4UIcmdWithADoubleAndUnit(
            "/polar/GlobalConfig/set/hit_threshold",this);
    fGlobalConfigHitThrCmd_->SetParameterName("hit_threshold",false);
    fGlobalConfigHitThrCmd_->SetGuidance("Set hit threshold");
    fGlobalConfigHitThrCmd_->SetUnitCategory("Energy");
//    fGlobalConfigTriThrCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

    fGlobalConfigBarThrCmd_ = new G4UIcmdWithADoubleAndUnit(
            "/polar/GlobalConfig/set/bar_threshold",this);
    fGlobalConfigBarThrCmd_->SetParameterName("bar_threshold",false);
    fGlobalConfigBarThrCmd_->SetGuidance("Set Bar threshold");
    fGlobalConfigBarThrCmd_->SetUnitCategory("Energy");

    fGlobalConfigOutDirCmd_ = new G4UIcmdWithAString(
            "/polar/GlobalConfig/set/output_directory", this);
    fGlobalConfigOutDirCmd_->SetParameterName("output_directory", false);
    fGlobalConfigOutDirCmd_->SetGuidance("Set output directory");

    fGlobalConfigPolAglCmd_ = new G4UIcmdWithADoubleAndUnit(
            "/polar/GlobalConfig/set/polarisation_angle", this);
    fGlobalConfigPolAglCmd_->SetParameterName("polarisation_angle", false);
    fGlobalConfigPolAglCmd_->SetGuidance("Set polarisation angle");
    fGlobalConfigPolAglCmd_->SetUnitCategory("Angle");

    fGlobalConfigIncThetaCmd_ = new G4UIcmdWithADoubleAndUnit(
            "/polar/GlobalConfig/set/incidence_theta", this);
    fGlobalConfigIncThetaCmd_->SetParameterName("incidence_theta", false);
    fGlobalConfigIncThetaCmd_->SetGuidance("Set incidence theta angle");
    fGlobalConfigIncThetaCmd_->SetUnitCategory("Angle");

    fGlobalConfigIncPhiCmd_ = new G4UIcmdWithADoubleAndUnit(
            "/polar/GlobalConfig/set/incidence_phi", this);
    fGlobalConfigIncPhiCmd_->SetParameterName("incidence_phi", false);
    fGlobalConfigIncPhiCmd_->SetGuidance("Set incidence phi angle");
    fGlobalConfigIncPhiCmd_->SetUnitCategory("Angle");

    fGlobalConfigEventVerCmd_ = new G4UIcmdWithAnInteger(
            "/polar/GlobalConfig/set/event_verbose", this);
    fGlobalConfigEventVerCmd_->SetParameterName("event_verbose", false);
    fGlobalConfigEventVerCmd_->SetGuidance("Set event verbose");

    fGlobalConfigPriOnlyCmd_ = new G4UIcmdWithABool(
            "/polar/GlobalConfig/set/primary_only", this);
    fGlobalConfigPriOnlyCmd_->SetParameterName("primary_only", false);
    fGlobalConfigPriOnlyCmd_->SetGuidance("Set primary only");

    fGlobalConfigBirksCmd_ = new G4UIcmdWithADouble(
            "/polar/GlobalConfig/set/birks_constant", this);
    fGlobalConfigBirksCmd_->SetParameterName("birks_constant", false);
    fGlobalConfigBirksCmd_->SetGuidance("Set birks constant");

    fGlobalConfigSpaceLabCmd_ = new G4UIcmdWithABool(
            "/polar/GlobalConfig/set/spacelab", this);
    fGlobalConfigSpaceLabCmd_->SetParameterName("spacelab", false);
    fGlobalConfigSpaceLabCmd_->SetGuidance("Set spacelab");

    fGlobalConfigPhysVerCmd_ = new G4UIcmdWithAnInteger(
            "/polar/GlobalConfig/set/phys_verbose", this);
    fGlobalConfigPhysVerCmd_->SetParameterName("phys_verbose", false);
    fGlobalConfigPhysVerCmd_->SetGuidance("Set phys_verbose");

    fGlobalConfigSimOutMoreCmd_ = new G4UIcmdWithABool(
            "/polar/GlobalConfig/set/simout_more", this);
    fGlobalConfigSimOutMoreCmd_->SetParameterName("simout_more", false);
    fGlobalConfigSimOutMoreCmd_->SetGuidance("Set simout_more");

    fGlobalConfigPhysMoreCmd_ = new G4UIcmdWithABool(
            "/polar/GlobalConfig/set/phys_more", this);
    fGlobalConfigPhysMoreCmd_->SetParameterName("phys_more", false);
    fGlobalConfigPhysMoreCmd_->SetGuidance("Set phys_more");

    fGlobalConfigAntAglUDCmd_ = new G4UIcmdWithADoubleAndUnit(
            "/polar/GlobalConfig/set/antenna_angle_ud", this);
    fGlobalConfigAntAglUDCmd_->SetParameterName("antenna_angle_ud", false);
    fGlobalConfigAntAglUDCmd_->SetGuidance("Set antenna_angle_ud");
    fGlobalConfigAntAglUDCmd_->SetUnitCategory("Angle");

    fGlobalConfigAntAglLRCmd_ = new G4UIcmdWithADoubleAndUnit(
            "/polar/GlobalConfig/set/antenna_angle_lr", this);
    fGlobalConfigAntAglLRCmd_->SetParameterName("antenna_angle_lr", false);
    fGlobalConfigAntAglLRCmd_->SetGuidance("Set antenna_angle_lr");
    fGlobalConfigAntAglLRCmd_->SetUnitCategory("Angle");
}

ConfigMessenger::~ConfigMessenger() {
    delete fGlobalConfigDir_;
    delete fGlobalConfigHitThrCmd_;
    delete fGlobalConfigBarThrCmd_;
    delete fGlobalConfigOutDirCmd_;
    delete fGlobalConfigPolAglCmd_;
    delete fGlobalConfigIncThetaCmd_;
    delete fGlobalConfigIncPhiCmd_;
    delete fGlobalConfigEventVerCmd_;
    delete fGlobalConfigPriOnlyCmd_;
    delete fGlobalConfigBarThrCmd_;
    delete fGlobalConfigSpaceLabCmd_;
    delete fGlobalConfigPhysVerCmd_;
    delete fGlobalConfigSimOutMoreCmd_;
    delete fGlobalConfigPhysMoreCmd_;
    delete fGlobalConfigAntAglUDCmd_;
    delete fGlobalConfigAntAglLRCmd_;
}

void ConfigMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fGlobalConfigHitThrCmd_) {
        fGlobalConfig_->hit_threshold = fGlobalConfigHitThrCmd_->GetNewDoubleValue(newValue);
    } else if (command == fGlobalConfigBarThrCmd_) {
        fGlobalConfig_->bar_threshold = fGlobalConfigBarThrCmd_->GetNewDoubleValue(newValue);
    } else if (command == fGlobalConfigOutDirCmd_) {
        fGlobalConfig_->output_directory = newValue;
    } else if (command == fGlobalConfigPolAglCmd_) {
        fGlobalConfig_->polarisation_angle = fGlobalConfigPolAglCmd_->GetNewDoubleValue(newValue);
    } else if (command == fGlobalConfigIncThetaCmd_) {
        fGlobalConfig_->incidence_theta = fGlobalConfigIncThetaCmd_->GetNewDoubleValue(newValue);
    } else if (command == fGlobalConfigIncPhiCmd_) {
        fGlobalConfig_->incidence_phi = fGlobalConfigIncPhiCmd_->GetNewDoubleValue(newValue);
    } else if (command == fGlobalConfigEventVerCmd_) {
        fGlobalConfig_->event_verbose = fGlobalConfigEventVerCmd_->GetNewIntValue(newValue);
    } else if (command == fGlobalConfigPriOnlyCmd_) {
        fGlobalConfig_->primary_only = fGlobalConfigPriOnlyCmd_->GetNewBoolValue(newValue);
    } else if (command == fGlobalConfigBirksCmd_) {
        fGlobalConfig_->birks_constant = fGlobalConfigBirksCmd_->GetNewDoubleValue(newValue);
    } else if (command == fGlobalConfigSpaceLabCmd_) {
        fGlobalConfig_->spacelab = fGlobalConfigSpaceLabCmd_->GetNewBoolValue(newValue);
    } else if (command == fGlobalConfigPhysVerCmd_) {
        fGlobalConfig_->phys_verbose = fGlobalConfigPhysVerCmd_->GetNewIntValue(newValue);
    } else if (command == fGlobalConfigSimOutMoreCmd_) {
        fGlobalConfig_->simout_more = fGlobalConfigSimOutMoreCmd_->GetNewBoolValue(newValue);
    } else if (command == fGlobalConfigPhysMoreCmd_) {
        fGlobalConfig_->phys_more = fGlobalConfigPhysMoreCmd_->GetNewBoolValue(newValue);
    } else if (command == fGlobalConfigAntAglUDCmd_) {
        fGlobalConfig_->antenna_angle_ud = fGlobalConfigAntAglUDCmd_->GetNewDoubleValue(newValue);
    } else if (command == fGlobalConfigAntAglLRCmd_) {
        fGlobalConfig_->antenna_angle_lr = fGlobalConfigAntAglLRCmd_->GetNewDoubleValue(newValue);
    }
}
