//  Asymmetry Analysis of RHIC pC Polarimeter
//  Error/Anomaly Finding Routine
//  file name :   AsymErrorDetector.cc
// 
//  Author    :   Itaru Nakagawa
//  Creation  :   08/01/2006
//                

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <iostream.h>
#include "TMinuit.h"
#include "TString.h"
#include "TMath.h"
#include "rhicpol.h"
#include "rpoldata.h"
#include "Asym.h"
#include "WeightedMean.h"
#include "AsymErrorDetector.h"


StructBunchCheck bnchchk;
StructStripCheck strpchk;



//
// Class name  : 
// Method name : int InvariantMassCorrelation()
//
// Description : Check the Mass vs. 12C Kinetic Enegy Correlation 
// Input       : int st
// Return      : 
//
int 
InvariantMassCorrelation(int st){

  char htitle[100],histname[100];

  // Function for Fitting
  TF1 * g1 = new TF1("g1","gaus",5,16);
  TF1 * f1 = new TF1("f1","pol1",200,1000);
  f1->SetLineColor(2);

  // Mass vs. Energy correlation
  sprintf(histname,"mass_vs_e_ecut_st%d",st);
  mass_vs_e_ecut[st]->Write();
  TH2F * hslice = (TH2F*) gDirectory->Get(histname);
  hslice->SetName("hslice");

  // slice histogram for fit
  hslice->FitSlicesY(g1);

  TH1D *hslice_1 = (TH1D*)gDirectory->Get("hslice_1");
  sprintf(histname,"mass_vs_energy_corr_st%d",st);
  hslice_1->SetName(histname);
  sprintf(htitle,"%8.3f:Invariant Mass vs. Energy Correlation Fit (Str%d)",runinfo.RUNID,st+1);
  hslice_1->SetTitle(htitle);
  hslice_1->GetXaxis()->SetTitle("12C Kinetic Energy [keV]");
  hslice_1->GetYaxis()->SetTitle("Invariant Mass [GeV]");
  hslice_1->Fit("f1","Q");

  //Get Fitting results
  for (int j=0;j<2; j++) {
    strpchk.ecorr.p[j][st]    = f1->GetParameter(j);
    strpchk.ecorr.perr[j][st] = strpchk.ecorr.p[j][st] != 0 ? f1->GetParError(j) : 0;
  }

  // Delete unnecessary histograms generated by FitSliceY();
  TH1D *hslice_0 = (TH1D*)gDirectory->Get("hslice_0");
  TH1D *hslice_2 = (TH1D*)gDirectory->Get("hslice_2");
  TH1D *hslice_chi2 = (TH1D*)gDirectory->Get("hslice_chi2");
  hslice_0->Delete(); hslice_2->Delete(); hslice_chi2->Delete();

  // Make graph of p1 paramter as a function of strip number when strip number is the last one
  if (st==NSTRIP-1) {
    float strip[NSTRIP],ex[NSTRIP];
    for (int k=0;k<NSTRIP;k++) {strip[k]=k+1;ex[k]=0;}
    TGraphErrors * ecorr = new TGraphErrors(NSTRIP, strip, strpchk.ecorr.p[1], ex, strpchk.ecorr.perr[1]);
    sprintf(htitle,"Run%8.3f : P[1] distribution for Mass vs. Energy Correlation", runinfo.RUNID);
    ecorr -> SetTitle(htitle);
    ecorr -> GetXaxis()->SetTitle("Strip Number");
    ecorr -> GetYaxis()->SetTitle("slope [GeV/keV]");
    ecorr -> SetMarkerStyle(20);
    ecorr -> SetMarkerColor(2);
    TLine * lp = new TLine(0, strpchk.p1.allowance, NSTRIP+1, strpchk.p1.allowance);
    TLine * ln = new TLine(0, strpchk.p1.allowance, NSTRIP+1, strpchk.p1.allowance);
    lp -> SetLineStyle(2);
    ln -> SetLineStyle(2);
    ecorr -> GetListOfFunctions()->Add(lp);
    ecorr -> GetListOfFunctions()->Add(ln);
    ecorr -> Write("ecorr");
  }

  return 0;

}


//
// Class name  : 
// Method name : StripErrorDetector()
//
// Description : fit banana with kinematic function. This routine is incomplete.
//             : 1. fix hard corded runconst.E2T consntant 1459.43. 
//             : 2. delete hbananan_1 histograms
// Input       : 
// Return      : 
//
void 
BananaFit(int st){

  TF1 * functof = new TF1("functof", "1459.43/sqrt(x)",200,1500);

  char hname[100];
  sprintf(hname,"t_vs_e_st%d",st);
  t_vs_e[st]->GetListOfFunctions()->Add(functof);
  t_vs_e[st]->Write();
  TH2F * hbanana = (TH2F*) gDirectory->Get(hname);
  hbanana->SetName("hbanana");

  // Get centers of banana
  hbanana->FitSlicesY();

  TH1D *hbanana_1 = (TH1D*) gDirectory->Get("hbanana_1");
  sprintf(hname,"banana_center_st%d",st);
  hbanana_1->SetName(hname);

  // Delete unnecessary histograms
  TH1D *hbanana_0    = (TH1D*) gDirectory->Get("hbanana_0");
  TH1D *hbanana_2    = (TH1D*) gDirectory->Get("hbanana_2");
  TH1D *hbanana_chi2 = (TH1D*) gDirectory->Get("hbanana_chi2");
  hbanana_0->Delete();  hbanana_2->Delete();  hbanana_chi2->Delete();

  return;
};



//
// Class name  : 
// Method name : StripErrorDetector()
//
// Description : find suspicious strips
// Input       : 
// Return      : 
//
int
StripAnomalyDetector(){


  int counter=0;
  float sigma=0;
  strpchk.average[0] = WeightedMean(feedback.RMS,feedback.err,NSTRIP);
  //  HHPAK(16320,strpchk.average);  
  
  TF1 *f1 = new TF1("f1","pol1",0,1000);

  strpchk.dev.max  = fabs(feedback.mdev[0]);
  strpchk.chi2.max = feedback.chi2[0];

  for (int i=0; i<NSTRIP; i++) {
    printf("Anomary Check for strip=%d ...\r",i);

    // t vs. Energy (this routine is incomplete)
    //    BananaFit(i);

    // MASS vs. Energy correlation
    InvariantMassCorrelation(i);
    if (!i) strpchk.p1.max   = fabs(strpchk.ecorr.p[1][i]);  // initialize max w/ strip 0
    if (fabs(strpchk.ecorr.p[1][i]) > strpchk.p1.max ) {
      strpchk.p1.max = fabs(strpchk.ecorr.p[1][i]);
      strpchk.p1.st  = i;
    }

    // Maximum devistion of peak from 12C_MASS
    if (fabs(feedback.mdev[i]) > strpchk.dev.max) {
      strpchk.dev.max  = fabs(feedback.mdev[i]);
      strpchk.dev.st   = i;
    }
    // Gaussian Mass fit Largest chi2
    if (feedback.chi2[i] > strpchk.chi2.max) {
      strpchk.chi2.max  = fabs(feedback.chi2[i]);
      strpchk.chi2.st   = i;
    }

    // Calculate one sigma of RMS distribution
    if (feedback.err[i]){
      sigma += (feedback.RMS[i]-strpchk.average[0])*(feedback.RMS[i]-strpchk.average[0])
	/feedback.err[i]/feedback.err[i];
      counter++;

    }

  }
  sigma=sqrt(sigma)/counter; 

  strpchk.p1.allowance   = errdet.MASS_ENERGY_CORR_ALLOWANCE;
  strpchk.dev.allowance  = errdet.MASS_DEV_ALLOWANCE;
  strpchk.chi2.allowance = errdet.MASS_CHI2_ALLOWANCE;
  float devlimit[1]={strpchk.dev.allowance+strpchk.average[0]};
  //  HHPAK(16330,devlimit);  
  float chi2limit[1]={strpchk.chi2.allowance};
  //  HHPAK(16350,chi2limit);

  // register and count suspicious strips 

  anal.anomaly.nstrip=0;
  for (int i=0;i<NSTRIP; i++) {
    if ((fabs(feedback.RMS[i])-strpchk.average[0]>strpchk.dev.allowance)   // large chi2 or deviation of peak position from average 
	||(feedback.chi2[i] > strpchk.chi2.allowance)           // chi2 of Gaussian fit on Inv. Mass peak
	||(fabs(strpchk.ecorr.p[1][i]) > strpchk.p1.allowance)) // mass vs. 12C kinetic energy correlation
      {
	anal.anomaly.st[anal.anomaly.nstrip]=i;
	++anal.anomaly.nstrip;
      }
  }
  
  UnrecognizedAnomaly(anal.anomaly.st,anal.anomaly.nstrip,runinfo.DisableStrip,runinfo.NDisableStrip,
		      anal.unrecog.anomaly.st, anal.unrecog.anomaly.nstrip);


  return 0;

};



//
// Class name  : 
// Method name : UnrecognizedAnomaly(int Mode)
//
// Description : Check whether anomalies are recognized or not.
// Input       : 
// Return      : unrecongnized (strip/bunch) ID in array z, and nz
//
int
UnrecognizedAnomaly(int *x, int nx, int *y, int ny, int *z, int &nz){

  int match[nx];
  for (int i=0;i<nx;i++) match[i]=x[i];

  // Check for mathing between two arrays x[nx], y[ny]
  for (int i=0; i<ny; i++){
    for (int j=0; j<nx; j++){
      if (y[i]==match[j]){ match[j]=-1; break;}
    }
  }

  nz=0;
  for (int i=0;i<nx; i++) {
    if (match[i]!=-1) { z[nz]=match[i] ;nz++ ;}
  }


  return 0;

}



//
// Class name  : 
// Method name : checkForBadBunches()
//
// Description : check for bad bunches
// Input       : 
// Return      : 
//
void checkForBadBunches()
{

  // counter initiariztion
  anal.anomaly.nbunch=0;
  bnchchk.allowance=errdet.BUNCH_ALLOWANCE_SIGMA;


	printf("checking for bad bunches\n");
	
	double avg;
	double sigma;
	for(int i=0;i<NDETECTOR;i++)
	{
		avg=0.;
		for(int j=0;j<120;j++)
		{
			avg+=Ncounts[i][j];
		}
		avg=avg/120.;
		
		sigma=0.;
		for(int j=0;j<120;j++)
		{
			sigma+=((Ncounts[i][j]-avg)*(Ncounts[i][j]-avg));
		}
		sigma=sigma/120.;
		sigma=sqrt(sigma);
		
		for(int j=0;j<120;j++)
		{
			if((Ncounts[i][j]-avg)> bnchchk.allowance*sigma)
			{
			  anal.anomaly.bunch[anal.anomaly.nbunch]=j+1;
			  anal.anomaly.nbunch++;
			  printf("WARNING: bunch # %d has very many counts in detector # %d\n", j+1, i+1);

			}
		}
		
	}


	UnrecognizedAnomaly(anal.anomaly.bunch,anal.anomaly.nbunch,runinfo.DisableBunch,runinfo.NDisableBunch,
			    anal.unrecog.anomaly.bunch, anal.unrecog.anomaly.nbunch);


}


			
