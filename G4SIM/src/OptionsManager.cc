#include "OptionsManager.hh"

using namespace std;

OptionsManager::OptionsManager() {
    init();
}

OptionsManager::~OptionsManager() {

}

bool OptionsManager::parse(int argc_par, char** argv_par) {
    if (argc_par < 2)
        return false;
    string cur_par_str;
    int idx = 0;
    while (idx < argc_par - 1) {
        cur_par_str = argv_par[++idx];
        if (cur_par_str == "--version") {
            version_flag_ = true;
            return false;
        }
        if (cur_par_str[0] == '-') {
            if (cur_par_str.length() != 2)
                return false;
            char cur_option = cur_par_str[1];
            switch (cur_option) {
            case 'g':
                if (ter_flag) {
                    return false;
                }
                gui_flag = true;
                if (idx < argc_par - 1) {
                    vis_mac_file = argv_par[++idx];
                    if (vis_mac_file[0] == '-') {
                        return false;
                    }
                } else {
                    return false;
                }
                break;
            case 't':
                if (gui_flag) {
                    return false;
                }
                ter_flag = true;
                if (idx < argc_par - 1) {
                    vis_mac_file = argv_par[++idx];
                    if (vis_mac_file[0] == '-') {
                        return false;
                    }
                } else {
                    return false;
                }
                break;
            case 'm':
                if (idx < argc_par - 1) {
                    string tmp_string = argv_par[++idx];
                    if (tmp_string[0] == '-') {
                        return false;
                    } else {
                        mt_flag = true;
                        num_of_thread = atoi(tmp_string.c_str());
                    }
                } else {
                    return false;
                }
                break;
            case 'o':
                if (idx < argc_par - 1) {
                    output_file = argv_par[++idx];
                    if (output_file[0] == '-') {
                        return false;
                    }
                } else {
                    return false;
                }
                break;
            case 'f':
                fixed_name = true;
                break;
            case 'c':
                if (idx < argc_par - 1) {
                    config_file = argv_par[++idx];
                    if (config_file[0] == '-') {
                        return false;
                    }
                } else {
                    return false;
                }
                break;
            default:
                return false;
            }
        } else {
            if (primary_file == "") {
                primary_file = cur_par_str;
                if (primary_file.size() < 6) return false;
                if (primary_file.substr(primary_file.size() - 3) == string("mac")) {
                    gps_flag = true;
                } else if (primary_file.substr(primary_file.size() - 4) == string("root")) {
                    gps_flag = false;
                } else {
                    cout << "wrong primary file extention." << endl;
                    return false;
                }
            } else if (run_mac_file == "") {
                run_mac_file = cur_par_str;
                if (run_mac_file.size() < 6) return false;
                if (run_mac_file.substr(run_mac_file.size() - 3) != string("mac")) {
                    cout << "wrong run file extention." << endl;
                    return false;
                }
            } else {
                return false;
            }
        }
    }

    if (vis_mac_file == "") {
        if (primary_file == "" || run_mac_file == "") {
            return false;
        }
    } else {
        if (vis_mac_file.substr(vis_mac_file.size() - 3) != string("mac")) {
            cout << "wrong vis file extention." << endl;
            return false;
        }
    }
    if (mt_flag) {
        if (num_of_thread < 1) {
            return false;
        }
    }
    if (!gps_flag) {
        mt_flag = false;
        num_of_thread = 0;
    }

    return true;
}

void OptionsManager::print_help() {
    cout << "Usage:" << endl;
    cout << "  " << SW_NAME << " -g <vis.mac>" << endl;
    cout << "  " << SW_NAME << " -t <init.mac>" << endl;
    cout << "  " << SW_NAME << " <gps.mac>  <run.mac> [-o <output.root>] [-m <num_of_thread>]" << endl;
    cout << "  " << SW_NAME << " <gen.root> <run.mac> [-o <output.root>] [-f]" << endl;
    cout << "  " << SW_NAME << " ... [-c <config_file.mac>]" << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << "  -g <visGUI.mac>                  macro file for gui vis" << endl;
    cout << "  -t <vis.mac>                     macro file for terminal vis" << endl;
    cout << "  -m <num_of_thread>               use MT mode, default is sequential mode" << endl;
    cout << "  -o <output.root>                 output file name" << endl;
    cout << "  -f                               fixed output filename for only sequential mode" << endl;
    cout << "  -c <config.mac>                  configure file executed before init" << endl;
    cout << endl;
    cout << "  --version                        print version and author information" << endl;
    cout << endl;
}

void OptionsManager::print_version() {
    cout << endl;
    cout << "    " << SW_NAME << " - G4SIM" << endl;
    cout << "    " << SW_VERSION << " (" << RELEASE_DATE << ", compiled " << __DATE__ << " " << __TIME__ << ")" << endl;
    cout << endl;
    cout << " Copyright (C) 2016-2017 G4SIM Group" << endl;
    cout << endl;
    cout << " Main Contributors: " << endl;
    cout << " - Zhengheng Li <lizhengheng@ihep.ac.cn>" << endl;
    cout << endl;
}

void OptionsManager::print_options() {

}

void OptionsManager::init() {
    vis_mac_file = "";
    gui_flag = false;
    ter_flag = false;
    primary_file = "";
    run_mac_file = "";
    gps_flag = true;
    output_file = "output.root";
    mt_flag = false;
    num_of_thread = 0;
    config_file = "";
    fixed_name = false;

    version_flag_ = false;
}

bool OptionsManager::get_version_flag() {
    return version_flag_;
}
