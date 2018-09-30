#ifndef MySensitiveDetector_hh
#define MySensitiveDetector_hh 1

#include "G4VSensitiveDetector.hh"
#include "G4EmSaturation.hh"
#include "MyHit.hh"

class MySensitiveDetector: public G4VSensitiveDetector {
public:
    MySensitiveDetector(G4String SDName, G4String HCName);
    ~MySensitiveDetector();

    void Initialize(G4HCofThisEvent* hitCollection);
    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
    void EndOfEvent(G4HCofThisEvent* hitCollection);

private:
    MyHitsCollection * fMyHitsCollection_;
    G4EmSaturation* fEmSaturation_;

};

#endif
