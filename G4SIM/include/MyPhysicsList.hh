#ifndef MyPhysicsList_h
#define MyPhysicsList_h 1

#include "G4VModularPhysicsList.hh"

class MyPhysicsList: public G4VModularPhysicsList {
public:
    MyPhysicsList(G4int ver = 1);
    ~MyPhysicsList();

    void SetCuts();

};

#endif /* MyPhysicsList_h */
