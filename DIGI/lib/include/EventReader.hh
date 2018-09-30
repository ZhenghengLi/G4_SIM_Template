#ifndef EventReader_HH
#define EventReader_HH

#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"

using namespace std;

class EventReader {
public:
    struct Keyword_T {
        Int_t RunID;
        Int_t NumberOfEvents;
        Int_t TotalHits;
        Int_t TotalEvents;
    };

    struct Primary_T {
        // necessary
        Int_t    EventID;
        Int_t    HitsCount;
        // optional
        Char_t   PName_Primary[80];
        Float_t  X_Primary;
        Float_t  Y_Primary;
        Float_t  Z_Primary;
        Double_t Time_Primary;
        Float_t  Energy_Primary;
        Float_t  POLX_Primary;
        Float_t  POLY_Primary;
        Float_t  POLZ_Primary;
        Float_t  Theta_Primary;
        Float_t  Phi_Primary;
    };

    struct HitsCol_T {
        // necessary
        Int_t    EventID;
        Int_t    TrackID;
        Int_t    ParticleCode;
        Int_t    BarID;
        Int_t    ModID;
        Float_t  LocPosX;
        Float_t  LocPosY;
        Float_t  LocPosZ;
        Float_t  EnergyVis;
        // optional
        Char_t   ParticleName[80];
        Double_t PreGlobalTime;
        Int_t    StepStatus;
        Float_t  PreMomTheta;
        Float_t  PreMomPhi;
        Float_t  PostMomTheta;
        Float_t  PostMomPhi;
        Float_t  PreKinEnergy;
        Float_t  PostKinEnergy;
        Char_t   ProcessName[80];
        Float_t  EnergyDep;
        Double_t DeltaTime;
    };

private:
    TFile* t_file_in_;
    TTree* t_keyword_tree_;
    TTree* t_primary_tree_;
    TTree* t_hitscol_tree_;

    bool simout_more_;

    HitsCol_T cur_hit_;

    Long64_t  keyword_cur_entry_;
    bool      keyword_reach_end_;
    Long64_t  primary_cur_entry_;
    bool      primary_reach_end_;
    Long64_t  hitscol_cur_entry_;
    bool      hitscol_reach_end_;

public:
    Keyword_T         Keyword;
    Primary_T         Primary;
    vector<HitsCol_T> HitsCol;

public:
    EventReader();
    ~EventReader();

    bool open(const char* filename);
    void close();

    void keyword_start();
    bool keyword_next();

    void primary_start();
    bool primary_next();

    void event_start();
    bool event_next();

    Long64_t keyword_get_tot_entries();
    Long64_t keyword_get_cur_entry();
    Long64_t primary_get_tot_entries();
    Long64_t primary_get_cur_entry();
    Long64_t hitscol_get_tot_entries();
    Long64_t hitscol_get_cur_entry();

    void primary_deactive_all();
    void primary_active(const char* branch);
    void hitscol_deactive_all();
    void hitscol_active(const char* branch);

    bool get_simout_more();

};

#endif
