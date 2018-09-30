#ifndef MyRunAction_h
#define MyRunAction_h

#include "globals.hh"
#include "G4UserRunAction.hh"

class MyRunAction: public G4UserRunAction {
private:
    G4String output_file_;
    G4bool   fixed_output_;
    static G4int randomSeed_;
    static G4int runId_;

public:
    MyRunAction(G4String the_output_file_, G4bool fixed_name = false);
    ~MyRunAction();

    G4Run* GenerateRun();
    void BeginOfRunAction(const G4Run* aRun);
    void EndOfRunAction(const G4Run* aRun);

};

#endif /* MyRunAction_h */
