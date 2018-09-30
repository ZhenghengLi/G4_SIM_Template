#ifndef OPTIONSMANAGER_H
#define OPTIONSMANAGER_H

#include <iostream>
#include <vector>
#include "TString.h"
#include "Constants.hh"

using namespace std;

class OptionsManager {
private:
    bool version_flag_;

public:
    vector<TString> simdata_filelist;
    TString digiout_filename;   // -o
    TString config_filename;    // -c
    TString output_type;        // -t A|B
    int     rand_seed;          // -s

public:
    OptionsManager();
    ~OptionsManager();

    bool parse(int argc_par, char** argv_par);
    void print_help();
    void print_version();
    void print_options();
    void init();
    bool get_version_flag();

};

#endif
