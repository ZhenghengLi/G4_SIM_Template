#ifndef OutDataFile_HH
#define OutDataFile_HH

#include <iostream>
#include "RootInc.hh"
#include "Digi.hh"

using namespace std;

class OutDataFile {
public:
    OutDataFile();
    virtual ~OutDataFile();

    virtual bool open(const char* filename) = 0;
    virtual void write_event(const Digi::GeneralEvent_T& general_event) = 0;
    virtual void write_tree() = 0;
    virtual void close() = 0;
    virtual long int get_total_events() = 0;

};

#endif
