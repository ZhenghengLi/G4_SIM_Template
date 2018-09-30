#ifndef MyEventAction_hh
#define MyEventAction_hh 1

#include "G4UserEventAction.hh"
#include "G4Event.hh"

class MyEventAction: public G4UserEventAction {
public:
    MyEventAction();
    ~MyEventAction();

    void BeginOfEventAction(const G4Event* anEvent);
    void EndOfEventAction(const G4Event* anEvent);

};


#endif
