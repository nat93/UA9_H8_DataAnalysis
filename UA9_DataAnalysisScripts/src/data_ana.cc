///////////////////////////////////////////////////////////////////////
///                Date: 25/05/2019                                 ///
///               Autor: Andrii Natochii                            ///
/// Program description: Data analysis Class description.           ///
///////////////////////////////////////////////////////////////////////

#include "data_ana.hh"

using namespace std;

data_ana::data_ana(TString data_file_name)
{
    file = new TFile(data_file_name,"READ");
    tree = (TTree*)file->Get("simpleEvent");

    nentries = (Int_t)tree->GetEntries();

    cout<<"--> nEntries: "<<nentries<<endl;

    bEvent          = tree->GetBranch("Event");
    bTime           = tree->GetBranch("Time");
    bDate           = tree->GetBranch("Date");
    bGonioPos       = tree->GetBranch("GonioPos");
    bMultiHit       = tree->GetBranch("MultiHit");
    bMultiHits      = tree->GetBranch("MultiHits");
    bSingleTrack    = tree->GetBranch("SingleTrack");
    bTracks         = tree->GetBranch("Tracks");

    bEvent->SetAddress(&evt);
    bTime->SetAddress(&time);
    bDate->SetAddress(&date);
    bGonioPos->SetAddress(&gPos);
    bMultiHit->SetAddress(&isHit);
    bMultiHits->SetAddress(&hits);
    bSingleTrack->SetAddress(&isTrack);
    bTracks->SetAddress(&tracks);

    // Multi track
    // IN-coming
    TLeaf * In_x_m = bMultiHits->GetLeaf("thetaIn_x");
    In_x_m->SetAddress(&INx_multi);

    TLeaf * In_y_m = bMultiHits->GetLeaf("thetaIn_y");
    In_y_m->SetAddress(&INy_multi);

    // Impact parameter
    TLeaf * Imp_x_m = bMultiHits->GetLeaf("d0_x");
    Imp_x_m->SetAddress(&IMPx_multi);

    TLeaf * Imp_y_m = bMultiHits->GetLeaf("d0_y");
    Imp_y_m->SetAddress(&IMPy_multi);

    // Single track
    // IN-coming
    TLeaf * In_x = bTracks->GetLeaf("thetaIn_x");
    In_x->SetAddress(&INx);

    TLeaf * In_y = bTracks->GetLeaf("thetaIn_y");
    In_y->SetAddress(&INy);

    // OUT-coming
    TLeaf * Out_x = bTracks->GetLeaf("thetaOut_x");
    Out_x->SetAddress(&OUTx);

    TLeaf * Out_y = bTracks->GetLeaf("thetaOut_y");
    Out_y->SetAddress(&OUTy);

    // Impact parameter
    TLeaf * Imp_x = bTracks->GetLeaf("d0_x");
    Imp_x->SetAddress(&IMPx);

    TLeaf * Imp_y = bTracks->GetLeaf("d0_y");
    Imp_y->SetAddress(&IMPy);

    // Hits Position, Since 2018
//    tree->SetBranchAddress("Hits4x",&Hits4x);
//    tree->SetBranchAddress("Hits4y",&Hits4y);

    GetParameters(data_file_name.Data(), min_d0x, max_d0x, min_d0y, max_d0y, initial_gpos_x, k1, k2, a, b);

    angular_cut = TMath::Sqrt(2.0*20.0/(400e9))*1e6; // For CH efficiency measurement [urad], 400 GeV/c
}

data_ana::~data_ana()
{
    tree->Delete();
    file->Delete();
}

void data_ana::plot_histogramms()
{
    cout<<endl;
    cout<<endl<<"--> Plotting histogramms <--"<<endl;
    cout<<"--> max_d0x = "<<max_d0x<<endl;
    cout<<"--> min_d0x = "<<min_d0x<<endl;
    cout<<"--> max_d0y = "<<max_d0y<<endl;
    cout<<"--> min_d0y = "<<min_d0y<<endl;
    cout<<"--> k1 = "<<k1<<endl;
    cout<<"--> k2 = "<<k2<<endl;
    cout<<"--> a = "<<a<<endl;
    cout<<"--> b = "<<b<<endl;
    cout<<"--> angular_cut = "<<angular_cut<<endl;
    cout<<"--> initial_gpos_x = "<<initial_gpos_x<<endl;
    cout<<endl;

    //-------------------------------//
    //--- Histograms without cuts ---//
    //-------------------------------//

    // ThetaX
    hist1 = new TH1D("hist1","#Theta^{in}_{X}",2000,-500.0,500.0);
    hist2 = new TH1D("hist2","#Theta^{out}_{X}",2000,-500.0,500.0);
    // ThetaY
    hist3 = new TH1D("hist3","#Theta^{in}_{Y}",2000,-500.0,500.0);
    hist4 = new TH1D("hist4","#Theta^{out}_{Y}",2000,-500.0,500.0);
    // dThetaX
    hist5 = new TH1D("hist5","#Delta#Theta_{X}",2000,-500.0,500.0);
    // dThetaX
    hist6 = new TH1D("hist6","#Delta#Theta_{Y}",2000,-500.0,500.0);
    // dThetaY vs dThetaX
    hist7 = new TH2D("hist7","#Delta#Theta_{Y} vs #Delta#Theta_{X}",2000,-500.0,500.0,2000,-500.0,500.0);
    // dThetaX vs ThetaX
    hist8 = new TH2D("hist8","#Delta#Theta_{X} vs #Theta^{in}_{X}",2000,-500.0,500.0,2000,-500.0,500.0);
    // dThetaY vs ThetaY
    hist9 = new TH2D("hist9","#Delta#Theta_{Y} vs #Theta^{in}_{Y}",2000,-500.0,500.0,2000,-500.0,500.0);
    // dThetaX vs d0X
    hist10 = new TH2D("hist10","#Delta#Theta_{X} vs d0_{X}",4000,-20.0,20.0,2000,-500.0,500.0);
    // dThetaX vs d0Y
    hist11 = new TH2D("hist11","#Delta#Theta_{X} vs d0_{Y}",4000,-20.0,20.0,2000,-500.0,500.0);
    // dThetaY vs d0X
    hist12 = new TH2D("hist12","#Delta#Theta_{Y} vs d0_{X}",4000,-20.0,20.0,2000,-500.0,500.0);
    // dThetaY vs d0Y
    hist13 = new TH2D("hist13","#Delta#Theta_{Y} vs d0_{Y}",4000,-20.0,20.0,2000,-500.0,500.0);
    // d0X vs d0Y
    hist14 = new TH2D("hist14","d0_{Y} vs d0_{X}",4000,-20.0,20.0,4000,-20.0,20.0);
    // d0X
    hist15 = new TH1D("hist15","d0_{X}",4000,-20.0,20.0);
    // d0Y
    hist16 = new TH1D("hist16","d0_{Y}",4000,-20.0,20.0);
    // ThetaY vs ThetaX
    hist17 = new TH2D("hist17","#Theta_{Y} vs #Theta_{X}",2000,-500.0,500.0,2000,-500.0,500.0);
    // ThetaX vs d0X
    hist18 = new TH2D("hist18","#Theta_{X} vs d0_{X}",4000,-20.0,20.0,2000,-500.0,500.0);
    // ThetaX vs d0Y
    hist19 = new TH2D("hist19","#Theta_{X} vs d0_{Y}",4000,-20.0,20.0,2000,-500.0,500.0);
    // ThetaY vs d0X
    hist20 = new TH2D("hist20","#Theta_{Y} vs d0_{X}",4000,-20.0,20.0,2000,-500.0,500.0);
    // ThetaY vs d0Y
    hist21 = new TH2D("hist21","#Theta_{Y} vs d0_{Y}",4000,-20.0,20.0,2000,-500.0,500.0);

    //----------------------------//
    //--- Histograms with cuts ---//
    //----------------------------//

    // ThetaX
    hist1_cut = new TH1D("hist1_cut","#Theta^{in}_{X}",2000,-500.0,500.0);
    hist2_cut = new TH1D("hist2_cut","#Theta^{out}_{X}",2000,-500.0,500.0);
    // ThetaY
    hist3_cut = new TH1D("hist3_cut","#Theta^{in}_{Y}",2000,-500.0,500.0);
    hist4_cut = new TH1D("hist4_cut","#Theta^{out}_{Y}",2000,-500.0,500.0);
    // dThetaX
    hist5_cut = new TH1D("hist5_cut","#Delta#Theta_{X}",2000,-500.0,500.0);
    // dThetaX
    hist6_cut = new TH1D("hist6_cut","#Delta#Theta_{Y}",2000,-500.0,500.0);
    // dThetaY vs dThetaX
    hist7_cut = new TH2D("hist7_cut","#Delta#Theta_{Y} vs #Delta#Theta_{X}",2000,-500.0,500.0,2000,-500.0,500.0);
    // dThetaX vs ThetaX
    hist8_cut = new TH2D("hist8_cut","#Delta#Theta_{X} vs #Theta^{in}_{X}",2000,-500.0,500.0,2000,-500.0,500.0);
    // dThetaY vs ThetaY
    hist9_cut = new TH2D("hist9_cut","#Delta#Theta_{Y} vs #Theta^{in}_{Y}",2000,-500.0,500.0,2000,-500.0,500.0);
    // dThetaX vs d0X
    hist10_cut = new TH2D("hist10_cut","#Delta#Theta_{X} vs d0_{X}",4000,-20.0,20.0,2000,-500.0,500.0);
    // dThetaX vs d0Y
    hist11_cut = new TH2D("hist11_cut","#Delta#Theta_{X} vs d0_{Y}",4000,-20.0,20.0,2000,-500.0,500.0);
    // dThetaY vs d0X
    hist12_cut = new TH2D("hist12_cut","#Delta#Theta_{Y} vs d0_{X}",4000,-20.0,20.0,2000,-500.0,500.0);
    // dThetaY vs d0Y
    hist13_cut = new TH2D("hist13_cut","#Delta#Theta_{Y} vs d0_{Y}",4000,-20.0,20.0,2000,-500.0,500.0);
    // d0X vs d0Y
    hist14_cut = new TH2D("hist14_cut","d0_{Y} vs d0_{X}",4000,-20.0,20.0,4000,-20.0,20.0);
    // d0X
    hist15_cut = new TH1D("hist15_cut","d0_{X}",4000,-20.0,20.0);
    // d0Y
    hist16_cut = new TH1D("hist16_cut","d0_{Y}",4000,-20.0,20.0);
    // ThetaY vs ThetaX
    hist17_cut = new TH2D("hist17_cut","#Theta_{Y} vs #Theta_{X}",2000,-500.0,500.0,2000,-500.0,500.0);
    // ThetaX vs d0X
    hist18_cut = new TH2D("hist18_cut","#Theta_{X} vs d0_{X}",4000,-20.0,20.0,2000,-500.0,500.0);
    // ThetaX vs d0Y
    hist19_cut = new TH2D("hist19_cut","#Theta_{X} vs d0_{Y}",4000,-20.0,20.0,2000,-500.0,500.0);
    // ThetaY vs d0X
    hist20_cut = new TH2D("hist20_cut","#Theta_{Y} vs d0_{X}",4000,-20.0,20.0,2000,-500.0,500.0);
    // ThetaY vs d0Y
    hist21_cut = new TH2D("hist21_cut","#Theta_{Y} vs d0_{Y}",4000,-20.0,20.0,2000,-500.0,500.0);

    //-------------------//
    //--- Axis labels ---//
    //-------------------//

    hist1->GetXaxis()->SetTitle("#murad");
    hist2->GetXaxis()->SetTitle("#murad");
    hist3->GetXaxis()->SetTitle("#murad");
    hist4->GetXaxis()->SetTitle("#murad");
    hist5->GetXaxis()->SetTitle("#murad");
    hist6->GetXaxis()->SetTitle("#murad");
    hist7->GetXaxis()->SetTitle("#Delta#Theta_{X} , #murad");
    hist7->GetYaxis()->SetTitle("#Delta#Theta_{Y} , #murad");
    hist8->GetXaxis()->SetTitle("#Theta^{in}_{X} , #murad");
    hist8->GetYaxis()->SetTitle("#Delta#Theta_{X} , #murad");
    hist9->GetXaxis()->SetTitle("#Theta^{in}_{Y} , #murad");
    hist9->GetYaxis()->SetTitle("#Delta#Theta_{Y} , #murad");
    hist10->GetXaxis()->SetTitle("d0_{X} , mm");
    hist10->GetYaxis()->SetTitle("#Delta#Theta_{X} , #murad");
    hist11->GetXaxis()->SetTitle("d0_{Y} , mm");
    hist11->GetYaxis()->SetTitle("#Delta#Theta_{X} , #murad");
    hist12->GetXaxis()->SetTitle("d0_{X} , mm");
    hist12->GetYaxis()->SetTitle("#Delta#Theta_{Y} , #murad");
    hist13->GetXaxis()->SetTitle("d0_{Y} , mm");
    hist13->GetYaxis()->SetTitle("#Delta#Theta_{Y} , #murad");
    hist14->GetXaxis()->SetTitle("d0_{X} , mm");
    hist14->GetYaxis()->SetTitle("d0_{Y} , mm");
    hist15->GetXaxis()->SetTitle("mm");
    hist16->GetXaxis()->SetTitle("mm");
    hist17->GetYaxis()->SetTitle("#Theta^{in}_{Y} , #murad");
    hist17->GetXaxis()->SetTitle("#Theta^{in}_{X} , #murad");
    hist18->GetYaxis()->SetTitle("#Theta^{in}_{X} , #murad");
    hist18->GetXaxis()->SetTitle("d0_{X} , mm");
    hist19->GetYaxis()->SetTitle("#Theta^{in}_{X} , #murad");
    hist19->GetXaxis()->SetTitle("d0_{Y} , mm");
    hist20->GetYaxis()->SetTitle("#Theta^{in}_{Y} , #murad");
    hist20->GetXaxis()->SetTitle("d0_{X} , mm");
    hist21->GetYaxis()->SetTitle("#Theta^{in}_{Y} , #murad");
    hist21->GetXaxis()->SetTitle("d0_{Y} , mm");

    hist1_cut->GetXaxis()->SetTitle("#murad");
    hist2_cut->GetXaxis()->SetTitle("#murad");
    hist3_cut->GetXaxis()->SetTitle("#murad");
    hist4_cut->GetXaxis()->SetTitle("#murad");
    hist5_cut->GetXaxis()->SetTitle("#murad");
    hist6_cut->GetXaxis()->SetTitle("#murad");
    hist7_cut->GetXaxis()->SetTitle("#Delta#Theta_{X} , #murad");
    hist7_cut->GetYaxis()->SetTitle("#Delta#Theta_{Y} , #murad");
    hist8_cut->GetXaxis()->SetTitle("#Theta^{in}_{X} , #murad");
    hist8_cut->GetYaxis()->SetTitle("#Delta#Theta_{X} , #murad");
    hist9_cut->GetXaxis()->SetTitle("#Theta^{in}_{Y} , #murad");
    hist9_cut->GetYaxis()->SetTitle("#Delta#Theta_{Y} , #murad");
    hist10_cut->GetXaxis()->SetTitle("d0_{X} , mm");
    hist10_cut->GetYaxis()->SetTitle("#Delta#Theta_{X} , #murad");
    hist11_cut->GetXaxis()->SetTitle("d0_{Y} , mm");
    hist11_cut->GetYaxis()->SetTitle("#Delta#Theta_{X} , #murad");
    hist12_cut->GetXaxis()->SetTitle("d0_{X} , mm");
    hist12_cut->GetYaxis()->SetTitle("#Delta#Theta_{Y} , #murad");
    hist13_cut->GetXaxis()->SetTitle("d0_{Y} , mm");
    hist13_cut->GetYaxis()->SetTitle("#Delta#Theta_{Y} , #murad");
    hist14_cut->GetXaxis()->SetTitle("d0_{X} , mm");
    hist14_cut->GetYaxis()->SetTitle("d0_{Y} , mm");
    hist15_cut->GetXaxis()->SetTitle("mm");
    hist16_cut->GetXaxis()->SetTitle("mm");
    hist17_cut->GetYaxis()->SetTitle("#Theta^{in}_{Y} , #murad");
    hist17_cut->GetXaxis()->SetTitle("#Theta^{in}_{X} , #murad");
    hist18_cut->GetYaxis()->SetTitle("#Theta^{in}_{X} , #murad");
    hist18_cut->GetXaxis()->SetTitle("d0_{X} , mm");
    hist19_cut->GetYaxis()->SetTitle("#Theta^{in}_{X} , #murad");
    hist19_cut->GetXaxis()->SetTitle("d0_{Y} , mm");
    hist20_cut->GetYaxis()->SetTitle("#Theta^{in}_{Y} , #murad");
    hist20_cut->GetXaxis()->SetTitle("d0_{X} , mm");
    hist21_cut->GetYaxis()->SetTitle("#Theta^{in}_{Y} , #murad");
    hist21_cut->GetXaxis()->SetTitle("d0_{Y} , mm");

    tree->GetEvent(0);
    printf("--> gPos.x: %14.3f [urad]\n",gPos.x);
    for (Int_t i = 0; i < nentries; i++)
    {
        tree->GetEvent(i);

        // single track
        if(isTrack == 1  && !isHit)
        {
            // without cuts
            hist1->Fill(INx*1e6);
            hist2->Fill(OUTx*1e6);
            hist3->Fill(INy*1e6);
            hist4->Fill(OUTy*1e6);
            hist5->Fill((OUTx-INx)*1e6);
            hist6->Fill((OUTy-INy)*1e6);
            hist7->Fill((OUTx-INx)*1e6,(OUTy-INy)*1e6);
            hist8->Fill(INx*1e6,(OUTx-INx)*1e6);
            hist9->Fill(INy*1e6,(OUTy-INy)*1e6);
            hist10->Fill(IMPx,(OUTx-INx)*1e6);
            hist11->Fill(IMPy,(OUTx-INx)*1e6);
            hist12->Fill(IMPx,(OUTy-INy)*1e6);
            hist13->Fill(IMPy,(OUTy-INy)*1e6);
            hist14->Fill(IMPx,IMPy);
            hist15->Fill(IMPx);
            hist16->Fill(IMPy);
            hist17->Fill(INx*1e6,INy*1e6);
            hist18->Fill(IMPx,INx*1e6);
            hist19->Fill(IMPy,INx*1e6);
            hist20->Fill(IMPx,INy*1e6);
            hist21->Fill(IMPy,INy*1e6);

            // with cuts
            cut_d0x = 0;
            cut_d0y = 0;

            if(IMPy < max_d0y && IMPy > min_d0y) cut_d0y = 1;
            if(IMPx < max_d0x && IMPx > min_d0x) cut_d0x = 1;

            if(cut_d0x && cut_d0y)
            {
                hist1_cut->Fill(INx*1e6);
                hist2_cut->Fill(OUTx*1e6);
                hist3_cut->Fill(INy*1e6);
                hist4_cut->Fill(OUTy*1e6);
                hist5_cut->Fill((OUTx-INx)*1e6);
                hist6_cut->Fill((OUTy-INy)*1e6);
                hist7_cut->Fill((OUTx-INx)*1e6,(OUTy-INy)*1e6);
                hist8_cut->Fill(INx*1e6,(OUTx-INx)*1e6);
                hist9_cut->Fill(INy*1e6,(OUTy-INy)*1e6);
                hist10_cut->Fill(IMPx,(OUTx-INx)*1e6);
                hist11_cut->Fill(IMPy,(OUTx-INx)*1e6);
                hist12_cut->Fill(IMPx,(OUTy-INy)*1e6);
                hist13_cut->Fill(IMPy,(OUTy-INy)*1e6);
                hist14_cut->Fill(IMPx,IMPy);
                hist15_cut->Fill(IMPx);
                hist16_cut->Fill(IMPy);
                hist17_cut->Fill(INx*1e6,INy*1e6);
                hist18_cut->Fill(IMPx,INx*1e6);
                hist19_cut->Fill(IMPy,INx*1e6);
                hist20_cut->Fill(IMPx,INy*1e6);
                hist21_cut->Fill(IMPy,INy*1e6);
            }
        }
        else if(isHit == 1 && !isTrack)
        {
            // without cuts
            hist1->Fill(INx_multi*1e6);
            hist3->Fill(INy_multi*1e6);
            hist14->Fill(IMPx_multi,IMPy);
            hist15->Fill(IMPx_multi);
            hist16->Fill(IMPy_multi);
            hist17->Fill(INx_multi*1e6,INy_multi*1e6);
            hist18->Fill(IMPx_multi,INx_multi*1e6);
            hist19->Fill(IMPy_multi,INx_multi*1e6);
            hist20->Fill(IMPx_multi,INy_multi*1e6);
            hist21->Fill(IMPy_multi,INy_multi*1e6);

            // with cuts
            cut_d0x = 0;
            cut_d0y = 0;

            if(IMPy_multi < max_d0y && IMPy_multi > min_d0y) cut_d0y = 1;
            if(IMPx_multi < max_d0x && IMPx_multi > min_d0x) cut_d0x = 1;

            if(cut_d0x && cut_d0y)
            {
                hist1_cut->Fill(INx_multi*1e6);
                hist3_cut->Fill(INy_multi*1e6);
                hist14_cut->Fill(IMPx_multi,IMPy_multi);
                hist15_cut->Fill(IMPx_multi);
                hist16_cut->Fill(IMPy_multi);
                hist17_cut->Fill(INx_multi*1e6,INy_multi*1e6);
                hist18_cut->Fill(IMPx_multi,INx_multi*1e6);
                hist19_cut->Fill(IMPy_multi,INx_multi*1e6);
                hist20_cut->Fill(IMPx_multi,INy_multi*1e6);
                hist21_cut->Fill(IMPy_multi,INy_multi*1e6);
            }
        }
        if(i%10000 == 0)
        {
            printf("\r--> Working: %3.1f %% | Delta_gPos.x: %14.3f [urad]", 100*(Double_t)i/nentries,gPos.x-initial_gpos_x);
            fflush(stdout);
        }
    }
    printf("\nDone!\n");
}

void data_ana::write_histo_to_file(TString outFileName)
{
    cout<<"--> Writing output data to file <<"<<outFileName<<">>"<<endl;

    TFile *outfile = new TFile(outFileName,"recreate");

    hist1->Write();
    hist2->Write();
    hist3->Write();
    hist4->Write();
    hist5->Write();
    hist6->Write();
    hist7->Write();
    hist8->Write();
    hist9->Write();
    hist10->Write();
    hist11->Write();
    hist12->Write();
    hist13->Write();
    hist14->Write();
    hist15->Write();
    hist16->Write();
    hist17->Write();
    hist18->Write();
    hist19->Write();
    hist20->Write();
    hist21->Write();

    hist1_cut->Write();
    hist2_cut->Write();
    hist3_cut->Write();
    hist4_cut->Write();
    hist5_cut->Write();
    hist6_cut->Write();
    hist7_cut->Write();
    hist8_cut->Write();
    hist9_cut->Write();
    hist10_cut->Write();
    hist11_cut->Write();
    hist12_cut->Write();
    hist13_cut->Write();
    hist14_cut->Write();
    hist15_cut->Write();
    hist16_cut->Write();
    hist17_cut->Write();
    hist18_cut->Write();
    hist19_cut->Write();
    hist20_cut->Write();
    hist21_cut->Write();    

    outfile->Close();
}

void data_ana::get_torsion_efficiency_optimization(TString outFileName)
{
    cout<<endl<<"--> Get Torsion & Efficiency & Optimization <--"<<endl;

    // STEP 1
    // slice width in Y
    Double_t dSlice = 0.2; // [mm]
    // number of slices in Y
    const Int_t nSlice = (max_d0y - min_d0y)/dSlice;
    // number of bins
    const Int_t nBins = 80;
    Double_t minThetaIN = -200.0;
    Double_t maxThetaIN =  200.0;
    // number of rms for Theta_X^IN
    const Double_t nRMS = 2.0;

    TH2D* histo[nSlice];
    TH1D* hist_dThetaX = new TH1D("hist_dThetaX","#Delta#Theta_{X}",nBins,minThetaIN,maxThetaIN);
    TH1D* hist_ThetaX = new TH1D("hist_ThetaX","#Theta_{X}",nBins,minThetaIN,maxThetaIN);

    TString name, title;
    for(Int_t i = 0; i < nSlice; i++)
    {
        name = "histo_";
        name += i;
        title = "Y slice (";
        title += (Int_t)(min_d0y*1000.0 + i*dSlice*1000.0);
        title += " | ";
        title += (Int_t)(min_d0y*1000.0 + (i+1)*dSlice*1000.0);
        title += ") um";
        histo[i] = new TH2D(name.Data(),title.Data(),nBins,minThetaIN,maxThetaIN,nBins,minThetaIN,maxThetaIN);
    }

    // STEP 2
    for (Int_t i = 0; i < nentries; i++)
    {
        tree->GetEvent(i);

        // single track
        if(isTrack == 1  && !isHit)
        {
            // with cuts
            cut_d0x = 0;
            cut_d0y = 0;

            if(IMPy < max_d0y && IMPy > min_d0y) cut_d0y = 1;
            if(IMPx < max_d0x && IMPx > min_d0x) cut_d0x = 1;

            if(cut_d0x && cut_d0y)
            {
                for(Int_t i = 0; i < nSlice; i++)
                {
                    if(IMPy > min_d0y + i*dSlice && IMPy < min_d0y + (i+1)*dSlice)
                    {
                        histo[i]->Fill(INx*1e6,(OUTx-INx)*1e6);
                    }
                }
                hist_dThetaX->Fill((OUTx-INx)*1e6);
                hist_ThetaX->Fill(INx*1e6);
            }
        }

        if(i%10000 == 0)
        {
            printf("\r--> Working: %3.1f %% | Delta_gPos.x: %14.3f [urad]", 100*(Double_t)i/nentries,gPos.x - initial_gpos_x);
            fflush(stdout);
        }
    }
    cout<<endl;

    // STEP 3
    TFile *outfile = new TFile(outFileName.Data(),"recreate");

    for(Int_t i = 0; i < nSlice; i++)
    {
        histo[i]->Write();
    }
    hist_dThetaX->Write();
    hist_ThetaX->Write();

    // STEP 4
    cout<<"### STEP 4 ###"<<endl;

    Double_t ThetaXrms = hist_ThetaX->GetRMS();
    Double_t ThetaXmean = hist_ThetaX->GetMean();

    cout<<"--> ThetaXmean = "<<ThetaXmean<<" +/- "<<ThetaXrms<<" [urad]"<<endl;

    TCanvas* c_0 = new TCanvas("c_0","c_0",1000,1000);
    c_0->cd();
    hist_dThetaX->Draw("hist");
    Double_t par[6];


    //----------------------------------------------------------------------//
    //----------------------------------------------------------------------//
    // Fit Ranges                                                           //
    TF1* fit_1 = new TF1("fit_1","gaus",-100,0);                            // non-CH peak
    TF1* fit_2 = new TF1("fit_2","gaus",35,100);                            // CH peak
    TF1* fit_3 = new TF1("fit_3","gaus(0)+gaus(3)",-100,100);               // Full
    //----------------------------------------------------------------------//
    //----------------------------------------------------------------------//


    hist_dThetaX->Fit(fit_1,"R+");
    hist_dThetaX->Fit(fit_2,"R+");
    fit_1->SetLineColor(kCyan+2);
    fit_2->SetLineColor(kGreen+2);
    fit_1->Draw("same");
    fit_2->Draw("same");
    fit_1->GetParameters(&par[0]);
    fit_2->GetParameters(&par[3]);

    fit_3->SetParameters(par);
    hist_dThetaX->Fit(fit_3,"R+");
    fit_3->SetLineColor(kBlack);
    fit_3->Draw("same");
    fit_3->GetParameters(&par[0]);

    TF1* fit_am = new TF1("fit_am","gaus",minThetaIN,maxThetaIN);
    TF1* fit_ch = new TF1("fit_ch","gaus",minThetaIN,maxThetaIN);
    fit_am->SetParameters(&par[0]);
    fit_ch->SetParameters(&par[3]);
    fit_am->SetLineColor(kMagenta);
    fit_ch->SetLineColor(kRed);
    fit_am->Draw("same");
    fit_ch->Draw("same");

    c_0->Write();

    TCanvas* c_1[nSlice*nBins];
    TCanvas* c_1_1[nSlice];

    TGraphErrors* gr_ndf[nSlice];
    TGraphErrors* gr_ndf_fraction = new TGraphErrors();
    gr_ndf_fraction->SetName("gr_ndf_fraction");
    gr_ndf_fraction->SetTitle("gr_ndf_fraction");

    Int_t gr_iterator = 0;

    Int_t ij = 0;

    cout<<endl;
    TH2D* histo_eff_incAngle_incPosition = new TH2D("histo_eff_incAngle_incPosition","histo_eff_incAngle_incPosition",
                                                    nSlice,min_d0y,max_d0y,
                                                    nBins,minThetaIN,maxThetaIN);
    TH1D* histo_torsion[nSlice];

    for(Int_t i = 0; i < nSlice; i++)
    {
        gr_iterator = 0;

        gr_ndf[i] = new TGraphErrors();
        name = "gr_ndf_";
        name += i;
        gr_ndf[i]->SetName(name.Data());
        gr_ndf[i]->SetTitle(name.Data());

        name = "histo_torsion_";
        name += i;
        histo_torsion[i] = new TH1D(name.Data(),name.Data(),nBins,minThetaIN,maxThetaIN);

        for(Int_t j = 0; j < nBins; j++)
        {
            Int_t binID = j+1;

            if(histo[i]->GetXaxis()->GetBinCenter(binID) < (ThetaXmean - nRMS*ThetaXrms) ||
                    histo[i]->GetXaxis()->GetBinCenter(binID) > (ThetaXmean + nRMS*ThetaXrms)) continue;

            name = "c_";
            name += i;
            name += "_";
            name += binID;

            TH1D* projectionY = (TH1D*)histo[i]->ProjectionY("py",binID,binID);

            c_1[ij] = new TCanvas(name.Data(),name.Data(),1000,1000);
            c_1[ij]->cd();
            projectionY->Draw("hist");
            c_1[ij]->Write();
            ij++;

            Double_t integral_total, integral_total_err, integral_ndf, integral_ndf_err, angle, angle_err;

            angle = histo[i]->GetXaxis()->GetBinCenter(binID);
            angle_err = histo[i]->GetXaxis()->GetBinWidth(binID)/TMath::Sqrt(12.0);
            integral_total = projectionY->IntegralAndError(1,projectionY->GetNbinsX(),integral_total_err);

            // NotDeflected
            integral_ndf = projectionY->IntegralAndError(projectionY->GetXaxis()->FindBin(par[1] - 5.0*par[2]),
                    projectionY->GetXaxis()->FindBin(par[1] + 1.0*par[2]),
                    integral_ndf_err);

            Double_t ratio = 1.0 - integral_ndf/integral_total;
            Double_t ratio_err = TMath::Sqrt(TMath::Power(integral_ndf_err/integral_total,2) +
                                             TMath::Power(integral_ndf*integral_total_err/(integral_total*integral_total),2));

            gr_ndf[i]->SetPoint(gr_iterator,angle,ratio);
            gr_ndf[i]->SetPointError(gr_iterator,angle_err,ratio_err);

            histo_torsion[i]->SetBinContent(binID,ratio);
            histo_torsion[i]->SetBinError(binID,ratio_err);

            histo_eff_incAngle_incPosition->Fill((min_d0y + (i+0.5)*dSlice),angle,ratio);

            gr_iterator++;

            projectionY->Delete();
        }

        //---------------------------------------------//
        Double_t mean_val_pos = -999, mean_val_pos_err = -999;

//        Method I
//        mean_val_pos        = histo_torsion[i]->GetMean();
//        mean_val_pos_err    = histo_torsion[i]->GetMeanError();

//        Method II
//        mean_val_pos        = histo_torsion[i]->GetBinCenter(histo_torsion[i]->GetMaximumBin());
//        mean_val_pos_err    = histo_torsion[i]->GetBinWidth(histo_torsion[i]->GetMaximumBin())/TMath::Sqrt(12.0);

//        Method III
        TF1* fit_torsion_temp = new TF1("fit_torsion_temp","gaus",
                                        histo_torsion[i]->GetMean()-histo_torsion[i]->GetRMS(),
                                        histo_torsion[i]->GetMean()+histo_torsion[i]->GetRMS());
        histo_torsion[i]->Fit(fit_torsion_temp,"R0Q");
        mean_val_pos = fit_torsion_temp->GetParameter(1);
        mean_val_pos_err = fit_torsion_temp->GetParError(1);

        name = "c_";
        name += 1000+i;

        c_1_1[i] = new TCanvas(name.Data(),name.Data(),1000,1000);
        c_1_1[i]->cd();
        histo_torsion[i]->Draw();
        fit_torsion_temp->Draw("same");
        c_1_1[i]->Write();

        delete fit_torsion_temp;
        //---------------------------------------------//

        gr_ndf_fraction->SetPoint(i,(min_d0y + (i+0.5)*dSlice),mean_val_pos);
        gr_ndf_fraction->SetPointError(i,dSlice/TMath::Sqrt(12.0),mean_val_pos_err);

        printf("\r--> Working: %3.1f %%", 100*(Double_t)i/nSlice);
        fflush(stdout);
    }
    cout<<endl;
    for(Int_t i = 0; i < nSlice; i++)
    {
        gr_ndf[i]->Write();
        histo_torsion[i]->Write();
    }
    gr_ndf_fraction->Write();
    histo_eff_incAngle_incPosition->Write();

    // STEP 5
    cout<<"### STEP 5 ###"<<endl;
    cout<<"--> Torsion <--"<<endl;

    TCanvas* c_2 = new TCanvas("c_2","c_2",1000,1000);
    c_2->cd();
    gr_ndf_fraction->Draw("APL");
    TF1* torsion_function = new TF1("torsion_function","pol1",min_d0y,max_d0y);
    gr_ndf_fraction->Fit(torsion_function,"R+");
    c_2->Write();

    // STEP 6
    cout<<"### STEP 6 ###"<<endl;
    TH1D* hist_dThetaX_corr = new TH1D("hist_dThetaX_corr","#Delta#Theta_{X} Corr. (cut for #Theta_{X})",nBins,minThetaIN,maxThetaIN);
    TH2D* hist_dThetaX_ThetaX_corr = new TH2D("hist_dThetaX_ThetaX_corr","#Delta#Theta_{X} vs #Theta_{X} Corr.",nBins,minThetaIN,maxThetaIN,nBins,minThetaIN,maxThetaIN);
    TH2D* hist_dThetaX_IMPy_corr = new TH2D("hist_dThetaX_IMPy_corr","#Delta#Theta_{X} vs d0_{Y} Corr. (cut for #Theta_{X})",nSlice*2,min_d0y*2,max_d0y*2,nBins,minThetaIN,maxThetaIN);

    for (Int_t i = 0; i < nentries; i++)
    {
        tree->GetEvent(i);

        // single track
        if(isTrack == 1  && !isHit)
        {
            // with cuts
            cut_d0x = 0;
            cut_d0y = 0;

            if(IMPy < max_d0y && IMPy > min_d0y) cut_d0y = 1;
            if(IMPx < max_d0x && IMPx > min_d0x) cut_d0x = 1;

            if(cut_d0x && cut_d0y)
            {
                Double_t INx_corr = INx*1e6 - (torsion_function->GetParameter(1)*IMPy);
                hist_dThetaX_ThetaX_corr->Fill(INx_corr,(OUTx - INx)*1e6);
                hist_dThetaX_corr->Fill((OUTx - INx)*1e6);
                hist_dThetaX_IMPy_corr->Fill(IMPy,(OUTx - INx)*1e6);
            }
        }

        if(i%10000 == 0)
        {
            printf("\r--> Working: %3.1f %% | Delta_gPos.x: %14.3f [urad]", 100*(Double_t)i/nentries,gPos.x - initial_gpos_x);
            fflush(stdout);
        }
    }

    cout<<endl;
    hist_dThetaX_corr->Write();
    hist_dThetaX_IMPy_corr->Write();
    hist_dThetaX_ThetaX_corr->Write();

    // STEP 7
    cout<<"### STEP 7 ###"<<endl;
    cout<<"--> Angular Optimisation <--"<<endl;

    TCanvas* c_corr_1[nBins];
    ij = 0;

    fit_3->SetParameters(par);
    hist_dThetaX_corr->Fit(fit_3,"R+");
    fit_3->GetParameters(&par[0]);

    Double_t max_value = 0;
    TGraphErrors* gr_opt_ang = new TGraphErrors();
    gr_opt_ang->SetName("gr_opt_ang");
    gr_opt_ang->SetTitle("gr_opt_ang");

    for(Int_t j = 0; j < nBins; j++)
    {
        Int_t binID = j+1;
        if(hist_dThetaX_ThetaX_corr->GetXaxis()->GetBinCenter(binID) < (ThetaXmean - nRMS*ThetaXrms) ||
                hist_dThetaX_ThetaX_corr->GetXaxis()->GetBinCenter(binID) > (ThetaXmean + nRMS*ThetaXrms)) continue;

        name = "c_corr_";
        name += ij;
        name += "_";
        name += binID;

        TH1D* projectionY = (TH1D*)hist_dThetaX_ThetaX_corr->ProjectionY("py",binID,binID);

        c_corr_1[ij] = new TCanvas(name.Data(),name.Data(),1000,1000);
        c_corr_1[ij]->cd();
        projectionY->Draw("hist");
        c_corr_1[ij]->Write();

        Double_t integral_total, integral_total_err, integral_ndf, integral_ndf_err, angle, angle_err;

        angle = hist_dThetaX_ThetaX_corr->GetXaxis()->GetBinCenter(binID);
        angle_err = hist_dThetaX_ThetaX_corr->GetXaxis()->GetBinWidth(binID)/TMath::Sqrt(12.0);
        integral_total = projectionY->IntegralAndError(1,projectionY->GetNbinsX(),integral_total_err);

        // NotDeflected
        integral_ndf = projectionY->IntegralAndError(projectionY->GetXaxis()->FindBin(par[1] - 5.0*par[2]),
                projectionY->GetXaxis()->FindBin(par[1] + 1.0*par[2]),
                integral_ndf_err);

        Double_t ratio = 1.0 - integral_ndf/integral_total;
        Double_t ratio_err = TMath::Sqrt(TMath::Power(integral_ndf_err/integral_total,2) +
                                         TMath::Power(integral_ndf*integral_total_err/(integral_total*integral_total),2));

        if(ratio > max_value)
        {
            max_value = ratio;
        }

        gr_opt_ang->SetPoint(ij,angle,ratio);
        gr_opt_ang->SetPointError(ij,angle_err,ratio_err);

        projectionY->Delete();

        ij++;
    }
    cout<<endl;
    gr_opt_ang->Write();

    TCanvas* c_4 = new TCanvas("c_4","c_4",1000,1000);
    c_4->cd();
    gr_opt_ang->Draw("APL");
    TF1* fit_opt = new TF1("fit_opt","gaus",gr_opt_ang->GetMean()-gr_opt_ang->GetRMS(),gr_opt_ang->GetMean()+gr_opt_ang->GetRMS());
    gr_opt_ang->Fit(fit_opt,"R+");
    fit_opt->Draw("same");
    c_4->Write();

    Double_t optimalAngle = fit_opt->GetParameter(1);
    Double_t optimalAngle_err = fit_opt->GetParError(1);
    cout<<"--> Optimal Angle: "<<optimalAngle<<" +/- "<<optimalAngle_err<<endl;

    // STEP 8
    cout<<"### STEP 8 ###"<<endl;

    TH1D* hist_dThetaX_corr_cut = new TH1D("hist_dThetaX_corr_cut","#Delta#Theta_{X} Corr. (cut for #Theta_{X})",nBins,minThetaIN,maxThetaIN);
    TH2D* hist_dThetaX_ThetaX_corr_cut = new TH2D("hist_dThetaX_ThetaX_corr_cut","#Delta#Theta_{X} vs #Theta_{X} Corr.",nBins,minThetaIN,maxThetaIN,nBins,minThetaIN,maxThetaIN);
    TH2D* hist_dThetaX_ThetaX_corr_ncut = new TH2D("hist_dThetaX_ThetaX_corr_ncut","#Delta#Theta_{X} vs #Theta_{X} Corr.",nBins,minThetaIN,maxThetaIN,nBins,minThetaIN,maxThetaIN);
    TH2D* hist_dThetaX_IMPy_corr_cut = new TH2D("hist_dThetaX_IMPy_corr_cut","#Delta#Theta_{X} vs d0_{Y} Corr. (cut for #Theta_{X})",nSlice*2,min_d0y*2,max_d0y*2,nBins,minThetaIN,maxThetaIN);

    for (Int_t i = 0; i < nentries; i++)
    {
        tree->GetEvent(i);

        // single track
        if(isTrack == 1  && !isHit)
        {
            // with cuts
            cut_d0x = 0;
            cut_d0y = 0;

            if(IMPy < max_d0y && IMPy > min_d0y) cut_d0y = 1;
            if(IMPx < max_d0x && IMPx > min_d0x) cut_d0x = 1;

            if(cut_d0x && cut_d0y)
            {
                Double_t INx_corr = INx*1e6 - (torsion_function->GetParameter(1)*IMPy) - optimalAngle;

                hist_dThetaX_ThetaX_corr_ncut->Fill(INx_corr,(OUTx - INx)*1e6);
                if(TMath::Abs(INx_corr) < angular_cut)
                {
                    hist_dThetaX_ThetaX_corr_cut->Fill(INx_corr,(OUTx - INx)*1e6);
                    hist_dThetaX_corr_cut->Fill((OUTx - INx)*1e6);
                    hist_dThetaX_IMPy_corr_cut->Fill(IMPy,(OUTx - INx)*1e6);
                }
            }
        }

        if(i%10000 == 0)
        {
            printf("\r--> Working: %3.1f %% | Delta_gPos.x: %14.3f [urad]", 100*(Double_t)i/nentries,gPos.x - initial_gpos_x);
            fflush(stdout);
        }
    }

    cout<<endl;
    hist_dThetaX_ThetaX_corr_cut->Write();
    hist_dThetaX_ThetaX_corr_ncut->Write();
    hist_dThetaX_corr_cut->Write();
    hist_dThetaX_IMPy_corr_cut->Write();

    TCanvas* c_3 = new TCanvas("c_3","c_3",1000,1000);
    c_3->cd();
    hist_dThetaX_corr_cut->GetXaxis()->SetTitle("#Delta#Theta_{X} [#murad]");
    hist_dThetaX_corr_cut->GetXaxis()->CenterTitle();
    hist_dThetaX_corr_cut->GetYaxis()->SetTitle("Counts");
    hist_dThetaX_corr_cut->GetYaxis()->CenterTitle();
    hist_dThetaX_corr_cut->SetLineWidth(2);
    hist_dThetaX_corr_cut->Draw("hist");


    fit_1->SetParameters(&par[0]);
    fit_2->SetParameters(&par[3]);
    hist_dThetaX_corr_cut->Fit(fit_1,"R+");
    hist_dThetaX_corr_cut->Fit(fit_2,"R+");
    fit_1->SetLineColor(kRed);
    fit_2->SetLineColor(kRed);
    fit_1->Draw("same");
    fit_2->Draw("same");
    fit_1->GetParameters(&par[0]);
    fit_2->GetParameters(&par[3]);

//    fit_3->SetParameters(par);
//    hist_dThetaX_corr_cut->Fit(fit_3,"R+");
//    fit_3->SetLineColor(kBlack);
//    fit_3->SetLineWidth(4);
//    fit_3->Draw("same");
//    fit_3->GetParameters(&par[0]);

    fit_am->SetParameters(&par[0]);
    fit_ch->SetParameters(&par[3]);
    fit_am->SetLineColor(kMagenta);
    fit_ch->SetLineColor(kCyan+2);
    fit_am->SetLineWidth(3);
    fit_ch->SetLineWidth(3);
    fit_am->Draw("same");
    fit_ch->Draw("same");

    TLine* line_am = new TLine(par[1],0,par[1],hist_dThetaX_corr_cut->GetMaximum());
    TLine* line_ch = new TLine(par[4],0,par[4],hist_dThetaX_corr_cut->GetMaximum());
    TLine* line_ch_r_lim = new TLine(par[4]+3.0*par[5],0,par[4]+3.0*par[5],hist_dThetaX_corr_cut->GetMaximum());
    TLine* line_ch_l_lim = new TLine(par[4]-3.0*par[5],0,par[4]-3.0*par[5],hist_dThetaX_corr_cut->GetMaximum());
    line_am->SetLineStyle(7);
    line_ch->SetLineStyle(7);
    line_ch_r_lim->SetLineStyle(7);
    line_ch_l_lim->SetLineStyle(7);
    line_am->SetLineWidth(2);
    line_ch->SetLineWidth(2);
    line_ch_r_lim->SetLineWidth(2);
    line_ch_l_lim->SetLineWidth(2);
    line_am->SetLineColor(kBlack);
    line_ch->SetLineColor(kBlack);
    line_ch_r_lim->SetLineColor(kBlack);
    line_ch_l_lim->SetLineColor(kBlack);
    line_am->Draw("same");
    line_ch->Draw("same");
    line_ch_r_lim->Draw("same");
    line_ch_l_lim->Draw("same");
    c_3->Write();

    Double_t integral_ch_cut, integral_ch_cut_err;
    Double_t integral_tot_cut, integral_tot_cut_err;

    integral_tot_cut = hist_dThetaX_corr_cut->IntegralAndError(1,hist_dThetaX_corr_cut->GetNbinsX(),integral_tot_cut_err);

    //------------------------------------------------//
    // Method I
//    integral_ch_cut = hist_dThetaX_corr_cut->IntegralAndError(hist_dThetaX_corr_cut->GetXaxis()->FindBin(par[4]),
//            hist_dThetaX_corr_cut->GetXaxis()->FindBin(par[4]+3.0*par[5]),integral_ch_cut_err);
//    integral_ch_cut *= 2.0;
//    integral_ch_cut_err *= 2.0;

    // Method II
    integral_ch_cut = hist_dThetaX_corr_cut->IntegralAndError(hist_dThetaX_corr_cut->GetXaxis()->FindBin(par[4]-3.0*par[5]),
            hist_dThetaX_corr_cut->GetXaxis()->FindBin(par[4]+3.0*par[5]),integral_ch_cut_err);
    //------------------------------------------------//

    Double_t ch_eff = integral_ch_cut/integral_tot_cut;
    Double_t ch_eff_err = TMath::Sqrt(TMath::Power(integral_ch_cut_err/integral_tot_cut,2) +
                                      TMath::Power(integral_ch_cut*integral_tot_cut_err/(integral_tot_cut*integral_tot_cut),2));

    cout<<endl<<endl;
    cout<<"######################################################"<<endl;
    printf("## Channeling Efficiency: \t %.3f +/- %.2e \n",ch_eff,ch_eff_err);
    printf("## Angular Cut: \t\t %.2f [urad] \n",angular_cut);
    cout<<"######################################################"<<endl;
    cout<<endl<<endl;

    cout<<endl<<endl;
    cout<<"######################################################"<<endl;
    cout<<"## SET THESE PARAMETERS INTO A PARAMETERS.DAT FILE: ##"<<endl;
    printf("## k1 = %.2e \t [rad]\n",optimalAngle*1e-6);
    printf("## k2 = %.2e \t [rad/mm]\n",torsion_function->GetParameter(1)*1e-6);
    cout<<"######################################################"<<endl;
    cout<<endl<<endl;

    // STEP 9
    outfile->Close();
    cout<<"--> Output file: "<<outfile->GetName()<<endl;
}

void data_ana::find_inelastic_counts_fixed(TString outputfilename, Int_t year, Int_t runID)
{
    cout<<endl<<"--> Calculating of the inelastic counts for a fixed position <--"<<endl;

    Int_t* N0               = new Int_t[number_of_cutting_angels];
    Int_t* N12              = new Int_t[number_of_cutting_angels];
    Double_t* theta_cut     = new Double_t[number_of_cutting_angels];

    for(Int_t j = 0; j < number_of_cutting_angels; j++)
    {
        N0[j]   = 0;
        N12[j]  = 0;

        if(number_of_cutting_angels > 1)
            theta_cut[j] = theta_cut_min + j*(theta_cut_max - theta_cut_min)/(number_of_cutting_angels - 1);
        else
            theta_cut[j] = theta_cut_min;
        printf("# ThetaCut[%d] = %.2f [urad]\n",j,theta_cut[j]*1e6);
    }

    TTree * tree = (TTree*)file->Get("simpleEvent");

    tree->GetEvent(0);
    printf("--> gPos.x: %14.3f [urad]\n",gPos.x);

    for (Int_t i = 0; i < nentries; i++)
    {
        cut_d0x = 0;
        cut_d0y = 0;

        Double_t INx_corr = 0.0;

        tree->GetEvent(i);
        // Geometrical cuts
        if(isTrack == 1 && isHit == 0)
        {
            INx_corr = INx - (k2*IMPy + k1);

            if (IMPy < max_d0y && IMPy > min_d0y) cut_d0y = 1;
            if (IMPx < max_d0x && IMPx > min_d0x) cut_d0x = 1;
        }
        else if(isHit == 1)
        {
            INx_corr = INx_multi - (k2*IMPy_multi + k1);

            if (IMPy_multi < max_d0y && IMPy_multi > min_d0y) cut_d0y = 1;
            if (IMPx_multi < max_d0x && IMPx_multi > min_d0x) cut_d0x = 1;
        }
        if(isTrack == 1 || isHit == 1)
        {
            if(cut_d0x == 1 && cut_d0y == 1)
            {
                for(Int_t j = 0; j < number_of_cutting_angels; j++)
                {
                    switch(runID)
                    {
                    case 0:
                        if (TMath::Abs(INx_corr) < theta_cut[j])
                        {
                            N0[j]++;

                            // NOTE: In 2018 we have changed a format of the plastic scintillators data!

                            if(year >= 2015 && year < 2018)
                            {
                                if (evt.nuclear == 3 || evt.nuclear == 6 || evt.nuclear == 9)
                                {
                                    N12[j]++;
                                }
                            }
                            else if(year >= 2018)
                            {
                                Int_t incrm_0 = ((evt.nuclearRaw >> det2bit(signal1)) & 1);
                                Int_t incrm_1 = ((evt.nuclearRaw >> det2bit(signal2)) & 1);
                                if(incrm_0 == 1 && incrm_1 == 1)
                                {
                                    N12[j]++;
                                }
                            }
                            else
                            {
                                cout<<endl<<endl<<">>>>> ERROR: Wrong date ! <<<<<"<<endl<<endl;
                                assert(0);
                            }
                        }
                        break;
                    case 1:
                        N0[j]++;

                        // NOTE: In 2018 we have changed a format of the plastic scintillators data!

                        if(year >= 2015 && year < 2018)
                        {
                            if (evt.nuclear == 3 || evt.nuclear == 6 || evt.nuclear == 9)
                            {
                                N12[j]++;
                            }
                        }
                        else if(year >= 2018)
                        {
                            Int_t incrm_0 = ((evt.nuclearRaw >> det2bit(signal1)) & 1);
                            Int_t incrm_1 = ((evt.nuclearRaw >> det2bit(signal2)) & 1);
                            if(incrm_0 == 1 && incrm_1 == 1)
                            {
                                N12[j]++;
                            }
                        }
                        else
                        {
                            cout<<endl<<endl<<">>>>> ERROR: Wrong date ! <<<<<"<<endl<<endl;
                            assert(0);
                        }
                        break;
                    default:
                        cout<<endl<<">>>>> WARNING: Nothing to do for the given run ID ! <<<<<"<<endl<<endl;
                        break;
                    }
                }
            }
        }
        if(i%10000 == 0)
        {
            printf("\r--> Working: %3.1f %% | Delta_gPos.x: %14.3f [urad]", 100*(Double_t)i/nentries,gPos.x - initial_gpos_x);
            fflush(stdout);
        }
    }
    cout<<endl;

    cout<<"--> Output filename: "<<outputfilename<<endl;
    ofstream outfile(outputfilename.Data());
    for(Int_t i = 0; i < number_of_cutting_angels; i++)
    {
        outfile<<theta_cut[i]<<" , "<<N12[i]<<" , "<<N0[i]<<"\n";
    }
    outfile.close();
}

void data_ana::find_inelastic_counts_scan(TString outputfilename, Int_t year, Int_t runID)
{
    cout<<endl<<"--> Calculating of the inelastic counts for an angular scan <--"<<endl;

    Int_t* N0                   = new Int_t[number_of_cutting_angel_center_position];
    Int_t* N12                  = new Int_t[number_of_cutting_angel_center_position];
    Double_t* theta_cut_center  = new Double_t[number_of_cutting_angel_center_position];

    for(Int_t k = 0; k < number_of_cutting_angel_center_position; k++)
    {
        N0[k]   = 0;
        N12[k]  = 0;

        if(number_of_cutting_angel_center_position > 1)
            theta_cut_center[k] = theta_cut_center_min + k*(theta_cut_center_max - theta_cut_center_min)/(number_of_cutting_angel_center_position - 1);
        else
            theta_cut_center[k] = theta_cut_center_min;

        printf("# ThetaCutCenter[%d] = %.2f [urad]\n",k,theta_cut_center[k]*1e6);
    }

    TTree * tree = (TTree*)file->Get("simpleEvent");

    tree->GetEvent(0);
    printf("--> gPos.x: %14.3f [urad]\n",gPos.x);

    for (Int_t i = 0; i < nentries; i++)
    {
        cut_d0x = 0;
        cut_d0y = 0;

        Double_t INx_corr = 0.0;

        tree->GetEvent(i);
        // Geometrical cuts
        if(isTrack == 1 && isHit == 0)
        {
            INx_corr = INx + (gPos.x - initial_gpos_x)*1e-6 - (k2*IMPy + k1);

            if (IMPy < max_d0y && IMPy > min_d0y) cut_d0y = 1;
            if (IMPx < max_d0x && IMPx > min_d0x) cut_d0x = 1;
        }
        else if(isHit == 1)
        {
            INx_corr = INx_multi + (gPos.x - initial_gpos_x)*1e-6 - (k2*IMPy_multi + k1);

            if (IMPy_multi < max_d0y && IMPy_multi > min_d0y) cut_d0y = 1;
            if (IMPx_multi < max_d0x && IMPx_multi > min_d0x) cut_d0x = 1;
        }
        if(isTrack == 1 || isHit == 1)
        {
            if(cut_d0x == 1 && cut_d0y == 1)
            {
                for(Int_t j = 0; j < number_of_cutting_angel_center_position; j++)
                {
                    switch(runID)
                    {
                    case 0:
                        if(INx_corr <= (theta_cut_center[j] + theta_cut_fixed) && INx_corr >= (theta_cut_center[j] - theta_cut_fixed))
                        {
                            N0[j]++;

                            // NOTE: In 2018 we have changed a format of the plastic scintillators data!

                            if(year >= 2015 && year < 2018)
                            {
                                if (evt.nuclear == 3 || evt.nuclear == 6 || evt.nuclear == 9)
                                {
                                    N12[j]++;
                                }
                            }
                            else if(year >= 2018)
                            {
                                Int_t incrm_0 = ((evt.nuclearRaw >> det2bit(signal1)) & 1);
                                Int_t incrm_1 = ((evt.nuclearRaw >> det2bit(signal2)) & 1);
                                if(incrm_0 == 1 && incrm_1 == 1)
                                {
                                    N12[j]++;
                                }
                            }
                            else
                            {
                                cout<<endl<<endl<<">>>>> ERROR: Wrong date ! <<<<<"<<endl<<endl;
                                assert(0);
                            }
                        }
                        break;
                    case 1:
                        N0[j]++;

                        // NOTE: In 2018 we have changed a format of the plastic scintillators data!

                        if(year >= 2015 && year < 2018)
                        {
                            if (evt.nuclear == 3 || evt.nuclear == 6 || evt.nuclear == 9)
                            {
                                N12[j]++;
                            }
                        }
                        else if(year >= 2018)
                        {
                            Int_t incrm_0 = ((evt.nuclearRaw >> det2bit(signal1)) & 1);
                            Int_t incrm_1 = ((evt.nuclearRaw >> det2bit(signal2)) & 1);
                            if(incrm_0 == 1 && incrm_1 == 1)
                            {
                                N12[j]++;
                            }
                        }
                        else
                        {
                            cout<<endl<<endl<<">>>>> ERROR: Wrong date ! <<<<<"<<endl<<endl;
                            assert(0);
                        }
                        break;
                    default:
                        cout<<endl<<">>>>> WARNING: Nothing to do for the given run ID ! <<<<<"<<endl<<endl;
                        break;
                    }

                }
            }
        }
        if(i%10000 == 0)
        {
            printf("\r--> Working: %3.1f %% | Delta_gPos.x: %14.3f [urad]", 100*(Double_t)i/nentries,gPos.x - initial_gpos_x);
            fflush(stdout);
        }
    }
    cout<<endl;

    cout<<"--> Output filename: "<<outputfilename<<endl;
    ofstream outfile(outputfilename.Data());
    for(Int_t i = 0; i < number_of_cutting_angel_center_position; i++)
    {
        outfile<<theta_cut_center[i]<<" , "<<N12[i]<<" , "<<N0[i]<<"\n";
    }
    outfile.close();
}

void data_ana::GetParameters(string filenamepath, double &_min_d0x, double &_max_d0x, double &_min_d0y, double &_max_d0y, double &_gpos, double &_k1, double &_k2, double &_a, double &_b)
{
    size_t pos = filenamepath.find_last_of('/');
    string filename = filenamepath.substr(pos+1);

    string parameters_filename = "parameters.dat";

    ifstream inputfile(parameters_filename.data());
    TString word;
    bool status_read = false;

    if(inputfile.is_open())
    {
        while(1)
        {
            inputfile>>word;
            if(word == filename)
            {
                status_read = true;
                inputfile>>_min_d0x;
                inputfile>>_max_d0x;
                inputfile>>_min_d0y;
                inputfile>>_max_d0y;
                inputfile>>_gpos;
                inputfile>>_k1;
                inputfile>>_k2;
                inputfile>>_a;
                inputfile>>_b;
                break;
            }

            if(inputfile.eof()) {break;}
        }
        inputfile.close();
    }
    else
    {
        cout<<"--> ERROR: Unable to open the input file: "<<parameters_filename<<endl;
        assert(0);
    }
    if(!status_read) cout<<endl<<endl<<"--> Did not find file '"<<filename<<"' in the filelist '"<<parameters_filename<<"' !!!"<<endl<<endl;
}

int data_ana::bit2det(int bit)
{
    int det = -1;
    switch (bit)
    {
    case 0:
        det = 5;
        break;
    case 1:
        det = 4;
        break;
    case 6:
        det = 3;
        break;
    case 7:
        det = 2;
        break;
    case 4:
        det = 1;
        break;
    case 5:
        det = 0;
        break;
    }
    return det;
}

int data_ana::det2bit(int det)
{
    int bit = -1;
    switch (det)
    {
    case 0:
        bit = 5;
        break;
    case 1:
        bit = 4;
        break;
    case 2:
        bit = 7;
        break;
    case 3:
        bit = 6;
        break;
    case 4:
        bit = 1;
        break;
    case 5:
        bit = 0;
        break;
    }
    return bit;
}

