#ifndef MySteppingAction_h
#define MySteppingAction_h 1

#include "G4UserSteppingAction.hh"

class MySteppingAction: public G4UserSteppingAction {
public:
    MySteppingAction();
    ~MySteppingAction();

    void UserSteppingAction(const G4Step* aStep);

};

#endif
