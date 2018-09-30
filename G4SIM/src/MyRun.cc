#include "MyRun.hh"
#include "MyHit.hh"
#include "G4SDManager.hh"
#include "GlobalConfig.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

MyRun::MyRun() {
    totalHits_ = 0;
    totalEvents_ = 0;
}

MyRun::~MyRun() {

}

void MyRun::RecordEvent(const G4Event* anEvent) {
    G4PrimaryVertex* primaryVertex = anEvent->GetPrimaryVertex(0);
    if (primaryVertex == NULL)
        return;

    G4Run::RecordEvent(anEvent);

    GlobalConfig* fGlobalConfig = GlobalConfig::Instance();

    // calculate and record value
    G4int theEventID = anEvent->GetEventID();
    MyHitsCollection* fMyHitsCollection = NULL;
    G4int numberOfHits = 0;
    if (!fGlobalConfig->primary_only) {
        G4int collectionID = G4SDManager::GetSDMpointer()->GetCollectionID("MyHitsCollection");
        fMyHitsCollection = static_cast<MyHitsCollection*>(anEvent->GetHCofThisEvent()->GetHC(collectionID));
        if (fMyHitsCollection == NULL) {
            G4ExceptionDescription msg;
            msg << "Cannot access hitsCollection: MyHitsCollection with ID " << collectionID;
            G4Exception("MyEventAction::EndOfEventAction", "MyCode0001", FatalException, msg);
        }
        numberOfHits = fMyHitsCollection->entries();
        if (numberOfHits < 1) return;
        double energy_dep[25][64];
        for (int i = 0; i < 25; i++) {
            for (int j = 0; j < 64; j++) {
                energy_dep[i][j] = 0.0;
            }
        }
        for (int i = 0; i < numberOfHits; i++) {
            MyHit* aHit = static_cast<MyHit*>(fMyHitsCollection->GetHit(i));
            energy_dep[aHit->ModID][aHit->BarID] += aHit->EnergyDep;
        }
        double max_energy_dep = 0;
        for (int i = 0; i < 25; i++) {
            for (int j = 0; j < 64; j++) {
                if (max_energy_dep < energy_dep[i][j]) {
                    max_energy_dep = energy_dep[i][j];
                }
            }
        }
        if (max_energy_dep < fGlobalConfig->bar_threshold) return;
    }

    G4AnalysisManager* fAnalysisManger = G4AnalysisManager::Instance();
    G4int firstNtupleId = 0;
    if (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::RMType::sequentialRM) {
        firstNtupleId = 1;
    }

    // read primary
    if (anEvent->GetNumberOfPrimaryVertex() > 1) {
        G4ExceptionDescription msg;
        msg << "More than one primary particle for one event.";
        G4Exception("MyEventAction::EndOfEventAction", "MyCode0002", FatalException, msg);
    }
    G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary(0);
    G4String        pname_primary   = primaryParticle->GetParticleDefinition()->GetParticleName();
    G4ThreeVector   xyz_primary     = primaryVertex->GetPosition();
    G4double        tim_primary     = primaryVertex->GetT0();
    G4double        ene_primary     = primaryParticle->GetKineticEnergy();
    G4ThreeVector   pol_primary     = primaryParticle->GetPolarization();
    G4ThreeVector   direction       = primaryParticle->GetMomentumDirection();
    G4double        theta_primary   = (-direction).theta();
    G4double        phi_primary     = (-direction).phi();
    if (phi_primary < 0)
        phi_primary += twopi;

    // fill ntuple
    // necessary
    fAnalysisManger->FillNtupleIColumn(firstNtupleId + 0, 0 , theEventID               );
    fAnalysisManger->FillNtupleIColumn(firstNtupleId + 0, 1 , numberOfHits             );
    // optional
    if (fGlobalConfig->simout_more) {
        fAnalysisManger->FillNtupleSColumn(firstNtupleId + 0, 2 , pname_primary        );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 0, 3 , xyz_primary.x() / cm );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 0, 4 , xyz_primary.y() / cm );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 0, 5 , xyz_primary.z() / cm );
        fAnalysisManger->FillNtupleDColumn(firstNtupleId + 0, 6 , tim_primary / second );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 0, 7 , ene_primary / keV    );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 0, 8 , pol_primary.x()      );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 0, 9 , pol_primary.y()      );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 0, 10, pol_primary.z()      );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 0, 11, theta_primary / deg  );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 0, 12, phi_primary / deg    );
    }
    fAnalysisManger->AddNtupleRow(firstNtupleId + 0);
    totalEvents_++;

    if (fGlobalConfig->primary_only) return;

    if (fGlobalConfig->event_verbose > 1)
        G4cout << "---> (Record Event) Begin of event: " << theEventID << G4endl;

    // save hits
    for (int i = 0; i < numberOfHits; i++) {
        MyHit* aHit = static_cast<MyHit*>(fMyHitsCollection->GetHit(i));
        // necessary
        fAnalysisManger->FillNtupleIColumn(firstNtupleId + 1, 0 , theEventID                       );
        fAnalysisManger->FillNtupleIColumn(firstNtupleId + 1, 1 , aHit->TrackID                    );
        fAnalysisManger->FillNtupleIColumn(firstNtupleId + 1, 2 , aHit->ParticleCode               );
        fAnalysisManger->FillNtupleIColumn(firstNtupleId + 1, 3 , aHit->BarID                      );
        fAnalysisManger->FillNtupleIColumn(firstNtupleId + 1, 4 , aHit->ModID                      );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 5 , aHit->LocalPosition.x() / mm     );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 6 , aHit->LocalPosition.y() / mm     );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 7 , aHit->LocalPosition.z() / mm     );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 8 , aHit->EnergyVis / keV            );
        fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 9 , aHit->EnergyDep / keV            );
        // optional
        if (fGlobalConfig->simout_more) {
            fAnalysisManger->FillNtupleSColumn(firstNtupleId + 1, 10, aHit->ParticleName           );
            fAnalysisManger->FillNtupleDColumn(firstNtupleId + 1, 11, aHit->GlobalTime / second    );
            G4int stepStatus = 0;
            if (aHit->IsEntering) stepStatus += 1;
            if (aHit->IsLeaving) stepStatus += 2;
            fAnalysisManger->FillNtupleIColumn(firstNtupleId + 1, 12, stepStatus                   );
            fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 13, aHit->PreMomTheta / deg      );
            fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 14, aHit->PreMomPhi / deg        );
            fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 15, aHit->PostMomTheta / deg     );
            fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 16, aHit->PostMomPhi / deg       );
            fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 17, aHit->PreKinEnergy / keV     );
            fAnalysisManger->FillNtupleFColumn(firstNtupleId + 1, 18, aHit->PostKinEnergy / keV    );
            fAnalysisManger->FillNtupleSColumn(firstNtupleId + 1, 19, aHit->ProcessName            );
            fAnalysisManger->FillNtupleDColumn(firstNtupleId + 1, 20, aHit->DeltaTime / second     );
        }
        fAnalysisManger->AddNtupleRow(firstNtupleId + 1);
        totalHits_++;
    }

    if (fGlobalConfig->event_verbose > 1)
        G4cout << "---> (Record Event) End of event: " << theEventID << G4endl;
}

void MyRun::Merge(const G4Run* aRun) {
    G4Run::Merge(aRun);

    const MyRun* localRun = static_cast<const MyRun*>(aRun);
    totalHits_ += localRun->totalHits_;
    totalEvents_ += localRun->totalEvents_;

}
