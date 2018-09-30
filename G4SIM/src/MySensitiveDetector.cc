#include "MySensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"
#include "G4StepPoint.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "GlobalConfig.hh"
#include "G4Version.hh"

MySensitiveDetector::MySensitiveDetector(G4String SDName, G4String HCName): G4VSensitiveDetector(SDName) {
    collectionName.insert(HCName);
    fMyHitsCollection_ = NULL;
    fEmSaturation_ = new G4EmSaturation(0);
}

MySensitiveDetector::~MySensitiveDetector() {
    delete fEmSaturation_;
    fEmSaturation_ = NULL;
}

void MySensitiveDetector::Initialize(G4HCofThisEvent* hitCollection) {
    // at the start of an event
    fMyHitsCollection_ = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);
    G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hitCollection->AddHitsCollection(HCID, fMyHitsCollection_);
}

G4bool MySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ) {
    // cut hit threshold
    GlobalConfig* fGlobalConfig = GlobalConfig::Instance();
    if (aStep->GetTotalEnergyDeposit() < fGlobalConfig->hit_threshold)
        return false;
    // track info
    G4Track* theTrack     = aStep->GetTrack();
    G4int    trackID      = theTrack->GetTrackID();
    G4String particleName = theTrack->GetDefinition()->GetParticleName();
    G4int    particleCode = theTrack->GetDefinition()->GetPDGEncoding();

    // point info
    G4StepPoint*      preStepPoint      = aStep->GetPreStepPoint();
    G4StepPoint*      postStepPoint     = aStep->GetPostStepPoint();
    G4double          globalTime        = preStepPoint->GetGlobalTime();
    G4TouchableHandle preTouchable      = preStepPoint->GetTouchableHandle();
    G4TouchableHandle postTouchable     = postStepPoint->GetTouchableHandle();
    G4int             barID             = preTouchable->GetCopyNumber(0);
    G4int             modID             = preTouchable->GetCopyNumber(1);
    G4ThreeVector     postWorldPosition = postStepPoint->GetPosition();
    G4ThreeVector     postLocalPosition = postTouchable->GetHistory()->GetTopTransform().TransformPoint(postWorldPosition);
    G4bool            isEntering        = (preStepPoint->GetStepStatus() == fGeomBoundary);
    G4bool            isLeaving         = (postStepPoint->GetStepStatus() == fGeomBoundary);
    G4ThreeVector     preMomDirection   = preStepPoint->GetMomentumDirection();
    G4double          preMomTheta       = preMomDirection.theta();
    G4double          preMomPhi         = preMomDirection.phi();
    if (preMomPhi < 0)
        preMomPhi += twopi;
    G4ThreeVector     postMomDirection  = postStepPoint->GetMomentumDirection();
    G4double          postMomTheta      = postMomDirection.theta();
    G4double          postMomPhi        = postMomDirection.phi();
    if (postMomPhi < 0)
        postMomPhi += twopi;
    G4double          preKinEnergy      = preStepPoint->GetKineticEnergy();
    G4double          postKinEnergy     = postStepPoint->GetKineticEnergy();
    G4String          processName       = postStepPoint->GetProcessDefinedStep()->GetProcessName();
    G4double          energyDep         = aStep->GetTotalEnergyDeposit();
#if G4VERSION_NUMBER < 1022   // this number is not sure, change it as the current version of Geant4 you are using if compilation error occurred
    G4double          energyVis         = fEmSaturation_->VisibleEnergyDeposition(aStep);
#else
    G4double          energyVis         = fEmSaturation_->VisibleEnergyDepositionAtAStep(aStep);
#endif
    G4double          deltaTime         = aStep->GetDeltaTime();

    // generate a hit
    MyHit* aHit = new MyHit();
    if (aHit == NULL) {
        G4ExceptionDescription msg;
        msg << "Cannot access hit " << G4endl;
        G4Exception("MySensitiveDetector::ProcessHits()", "MyCode0003", FatalException, msg);
    }
    // store hit information
    aHit->TrackID        = trackID;
    aHit->ParticleName   = particleName;
    aHit->ParticleCode   = particleCode;
    aHit->GlobalTime     = globalTime;
    aHit->BarID          = barID;
    aHit->ModID          = modID;
    aHit->LocalPosition  = postLocalPosition;
    aHit->IsEntering     = isEntering;
    aHit->IsLeaving      = isLeaving;
    aHit->PreMomTheta    = preMomTheta;
    aHit->PreMomPhi      = preMomPhi;
    aHit->PostMomTheta   = postMomTheta;
    aHit->PostMomPhi     = postMomPhi;
    aHit->PreKinEnergy   = preKinEnergy;
    aHit->PostKinEnergy  = postKinEnergy;
    aHit->ProcessName    = processName;
    aHit->EnergyDep      = energyDep;
    aHit->EnergyVis      = energyVis;
    aHit->DeltaTime      = deltaTime;

    fMyHitsCollection_->insert(aHit);

    return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent* ) {
    // at the stop of an event
}
