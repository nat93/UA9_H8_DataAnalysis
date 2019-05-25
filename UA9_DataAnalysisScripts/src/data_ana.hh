///////////////////////////////////////////////////////////////////////
///                Date: 25/05/2019                                 ///
///               Autor: Andrii Natochii                            ///
/// Program description: Data analysis Class description.           ///
///////////////////////////////////////////////////////////////////////

#ifndef data_ana_hh
#define data_ana_hh

#include "includes.hh"

using namespace std;

class data_ana
{
public:
    data_ana(TString data_file_name);
    ~data_ana();

    void plot_histogramms();
    void write_histo_to_file(TString outFileName);
    void get_torsion_efficiency_optimization(TString outFileName);
    void find_inelastic_counts_fixed(TString outputfilename, Int_t year, Int_t runID);
    void find_inelastic_counts_scan(TString outputfilename, Int_t year, Int_t runID);

private:
    void GetParameters(string filenamepath, double &_min_d0x, double &_max_d0x, double &_min_d0y, double &_max_d0y, double &_gpos, double &_k1, double &_k2, double &_a, double &_b);
    int bit2det(int bit);
    int det2bit(int det);

    Int_t nentries;
    TFile * file;
    TTree * tree;
    TBranch * bEvent;
    TBranch * bTime;
    TBranch * bDate;
    TBranch * bGonioPos;
    TBranch * bMultiHit;
    TBranch * bMultiHits;
    TBranch * bSingleTrack;
    TBranch * bTracks;

    double  INx_multi;
    double  INy_multi;
    double  IMPx_multi;
    double  IMPy_multi;
    double  INx;
    double  INy;
    double  OUTx;
    double  OUTy;
    double  IMPx;
    double  IMPy;

//    vector<double> * Hits4x = 0;
//    vector<double> * Hits4y = 0;

    TH1D *hist1;
    TH1D *hist2;
    TH1D *hist3;
    TH1D *hist4;
    TH1D *hist5;
    TH1D *hist6;
    TH2D *hist7;
    TH2D *hist8;
    TH2D *hist9;
    TH2D *hist10;
    TH2D *hist11;
    TH2D *hist12;
    TH2D *hist13;
    TH2D *hist14;
    TH1D *hist15;
    TH1D *hist16;
    TH2D *hist17;
    TH2D *hist18;
    TH2D *hist19;
    TH2D *hist20;
    TH2D *hist21;

    TH1D *hist1_cut;
    TH1D *hist2_cut;
    TH1D *hist3_cut;
    TH1D *hist4_cut;
    TH1D *hist5_cut;
    TH1D *hist6_cut;
    TH2D *hist7_cut;
    TH2D *hist8_cut;
    TH2D *hist9_cut;
    TH2D *hist10_cut;
    TH2D *hist11_cut;
    TH2D *hist12_cut;
    TH2D *hist13_cut;
    TH2D *hist14_cut;
    TH1D *hist15_cut;
    TH1D *hist16_cut;
    TH2D *hist17_cut;
    TH2D *hist18_cut;
    TH2D *hist19_cut;
    TH2D *hist20_cut;
    TH2D *hist21_cut;

    Double_t F12;
    Double_t errF12;
    Double_t max_d0x;
    Double_t min_d0x;
    Double_t max_d0x_sept;
    Double_t min_d0x_sept;
    Double_t max_d0y;
    Double_t min_d0y;
    Double_t initial_gpos_x;
    Double_t k1;
    Double_t k2;
    Double_t a;
    Double_t b;
    Double_t angular_cut;
    Double_t distL;

    Int_t cut_d0x;
    Int_t cut_d0y;

    struct Event
    {
        int run;
        int evtnum;
        int nuclear;
        int nuclearRaw;
    } evt;

    struct GonioPos
    {
        double x;
        double y;
        double z;
    } gPos;

    struct MultiHit
    {
        int p_nHits[5];
        double thetaIn_x;
        double thetaIn_y;
        double thetaInErr_x;
        double thetaInErr_y;
        double d0_x;
        double d0_y;
        double d0Err_x;
        double d0Err_y;
    } hits;

    struct Track
    {
        double thetaIn_x;
        double thetaIn_y;
        double thetaOut_x;
        double thetaOut_y;
        double thetaInErr_x;
        double thetaInErr_y;
        double thetaOutErr_x;
        double thetaOutErr_y;
        double d0_x;
        double d0_y;
        double d0Err_x;
        double d0Err_y;
        double chi2_x;
        double chi2_y;
    } tracks;

    int isHit;

    int isTrack;

    char time[80];
    char date[80];

    // For a fixed position
    static const Int_t number_of_cutting_angels = 8;            // number of the cut angles
    static const Double_t theta_cut_min         = 2.5e-6;       // rad
    static const Double_t theta_cut_max         = 20.0e-6;      // rad

    // For an angular scan
    static const Int_t number_of_cutting_angel_center_position  = 31;           // number of angular scan positions
    static const Double_t theta_cut_center_min                  = -50.0e-6;     // minimum angular scan edge
    static const Double_t theta_cut_center_max                  =  100.0e-6;    // maximum angular scan edge
    static const Double_t theta_cut_fixed                       = 2.5e-6;       // cutting angle for ech point

    //----------------------------------------------------------//
    //----------------------------------------------------------//
    // Plastic Scintillators Definition Since 2018              //
    //----------------------------------------------------------//
    static const Int_t signal1                  = 0;            //
    static const Int_t signal2                  = 1;            //
    //----------------------------------------------------------//
    // Cable name   || Cable Input  || Data Bit || Coded 8b Word//
    // SCINT 01     || signal0      || 5        || 223          //
    // SCINT 23     || signal1      || 4        || 239          //
    // INI 01       || signal2      || 7        || 127          //
    // INI 23       || signal3      || 6        || 191          //
    // ARD door     || signal4      || 1        || 253          //
    // ARD wall     || signal5      || 0        || 254          //
    //----------------------------------------------------------//
    //----------------------------------------------------------//

};

#endif

