//  Asymmetry Analysis of RHIC pC Polarimeter
//  file name :   AsymROOT.cc
// 
//  Author    :   I. Nakagawa
//  Creation  :   7/11/2006
//                

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include "rhicpol.h"
#include "rpoldata.h"
#include "Asym.h"
#include "AsymROOT.h"

// Direcotories
TDirectory * Kinema;
TDirectory * ErrDet;
TDirectory * Asymmetry;

//
//  Histogram Definitions 
//
//  Number arrays are TOT_WFD_CH, not NSTRIP, because there are events with strip>72,73,74,75
//  in Run06 which are target events. These histograms are deleted before ROOT file closing 
//  anyway though, need to be declared to aviod crash in histogram filling rouitne in process_event()
//
// Kinema Dir
TH2F * t_vs_e[TOT_WFD_CH];          // t vs. 12C Kinetic Energy (banana with/o cut)
TH2F * mass_vs_e_ecut[TOT_WFD_CH];  // Mass vs. 12C Kinetic Energy 
TH2F * mass_vs_t_ecut[TOT_WFD_CH];  // Mass vs. ToF (w/ Energy Cut)
TH2F * mass_vs_t[TOT_WFD_CH];       // Mass vs. ToF (w/o Energy Cut)

// ErrDet dir
TGraphErrors * mass_sigma_vs_strip;         // Mass sigma width vs. strip 
TGraphErrors * mass_chi2_vs_strip;          // Mass sigma width vs. strip 
TGraphErrors * mass_e_correlation_strip;    // Mass-energy correlation vs. strip
TGraph * rate_vs_bunch;                     // Counting rate vs. bunch
TH1F * bunch_rate;                          // Counting rate per bunch hisogram

// Asymmetry dir
TGraphErrors * asym_sinphi_fit;             // strip asymmetry and sin(phi) fit 


//
// Class name  : Root
// Method name : RootFile(char * filename)
//
// Description : Open Root File and define directory structure of histograms
//             : 
// Input       : char *filename
// Return      : 
//
int 
Root::RootFile(char *filename){

  rootfile = new TFile(filename,"RECREATE","ROOT Histogram file");

  // directory structure
  Kinema    = rootfile->mkdir("Kinema");
  ErrDet    = rootfile->mkdir("ErrDet");
  Asymmetry = rootfile->mkdir("Asymmetry");



  return 0;

}



//
// Class name  : Root
// Method name : RootHistBook()
//
// Description : Book ROOT Histograms
//             : 
// Input       : 
// Return      : 
//
int 
Root::RootHistBook(){

  Char_t hname[100], htitle[100];

  Kinema->cd();
  for (int i=0; i<TOT_WFD_CH; i++) {


    sprintf(hname,"t_vs_e_st%d",i);
    sprintf(htitle,"%8.3f : t vs. Kin.Energy Str%d ",runinfo.RUNID, i);
    t_vs_e[i] = new TH2F(hname,htitle, 50, 200, 1500, 100, 20, 90);

    sprintf(hname,"mass_vs_e_ecut_st%d",i);
    sprintf(htitle,"%8.3f : Mass vs. Kin.Energy (Energy Cut) Str%d ",runinfo.RUNID, i);
    mass_vs_e_ecut[i] = new TH2F(hname,htitle, 50, 200, 1000, 200, 6, 18);

    sprintf(hname,"mass_vs_t_ecut_st%d",i);
    sprintf(htitle,"%8.3f : Mass vs. ToF (Energy Cut) Str%d ", runinfo.RUNID, i);
    mass_vs_t_ecut[i] = new TH2F(hname,htitle, 100, 10, 90, 100, 5, 25);

    sprintf(hname,"mass_vs_t_st%d",i);
    sprintf(htitle,"%8.3f : Mass vs. ToF Str%d", runinfo.RUNID, i);
    mass_vs_t[i] = new TH2F(hname,htitle, 100, 10, 90, 100, 5, 25);

  }

  return 0;

}



//
// Class name  : Root
// Method name : DeleteHistogram
//
// Description : Delete Unnecessary Histograms
//             : 
// Input       : 
// Return      : 
//
int 
Root::DeleteHistogram(){

  
  // Delete histograms declared for WFD channel 72 - 75 to avoid crash. These channcles 
  // are for target channels and thus thes histograms wouldn't make any sense.
  for (int i=NSTRIP; i<TOT_WFD_CH; i++ ) {

    t_vs_e[i] -> Delete();
    mass_vs_e_ecut[i] -> Delete();  // Mass vs. 12C Kinetic Energy 
    mass_vs_t_ecut[i] -> Delete();  // Mass vs. ToF (w/ Energy Cut)
    mass_vs_t[i] -> Delete();       // Mass vs. ToF (w/o Energy Cut)

  }

  
  return 0;

}




//
// Class name  : Root
// Method name : RootFile(char * filename)
//
// Description : Write out memory and dump histogram before closing the rootfile
//             : 
// Input       : 
// Return      : 
//
int 
Root::CloseROOTFile(){
  
  
  // Write out memory before closing
  ErrDet->cd();
  //  mass_sigma_vs_strip -> Write("mass_sigma_vs_strip");
  //  mass_chi2_vs_strip -> Write("mass_chi2_vs_strip");
  //  mass_e_correlation_strip -> Write("mass_e_correlation_strip");
  bunch_rate -> Write("bunch_rate");
  rate_vs_bunch -> Write("rate_vs_bunch");

  Asymmetry->cd();
  asym_sinphi_fit -> Write("asym_sinphi_fit");


  rootfile->Write();


  // close rootfile
  rootfile->Close();

  return 0;

}


