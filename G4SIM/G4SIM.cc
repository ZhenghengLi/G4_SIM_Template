#include "G4RunManager.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "MyDetectorConstruction.hh"
#include "MyPhysicsList.hh"
#include "MyActionInitialization.hh"
#include "GlobalConfig.hh"

#include "OptionsManager.hh"

int main(int argc,char** argv) {
    // process command parameters
    OptionsManager options_mgr;
    if (!options_mgr.parse(argc, argv)) {
        if (options_mgr.get_version_flag()) {
            options_mgr.print_version();
        } else {
            options_mgr.print_help();
        }
        return 2;
    }

    GlobalConfig* fGlobalConfig = GlobalConfig::Instance();

    G4UImanager* uiManager = G4UImanager::GetUIpointer();
    G4String execute = "/control/execute ";
    if (options_mgr.config_file != "") {
        uiManager->ApplyCommand(execute + options_mgr.config_file);
        fGlobalConfig->print_config();
    }

    MyDetectorConstruction* fMyDetectorConstruction = new MyDetectorConstruction();
    MyPhysicsList*          fMyPhysicsList          = new MyPhysicsList(fGlobalConfig->phys_verbose);
    MyActionInitialization* fMyActionInitialization = new MyActionInitialization(options_mgr.gps_flag, options_mgr.output_file, options_mgr.fixed_name);

#ifdef G4MULTITHREADED
    G4MTRunManager* runManagerMT = NULL;
    G4RunManager*   runManagerSQ = NULL;
    if (options_mgr.mt_flag && options_mgr.gps_flag) {
        runManagerMT = new G4MTRunManager();
        G4cout << "#### using multi-threaded mode with " << options_mgr.num_of_thread << " threads ####" << G4endl;
        runManagerMT->SetNumberOfThreads(options_mgr.num_of_thread);
        // set mandatory initialization classes
        runManagerMT->SetUserInitialization(fMyDetectorConstruction);
        runManagerMT->SetUserInitialization(fMyPhysicsList);
        runManagerMT->SetUserInitialization(fMyActionInitialization);
        // initialize G4 kernel
        G4cout << "initialize G4 kernel" << G4endl;
        runManagerMT->Initialize();
    } else {
        runManagerSQ = new G4RunManager();
        G4cout << "#### using sequential mode ####" << G4endl;
        // set mandatory initialization classes
        runManagerSQ->SetUserInitialization(fMyDetectorConstruction);
        runManagerSQ->SetUserInitialization(fMyPhysicsList);
        runManagerSQ->SetUserInitialization(fMyActionInitialization);
        // initialize G4 kernel
        G4cout << "initialize G4 kernel" << G4endl;
        runManagerSQ->Initialize();
    }
#else
    G4RunManager* runManager = new G4RunManager();
    G4cout << "#### WARNING: only sequential mode can be used ####" << G4endl;
    G4cout << "#### using sequential mode ####" << G4endl;
    // set mandatory initialization classes
    runManager->SetUserInitialization(fMyDetectorConstruction);
    runManager->SetUserInitialization(fMyPhysicsList);
    runManager->SetUserInitialization(fMyActionInitialization);
    // initialize G4 kernel
    G4cout << "initialize G4 kernel" << G4endl;
    runManager->Initialize();
#endif

    G4cout << "#### G4 KERNEL INITIALIZED ####" << G4endl;

#ifdef G4VIS_USE
    G4VisManager* visManager = NULL;
    if (options_mgr.gui_flag || options_mgr.ter_flag) {
        visManager = new G4VisExecutive("warnings"); // "quiet", "errors", "warnings"
        visManager->Initialize();
        G4cout << "#### G4 VIS INITIALIZED ####" << G4endl;
    }
#endif

    if (options_mgr.gui_flag) {
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(1, argv);
        uiManager->ApplyCommand(execute + options_mgr.vis_mac_file);
        ui->SessionStart();
        delete ui;
#else
        G4cout << " ******** No UI_USE ********" << endl;
#endif
    } else if (options_mgr.ter_flag) {
        G4UIsession* session = NULL;
#ifdef G4UI_USE_TCSH
        session = new G4UIterminal(new G4UItcsh);
#else
        session = new G4UIterminal();
#endif
        uiManager->ApplyCommand(execute + options_mgr.vis_mac_file);
        session->SessionStart();
        delete session;
    } else {
        // start simulation
        G4cout<< "************** good morning **************" << G4endl;

        if (options_mgr.gps_flag) {
            G4cout << "==== using general particle source ====" << G4endl;
            uiManager->ApplyCommand(execute + options_mgr.primary_file);
        } else {
            G4cout << "==== using root file of particle data ====" << G4endl;
            if (!fMyActionInitialization->GetParticleDataFileR()->open(options_mgr.primary_file.c_str())) {
                G4cout << "particle data file open failed." << G4endl;
                return 1;
            }
        }

        uiManager->ApplyCommand(execute + options_mgr.run_mac_file);

        if (!options_mgr.gps_flag) {
            fMyActionInitialization->GetParticleDataFileR()->close();
        }

        G4cout<< "************** good night **************" << G4endl;
    }

    // job termination
#ifdef G4VIS_USE
    if (options_mgr.gui_flag || options_mgr.ter_flag) {
        delete visManager;
    }
#endif
#ifdef G4MULTITHREADED
    if (options_mgr.mt_flag) {
        delete runManagerMT;
    } else {
        delete runManagerSQ;
    }
#else
    delete runManager;
#endif
    return 0;
}

