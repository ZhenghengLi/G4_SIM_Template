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
    TString cur_par_str;
    int idx = 0;
    while (idx < argc_par - 1) {
        cur_par_str = argv_par[++idx];
        if (cur_par_str == "--version") {
            version_flag_ = true;
            return false;
        }
        if (cur_par_str[0] == '-') {
            if (cur_par_str.Length() != 2)
                return false;
            char cur_option = cur_par_str[1];
            switch (cur_option) {
            case 'o':
                if (idx < argc_par - 1) {
                    digiout_filename = argv_par[++idx];
                    if (digiout_filename[0] == '-') {
                        return false;
                    }
                } else {
                    return false;
                }
                break;
            case 'c':
                if (idx < argc_par - 1) {
                    config_filename = argv_par[++idx];
                    if (config_filename[0] == '-') {
                        return false;
                    }
                } else {
                    return false;
                }
                break;
            case 't':
                if (idx < argc_par - 1) {
                    output_type = argv_par[++idx];
                    if (output_type[0] == '-') {
                        return false;
                    }
                } else {
                    return false;
                }
                break;
            case 's':
                if (idx < argc_par - 1) {
                    TString tmp_par = argv_par[++idx];
                    if (tmp_par[0] == '-') {
                        return false;
                    } else {
                        rand_seed = tmp_par.Atoi();
                    }
                } else {
                    return false;
                }
                break;
            default:
                return false;
            }
        } else {
            simdata_filelist.push_back(cur_par_str);
        }
    }
    if (simdata_filelist.empty())
        return false;
    if (config_filename.IsNull())
        return false;
    if (digiout_filename.IsNull())
        digiout_filename = "digiout.root";
    if (output_type.IsNull()) return false;
    if (output_type != "A" && output_type != "B") {
        return false;
    }
    return true;
}

void OptionsManager::print_help() {
    cout << "Usage:" << endl;
    cout << "  " << SW_NAME << " <simdata1.root> <simdata2.root> <...> -c <config.cfg> -t <A|B> -o <digiout.root> [-s <seed>]" << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << "  -c <config.root>                 configuration file" << endl;
    cout << "  -o <digiout.root>                digitalized event file" << endl;
    cout << "  -t <A|B>                   the type of output file" << endl;
    cout << "  -s <seed>                        seed of random generator" << endl;
    cout << endl;
    cout << "  --version                        print version and author information" << endl;
    cout << endl;
}

void OptionsManager::print_version() {
    cout << endl;
    cout << "    " << SW_NAME << " - Simulation" << endl;
    cout << "    " << SW_VERSION << " (" << RELEASE_DATE << ", compiled " << __DATE__ << " " << __TIME__ << ")" << endl;
    cout << endl;
    cout << " Copyright (C) 2016-2017 Simulation Group" << endl;
    cout << endl;
    cout << " Main Contributors: " << endl;
    cout << " - Zhengheng Li <lizhengheng@ihep.ac.cn>" << endl;
    cout << endl;
}

void OptionsManager::print_options() {

}

void OptionsManager::init() {
    simdata_filelist.clear();
    digiout_filename.Clear();
    config_filename.Clear();
    output_type.Clear();
    rand_seed = 0;

    version_flag_ = false;
}

bool OptionsManager::get_version_flag() {
    return version_flag_;
}
