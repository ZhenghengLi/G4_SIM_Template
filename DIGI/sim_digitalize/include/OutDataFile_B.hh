#ifndef OutDataFile_B_HH
#define OutDataFile_B_HH

#include "OutDataFile.hh"

class OutDataFile_B : public OutDataFile {
public:
    struct Modules_T {
        Long64_t    evnt;
        Long64_t    trigg_num;
        Bool_t      trigger_bit[64];
        Float_t     energy_adc[64];
        Float_t     common_noise;
        Int_t       ct_num;
        Int_t       multiplicity;
        Int_t       total_n;
        Int_t       isped;
        Double_t    hv;
        Int_t       badch;
        Int_t       compress;
        Int_t       is_bad;
        Double_t    temperature;
        UShort_t    status;
        Int_t       raw_rate;
        Float_t     dead_ratio;
        Int_t       count;
        Double_t    time_wait;
    };

    struct Trigger_T {
        Long64_t    pkt_start;
        Int_t       pkt_count;
        Int_t       lost_count;
        Int_t       is_bad;
        Double_t    time_second;
        Long64_t    trigg_num;
        Double_t    time_wait;
        Double_t    tunix;
        Int_t       isped;
    };

private:
    TFile* t_file_out_;
    TTree* t_modules_tree_;
    TTree* t_trigger_tree_;

private:
    Modules_T t_modules_;
    Trigger_T t_trigger_;

private:
    Long64_t trigg_count_;
    Long64_t event_count_;
    Double_t time_second_;
    Long64_t evnt_count_;

    long int total_events_;

public:
    OutDataFile_B();
    virtual ~OutDataFile_B();

    bool open(const char* filename);
    void write_event(const Digi::GeneralEvent_T& general_event);
    void write_tree();
    void close();
    long int get_total_events();

};

#endif
