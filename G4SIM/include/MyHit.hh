#ifndef MyHit_hh
#define MyHit_hh 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "tls.hh"
#include "G4ThreeVector.hh"

class MyHit: public G4VHit {
public:
    MyHit();
    ~MyHit();

    inline void* operator new(size_t);
    inline void  operator delete(void* aHit);

public:
    // hit information
    G4int             TrackID;
    G4String          ParticleName;
    G4int             ParticleCode;
    G4double          GlobalTime;
    G4int             BarID;
    G4int             ModID;
    G4ThreeVector     LocalPosition;
    G4bool            IsEntering;
    G4bool            IsLeaving;
    G4double          PreMomTheta;
    G4double          PreMomPhi;
    G4double          PostMomTheta;
    G4double          PostMomPhi;
    G4double          PreKinEnergy;
    G4double          PostKinEnergy;
    G4String          ProcessName;
    G4double          EnergyDep;
    G4double          EnergyVis;
    G4double          DeltaTime;

private:
    static G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator_;

};

typedef G4THitsCollection<MyHit> MyHitsCollection;

inline void* MyHit::operator new(size_t) {
    if (MyHitAllocator_ == NULL)
        MyHitAllocator_ = new G4Allocator<MyHit>();
    return static_cast<void*>(MyHitAllocator_->MallocSingle());
}

inline void MyHit::operator delete(void* aHit) {
    MyHitAllocator_->FreeSingle(static_cast<MyHit*>(aHit));
}

#endif
