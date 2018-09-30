#ifndef OPTIONSMANAGER_H
#define OPTIONSMANAGER_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include "Constants.hh"

using namespace std;

class OptionsManager {
private:
    bool version_flag_;

public:
    string vis_mac_file;
    bool   gui_flag;         // -g
    bool   ter_flag;         // -t
    string primary_file;
    string run_mac_file;
    bool   gps_flag;
    string output_file;      // -o
    bool   mt_flag;          // -m
    int    num_of_thread;
    string config_file;      // -c
    bool   fixed_name;

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
