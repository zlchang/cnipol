/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2F.h"

#include "utils/utils.h"

#include "MAsymRunHists.h"

//#include "RunInfo.h"
#include "MseRunInfo.h"
#include "ChannelCalib.h"


ClassImp(MAsymRunHists)

using namespace std;

/** Default constructor. */
MAsymRunHists::MAsymRunHists() : DrawObjContainer(),
   fMinFill(14900), fMaxFill(15500),
   //fMinFill(15300), fMaxFill(15400),
   //fMinFill(10000), fMaxFill(11000),
   //fMinFill(10940), fMaxFill(10962),
   fMinTime(0), fMaxTime(UINT_MAX), fHTargetVsRun(), fVTargetVsRun()
{
   BookHists();
}


MAsymRunHists::MAsymRunHists(TDirectory *dir) : DrawObjContainer(dir),
   fMinFill(14900), fMaxFill(15500),
   //fMinFill(15300), fMaxFill(15400),
   //fMinFill(10000), fMaxFill(11000),
   //fMinFill(10940), fMaxFill(10962),
   fMinTime(0), fMaxTime(UINT_MAX), fHTargetVsRun(), fVTargetVsRun()
{
   BookHists();
}


/** Default destructor. */
MAsymRunHists::~MAsymRunHists()
{
   //delete grHTargetVsMeas;
   //delete grVTargetVsMeas;
}


/** */
void MAsymRunHists::BookHists(string sid)
{ //{{{
   fDir->cd();

   for (UInt_t i=0; i!=N_POLARIMETERS; i++) {
      for (IterBeamEnergy iBE=gRunConfig.fBeamEnergies.begin(); iBE!=gRunConfig.fBeamEnergies.end(); ++iBE) {
         BookHistsPolarimeter((EPolarimeterId) i, *iBE);
      }
   }

   char hName[256];

   for (IterBeamEnergy iBE=gRunConfig.fBeamEnergies.begin(); iBE!=gRunConfig.fBeamEnergies.end(); ++iBE)
   {
      string  strBeamE = RunConfig::AsString(*iBE);
      Color_t color    = RunConfig::AsColor(kB1U);

      sprintf(hName, "hPolarBlueRatioVsFill_%s", strBeamE.c_str());
      o[hName] = new TH1F(hName, hName, fMaxFill-fMinFill, fMinFill, fMaxFill);
      ((TH1*) o[hName])->GetYaxis()->SetRangeUser(0, 100);
      ((TH1*) o[hName])->SetTitle(";Fill;Polarization Ratio;");
      ((TH1*) o[hName])->SetMarkerStyle(kFullCircle);
      ((TH1*) o[hName])->SetMarkerSize(1);
      ((TH1*) o[hName])->SetMarkerColor(color);
      ((TH1*) o[hName])->SetOption("E1");

      color    = RunConfig::AsColor(kY2U);

      sprintf(hName, "hPolarYellowRatioVsFill_%s", strBeamE.c_str());
      o[hName] = new TH1F(hName, hName, fMaxFill-fMinFill, fMinFill, fMaxFill);
      ((TH1*) o[hName])->GetYaxis()->SetRangeUser(0, 100);
      ((TH1*) o[hName])->SetTitle(";Fill;Polarization Ratio;");
      ((TH1*) o[hName])->SetMarkerStyle(kFullCircle);
      ((TH1*) o[hName])->SetMarkerSize(1);
      ((TH1*) o[hName])->SetMarkerColor(color);
      ((TH1*) o[hName])->SetOption("E1");
   }

} //}}}


/** */
void MAsymRunHists::BookHistsPolarimeter(EPolarimeterId polId, EBeamEnergy beamE)
{ //{{{
   char hName[256];
   //char hTitle[256];
   string  strPolId = RunConfig::AsString(polId);
   string  strBeamE = RunConfig::AsString(beamE);
   Color_t color    = RunConfig::AsColor(polId);

   // Rate
   sprintf(hName, "grMaxRateVsMeas");
   TGraphErrors *grMaxRateVsMeas = new TGraphErrors();
   grMaxRateVsMeas->SetName(hName);
   grMaxRateVsMeas->SetMarkerStyle(kFullCircle);
   grMaxRateVsMeas->SetMarkerSize(1);
   grMaxRateVsMeas->SetMarkerColor(color);

   sprintf(hName, "hMaxRateVsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH2F(hName, hName, 1, fMinFill, fMaxFill, 1, 0, 100);
   ((TH1*) o[hName])->SetTitle(";Measurement;Max Rate;");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grMaxRateVsMeas, "p");

   // Targets
   sprintf(hName, "grHTargetVsMeas");
   TGraphErrors *grHTargetVsMeas = new TGraphErrors();
   grHTargetVsMeas->SetName(hName);
   //grHTargetVsMeas->SetMarkerStyle(kPlus);
   grHTargetVsMeas->SetMarkerStyle(kFullTriangleUp);
   grHTargetVsMeas->SetMarkerSize(2);
   grHTargetVsMeas->SetMarkerColor(color-3);

   sprintf(hName, "grVTargetVsMeas");
   TGraphErrors *grVTargetVsMeas = new TGraphErrors();
   grVTargetVsMeas->SetName(hName);
   //grVTargetVsMeas->SetMarkerStyle(kMultiply);
   grVTargetVsMeas->SetMarkerStyle(kFullTriangleDown);
   grVTargetVsMeas->SetMarkerSize(2);
   grVTargetVsMeas->SetMarkerColor(color+2);

   sprintf(hName, "hTargetVsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH2F(hName, hName, 1, fMinFill, fMaxFill, 1, 0, 7);
   ((TH1*) o[hName])->SetTitle(";Measurement;Target Id;");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grHTargetVsMeas, "p");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grVTargetVsMeas, "p");

   // Polarization
   TGraphErrors *grPolarVsMeas = new TGraphErrors();
   grPolarVsMeas->SetName("grPolarVsMeas");
   grPolarVsMeas->SetMarkerStyle(kFullCircle);
   grPolarVsMeas->SetMarkerSize(1);
   grPolarVsMeas->SetMarkerColor(color);

   sprintf(hName, "hPolarVsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH2F(hName, hName, 1, fMinFill, fMaxFill, 1, 0, 100);
   ((TH1*) o[hName])->SetTitle(";Measurement;Polarization, %;");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grPolarVsMeas, "p");

   sprintf(hName, "hPolarVsFill_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH1F(hName, hName, fMaxFill-fMinFill, fMinFill, fMaxFill);
   ((TH1*) o[hName])->GetYaxis()->SetRangeUser(0, 100);
   ((TH1*) o[hName])->SetTitle(";Fill;Polarization, %;");
   ((TH1*) o[hName])->SetMarkerStyle(kFullCircle);
   ((TH1*) o[hName])->SetMarkerSize(1);
   ((TH1*) o[hName])->SetMarkerColor(color);
   ((TH1*) o[hName])->SetOption("E1");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grPolarVsMeas, "p");

   // Polarization vs time
   TGraphErrors *grPolarVsTime = new TGraphErrors();
   grPolarVsTime->SetName("grPolarVsTime");
   grPolarVsTime->SetMarkerStyle(kFullCircle);
   grPolarVsTime->SetMarkerSize(1);
   grPolarVsTime->SetMarkerColor(color);

   sprintf(hName, "hPolarVsTime_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH2F(hName, hName, 1, fMinTime, fMaxTime, 1, 0, 100);
   ((TH1*) o[hName])->SetTitle(";Date & Time;Polarization, %;");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grPolarVsTime, "p");
   ((TH1*) o[hName])->GetXaxis()->SetTimeOffset(0, "gmt");
   ((TH1*) o[hName])->GetXaxis()->SetTimeDisplay(1);
   //((TH1*) o[hName])->GetXaxis()->SetTimeFormat("%b %d, %H:%M:%S");
   ((TH1*) o[hName])->GetXaxis()->SetTimeFormat("%H:%M");

   //sprintf(hName, "hPolarVsFill_%s_%s", strPolId.c_str(), strBeamE.c_str());
   //o[hName] = new TH1F(hName, hName, fMaxFill-fMinFill, fMinFill, fMaxFill);
   //((TH1*) o[hName])->GetYaxis()->SetRangeUser(0, 100);
   //((TH1*) o[hName])->SetTitle(";Fill;Polarization, %;");
   //((TH1*) o[hName])->SetMarkerStyle(kFullCircle);
   //((TH1*) o[hName])->SetMarkerSize(1);
   //((TH1*) o[hName])->SetMarkerColor(color);
   //((TH1*) o[hName])->SetOption("E1");
   //((TH1*) o[hName])->GetListOfFunctions()->Add(grPolarVsTime, "p");

   // Profile r
   TGraphErrors *grRVsMeas = new TGraphErrors();
   grRVsMeas->SetName("grRVsMeas");
   grRVsMeas->SetMarkerStyle(kFullCircle);
   grRVsMeas->SetMarkerSize(1);
   grRVsMeas->SetMarkerColor(color);

   TGraphErrors *grRVsMeasH = new TGraphErrors();
   grRVsMeasH->SetName("grRVsMeasH");
   grRVsMeasH->SetMarkerStyle(kFullTriangleUp);
   grRVsMeasH->SetMarkerSize(2);
   grRVsMeasH->SetMarkerColor(color-3);

   TGraphErrors *grRVsMeasV = new TGraphErrors();
   grRVsMeasV->SetName("grRVsMeasV");
   grRVsMeasV->SetMarkerStyle(kFullTriangleDown);
   grRVsMeasV->SetMarkerSize(2);
   grRVsMeasV->SetMarkerColor(color+2);

   sprintf(hName, "hRVsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH2F(hName, hName, 1, fMinFill, fMaxFill, 1, -0.1, 1);
   ((TH1*) o[hName])->SetTitle(";Measurement;r;");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grRVsMeasH, "p");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grRVsMeasV, "p");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grRVsMeas, "p");

   sprintf(hName, "hRVsFill_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH1F(hName, hName, fMaxFill-fMinFill, fMinFill, fMaxFill);
   ((TH1*) o[hName])->GetYaxis()->SetRangeUser(-0.1, 1);
   ((TH1*) o[hName])->SetTitle(";Fill;r;");
   ((TH1*) o[hName])->SetMarkerStyle(kFullCircle);
   ((TH1*) o[hName])->SetMarkerSize(1);
   ((TH1*) o[hName])->SetMarkerColor(color);
   ((TH1*) o[hName])->SetOption("E1");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grRVsMeas, "p");

   sprintf(hName, "hRVsFill_H_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH1F(hName, hName, fMaxFill-fMinFill, fMinFill, fMaxFill);
   ((TH1*) o[hName])->GetYaxis()->SetRangeUser(-0.1, 1);
   ((TH1*) o[hName])->SetTitle(";Fill;r;");
   ((TH1*) o[hName])->SetMarkerStyle(kFullTriangleUp);
   ((TH1*) o[hName])->SetMarkerSize(2);
   ((TH1*) o[hName])->SetMarkerColor(color-3);
   ((TH1*) o[hName])->SetOption("E1");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grRVsMeasH, "p");

   sprintf(hName, "hRVsFill_V_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH1F(hName, hName, fMaxFill-fMinFill, fMinFill, fMaxFill);
   ((TH1*) o[hName])->GetYaxis()->SetRangeUser(-0.1, 1);
   ((TH1*) o[hName])->SetTitle(";Fill;r;");
   ((TH1*) o[hName])->SetMarkerStyle(kFullTriangleDown);
   ((TH1*) o[hName])->SetMarkerSize(2);
   ((TH1*) o[hName])->SetMarkerColor(color+2);
   ((TH1*) o[hName])->SetOption("E1");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grRVsMeasV, "p");

   // t0
   TGraphErrors *grT0VsMeas = new TGraphErrors();
   grT0VsMeas->SetName("grT0VsMeas");
   grT0VsMeas->SetMarkerStyle(kFullCircle);
   grT0VsMeas->SetMarkerSize(1);
   grT0VsMeas->SetMarkerColor(color);

   Int_t t0Lo = -30;
   Int_t t0Hi =  30;

   Int_t dlLo = 30;
   Int_t dlHi = 80;

   //if (beamE == 250) {
   //   if (polId == kB1U || polId == kY2U) { t0Lo = -10; t0Hi = 5; }
   //   if (polId == kB2D || polId == kY1D) { t0Lo = -20; t0Hi = 5; }

   //   //if (polId == kB2D) { dlLo = 40; dlHi = 60; }

   //} else if (beamE == 24) {
   //   if (polId == kB1U || polId == kY2U) { t0Lo = -20; t0Hi = 0; }
   //   if (polId == kB2D || polId == kY1D) { t0Lo = -30; t0Hi = 0; }

   //   //if (polId == kY1D) { dlLo = 60; dlHi = 80; }
   //}

   sprintf(hName, "hT0VsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH2F(hName, hName, 1, fMinFill, fMaxFill, 1, t0Lo, t0Hi);
   ((TH1*) o[hName])->SetTitle(";Measurement;t_{0}, ns;");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grT0VsMeas, "p");

   sprintf(hName, "hT0VsFill_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH1F(hName, hName, fMaxFill-fMinFill, fMinFill, fMaxFill);
   //((TH1*) o[hName])->GetYaxis()->SetRangeUser(t0Lo, t0Hi);
   ((TH1*) o[hName])->SetTitle(";Fill;t_{0}, ns;");
   ((TH1*) o[hName])->SetMarkerStyle(kFullCircle);
   ((TH1*) o[hName])->SetMarkerSize(1);
   ((TH1*) o[hName])->SetMarkerColor(color);
   ((TH1*) o[hName])->SetOption("E1");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grT0VsMeas, "p");

   // DL
   TGraphErrors *grDLVsMeas = new TGraphErrors();
   grDLVsMeas->SetName("grDLVsMeas");
   grDLVsMeas->SetMarkerStyle(kFullCircle);
   grDLVsMeas->SetMarkerSize(1);
   grDLVsMeas->SetMarkerColor(color);

   sprintf(hName, "hDLVsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH2F(hName, hName, 1000, fMinFill, fMaxFill, dlHi-dlLo, dlLo, dlHi);
   ((TH1*) o[hName])->SetTitle(";Measurement;Dead Layer, #mug/cm^{2};");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grDLVsMeas, "p");

   sprintf(hName, "hDLVsFill_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH1F(hName, hName, fMaxFill-fMinFill, fMinFill, fMaxFill);
   ((TH1*) o[hName])->GetYaxis()->SetRangeUser(dlLo, dlHi);
   ((TH1*) o[hName])->SetTitle(";Fill;Dead Layer, #mug/cm^{2};");
   ((TH1*) o[hName])->SetMarkerStyle(kFullCircle);
   ((TH1*) o[hName])->SetMarkerSize(1);
   ((TH1*) o[hName])->SetMarkerColor(color);
   ((TH1*) o[hName])->SetOption("E1");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grDLVsMeas, "p");

   // Banana fit params
   TGraphErrors *grBananaChi2Ndf = new TGraphErrors();
   grBananaChi2Ndf->SetName("grBananaChi2Ndf");
   grBananaChi2Ndf->SetMarkerStyle(kFullCircle);
   grBananaChi2Ndf->SetMarkerSize(1);
   grBananaChi2Ndf->SetMarkerColor(color);

   sprintf(hName, "hBananaChi2Ndf_%s_%s", strPolId.c_str(), strBeamE.c_str());
   //o[hName] = new TH2F(hName, hName, 1, fMinFill, fMaxFill, 1, 0, 5e4);
   o[hName] = new TH2F(hName, hName, 1, fMinFill, fMaxFill, 1, 0, 1000);
   ((TH1*) o[hName])->SetTitle(";Measurement;#chi^{2}/ndf;");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grBananaChi2Ndf, "p");

   // per channel histograms

   for (UShort_t iCh=1; iCh<=N_SILICON_CHANNELS; iCh++)
   {
      // t0
      grT0VsMeas = new TGraphErrors();
      grT0VsMeas->SetName("grT0VsMeas");
      grT0VsMeas->SetMarkerStyle(kFullCircle);
      grT0VsMeas->SetMarkerSize(1);
      grT0VsMeas->SetMarkerColor(color);

      sprintf(hName, "hT0VsMeas_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
      o[hName] = new TH2F(hName, hName, 1000, fMinFill, fMaxFill, t0Hi-t0Lo, t0Lo, t0Hi);
      ((TH1*) o[hName])->SetTitle(";Measurement;t_{0}, ns;");
      ((TH1*) o[hName])->GetListOfFunctions()->Add(grT0VsMeas, "p");

      sprintf(hName, "hT0_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
      o[hName] = new TH1F(hName, hName, t0Hi-t0Lo, t0Lo, t0Hi);
      ((TH1*) o[hName])->SetTitle(";t_{0}, ns;Events;");

      // DL
      grDLVsMeas = new TGraphErrors();
      grDLVsMeas->SetName("grDLVsMeas");
      grDLVsMeas->SetMarkerStyle(kFullCircle);
      grDLVsMeas->SetMarkerSize(1);
      grDLVsMeas->SetMarkerColor(color);

      sprintf(hName, "hDLVsMeas_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
      o[hName] = new TH2F(hName, hName, 1000, fMinFill, fMaxFill, dlHi-dlLo, dlLo, dlHi);
      ((TH1*) o[hName])->SetTitle(";Measurement;Dead Layer, #mug/cm^{2};");
      ((TH1*) o[hName])->GetListOfFunctions()->Add(grDLVsMeas, "p");

      sprintf(hName, "hDL_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
      o[hName] = new TH1F(hName, hName, dlHi-dlLo, dlLo, dlHi);
      ((TH1*) o[hName])->SetTitle(";Dead Layer, #mug/cm^{2};Events;");

      // t0 vs DL
      TGraphErrors* grT0VsDL = new TGraphErrors();
      grT0VsDL->SetName("grT0VsDL");
      grT0VsDL->SetMarkerStyle(kFullCircle);
      grT0VsDL->SetMarkerSize(1);
      grT0VsDL->SetMarkerColor(color);

      sprintf(hName, "hT0VsDL_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
      o[hName] = new TH2F(hName, hName, dlHi-dlLo, dlLo, dlHi, t0Hi-t0Lo, t0Lo, t0Hi);
      ((TH1*) o[hName])->SetTitle(";Dead Layer, #mug/cm^{2};t_{0}, ns;");
      ((TH1*) o[hName])->GetListOfFunctions()->Add(grT0VsDL, "p");
   }

   sprintf(hName, "hT0VsChannel_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH1F(hName, hName, N_SILICON_CHANNELS, 1, N_SILICON_CHANNELS+1);
   ((TH1*) o[hName])->SetTitle(";Channel;Mean t_{0}, ns;");

   sprintf(hName, "hDLVsChannel_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH1F(hName, hName, N_SILICON_CHANNELS, 1, N_SILICON_CHANNELS+1);
   ((TH1*) o[hName])->SetTitle(";Channel;Mean Dead Layer, #mug/cm^{2};");

   // t0 vs DL
   TGraphErrors* grT0VsDLMean = new TGraphErrors();
   grT0VsDLMean->SetName("grT0VsDLMean");
   grT0VsDLMean->SetMarkerStyle(kFullCircle);
   grT0VsDLMean->SetMarkerSize(1);
   grT0VsDLMean->SetMarkerColor(color);

   sprintf(hName, "hT0VsDLMean_%s_%s", strPolId.c_str(), strBeamE.c_str());
   o[hName] = new TH2F(hName, hName, dlHi-dlLo, dlLo, dlHi, t0Hi-t0Lo, t0Lo, t0Hi);
   ((TH1*) o[hName])->SetTitle(";Dead Layer, #mug/cm^{2};Mean t_{0}, ns;");
   ((TH1*) o[hName])->GetListOfFunctions()->Add(grT0VsDLMean, "p");

} //}}}


/** */
void MAsymRunHists::Fill(EventConfig &rc)
{
   Double_t runId            = rc.fRunInfo->RUNID;
   UInt_t   fillId           = (UInt_t) runId;
   UInt_t   beamEnergy       = (UInt_t) (rc.fRunInfo->GetBeamEnergy() + 0.5);
   Short_t  polId            = rc.fRunInfo->fPolId;
   time_t   runStartTime     = rc.fRunInfo->StartTime;
   Short_t  targetId         = rc.fMseRunInfoX->target_id;
   Char_t   targetOrient     = rc.fMseRunInfoX->target_orient[0];
   Float_t  ana_power        = rc.fAnaResult->A_N[1];
   Float_t  asymmetry        = rc.fAnaResult->sinphi[0].P[0] * rc.fAnaResult->A_N[1];
   Float_t  asymmetryErr     = rc.fAnaResult->sinphi[0].P[1] * rc.fAnaResult->A_N[1];
   Float_t  polarization     = rc.fAnaResult->sinphi[0].P[0] * 100.;
   Float_t  polarizationErr  = rc.fAnaResult->sinphi[0].P[1] * 100.;
   Float_t  profileRatio     = rc.fAnaResult->fIntensPolarR;
   Float_t  profileRatioErr  = rc.fAnaResult->fIntensPolarRErr;
   Float_t  max_rate         = rc.fAnaResult->max_rate;
   Float_t  t0               = rc.fCalibrator->fChannelCalibs[0].fT0Coef;
   Float_t  t0Err            = rc.fCalibrator->fChannelCalibs[0].fT0CoefErr;
   Float_t  dl               = rc.fCalibrator->fChannelCalibs[0].fDLWidth;
   Float_t  dlErr            = rc.fCalibrator->fChannelCalibs[0].fDLWidthErr;

   ChannelCalib *chCalib     = rc.fCalibrator->GetAverage();
   t0    = chCalib->fT0Coef;
   t0Err = chCalib->fT0CoefErr;
   dl       = chCalib->fDLWidth;
   dlErr    = chCalib->fDLWidthErr;

   //if (isnan(t0) || isinf(t0))
   //   printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
   //t0Err         = chCalib->fT0CoefErr;

   if (gRunConfig.fBeamEnergies.find((EBeamEnergy) beamEnergy) == gRunConfig.fBeamEnergies.end())
      return;

   char hName[256];

   string strPolId = RunConfig::AsString((EPolarimeterId) polId);
   string strBeamE = RunConfig::AsString((EBeamEnergy) beamEnergy);

   TGraphErrors *graphErrs = 0;
   TGraph       *graph = 0;

   // Rate
   sprintf(hName, "hMaxRateVsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
   graph = (TGraph*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grMaxRateVsMeas");
   graph->SetPoint(graph->GetN(), runId, max_rate);

   // Targets
   sprintf(hName, "hTargetVsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());

   if (targetOrient == 'H') {
      graph = (TGraph*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grHTargetVsMeas");
   }

   if (targetOrient == 'V') {
      graph = (TGraph*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grVTargetVsMeas");
   }

   graph->SetPoint(graph->GetN(), runId, targetId);

   UInt_t nPoints = 0;

   // Polarization
   sprintf(hName, "hPolarVsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
   graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grPolarVsMeas");
   nPoints = graphErrs->GetN();
   graphErrs->SetPoint(nPoints, runId, polarization);
   graphErrs->SetPointError(nPoints, 0, polarizationErr);

   // Polarization vs time
   sprintf(hName, "hPolarVsTime_%s_%s", strPolId.c_str(), strBeamE.c_str());
   graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grPolarVsTime");
   nPoints = graphErrs->GetN();
   graphErrs->SetPoint(nPoints, runStartTime, polarization);
   graphErrs->SetPointError(nPoints, 0, polarizationErr);

   // Profiles r
   sprintf(hName, "hRVsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());

   graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grRVsMeas");
   nPoints = graphErrs->GetN();
   graphErrs->SetPoint(nPoints, runId, profileRatio);
   graphErrs->SetPointError(nPoints, 0, profileRatioErr);

   if (targetOrient == 'H') {
      graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grRVsMeasH");
   }

   if (targetOrient == 'V') {
      graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grRVsMeasV");
   }

   nPoints = graphErrs->GetN();
   graphErrs->SetPoint(nPoints, runId, profileRatio);
   graphErrs->SetPointError(nPoints, 0, profileRatioErr);

   // t0
   sprintf(hName, "hT0VsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
   graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grT0VsMeas");
   nPoints = graphErrs->GetN();
   graphErrs->SetPoint(nPoints, runId, t0);
   graphErrs->SetPointError(nPoints, 0, t0Err);

   // Dead layer
   sprintf(hName, "hDLVsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
   graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grDLVsMeas");
   nPoints = graphErrs->GetN();
   graphErrs->SetPoint(nPoints, runId, dl);
   graphErrs->SetPointError(nPoints, 0, dlErr);

   // Banana fit params
   //Float_t bananaChi2Ndf = rc.fCalibrator->fChannelCalibs[0].fBananaChi2Ndf;
   Float_t bananaChi2Ndf = chCalib->fBananaChi2Ndf;

   sprintf(hName, "hBananaChi2Ndf_%s_%s", strPolId.c_str(), strBeamE.c_str());
   graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grBananaChi2Ndf");
   nPoints = graphErrs->GetN();
   graphErrs->SetPoint(nPoints, runId, bananaChi2Ndf);

   // per channel hists

   for (UShort_t iCh=1; iCh<=N_SILICON_CHANNELS; iCh++) {

      // T0
      t0    = rc.fCalibrator->fChannelCalibs[iCh].fT0Coef;
      t0Err = rc.fCalibrator->fChannelCalibs[iCh].fT0CoefErr;

      sprintf(hName, "hT0VsMeas_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
      graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grT0VsMeas");
      nPoints = graphErrs->GetN();
      graphErrs->SetPoint(nPoints, runId, t0);
      graphErrs->SetPointError(nPoints, 0, t0Err);

      sprintf(hName, "hT0_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
      ((TH1*) o[hName])->Fill(t0);

      // DL
      dl    = rc.fCalibrator->fChannelCalibs[iCh].fDLWidth;
      dlErr = rc.fCalibrator->fChannelCalibs[iCh].fDLWidthErr;

      sprintf(hName, "hDLVsMeas_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
      graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grDLVsMeas");
      nPoints = graphErrs->GetN();
      graphErrs->SetPoint(nPoints, runId, dl);
      graphErrs->SetPointError(nPoints, 0, dlErr);

      sprintf(hName, "hDL_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
      ((TH1*) o[hName])->Fill(dl);

      // t0 vs DL
      sprintf(hName, "hT0VsDL_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
      graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grT0VsDL");
      nPoints = graphErrs->GetN();
      graphErrs->SetPoint(nPoints, dl, t0);
      graphErrs->SetPointError(nPoints, dlErr, t0Err);
   }
}


/** */
void MAsymRunHists::Print(const Option_t* opt) const
{ //{{{
   opt = ""; //printf("MAsymRunHists:\n");
   DrawObjContainer::Print();
} //}}}


/** */
void MAsymRunHists::Fill(ChannelEvent *ch, string sid)
{ //{{{
} //}}}


/** */
void MAsymRunHists::PostFill()
{
   char hName[256];

   //Double_t *arrHX = new Double_t[fHTargetVsRun.size()];
   //Double_t *arrHY = new Double_t[fHTargetVsRun.size()];

   //map<Double_t, UInt_t>::iterator im;
   //UInt_t i = 0;

   //for (im=fHTargetVsRun.begin(); im!=fHTargetVsRun.end(); ++im, ++i) {
   //   *(arrHX + i) = im->first;   
   //   *(arrHY + i) = im->second;   
   //}

   //Double_t *arrVX = new Double_t[fVTargetVsRun.size()];
   //Double_t *arrVY = new Double_t[fVTargetVsRun.size()];

   //for (im=fVTargetVsRun.begin(), i=0; im!=fVTargetVsRun.end(); ++im, ++i) {
   //   *(arrVX + i) = im->first;   
   //   *(arrVY + i) = im->second;   
   //}

   for (UInt_t i=0; i!=N_POLARIMETERS; i++) {

      for (IterBeamEnergy iBE=gRunConfig.fBeamEnergies.begin(); iBE!=gRunConfig.fBeamEnergies.end(); ++iBE) {

         string strPolId = RunConfig::AsString((EPolarimeterId) i);
         string strBeamE = RunConfig::AsString(*iBE);

         // Adjust axis range
         Double_t xmin, ymin, xmax, ymax, xdelta, ydelta;
         Double_t xminDL, yminDL, xmaxDL, ymaxDL, xdeltaDL, ydeltaDL;

         sprintf(hName, "hT0VsMeas_%s_%s", strPolId.c_str(), strBeamE.c_str());
         TGraphErrors *graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grT0VsMeas");
         graphErrs->ComputeRange(xmin, ymin, xmax, ymax);
         ydelta = (ymax - ymin)*0.1;
         ((TH1*) o[hName])->GetYaxis()->SetLimits(ymin-ydelta, ymax+ydelta);

         sprintf(hName, "hT0VsFill_%s_%s", strPolId.c_str(), strBeamE.c_str());
         graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grT0VsMeas");
         //((TH1*) o[hName])->GetYaxis()->SetLimits(ymin-ydelta, ymax+ydelta);
         ((TH1*) o[hName])->GetYaxis()->SetRangeUser(ymin-ydelta, ymax+ydelta);

         Float_t fillFirst = fMinFill, fillLast = fMaxFill;

         //switch ((EPolarimeterId) i) {
         //case kB1U:
         //   fillFirst = 15154;
         //   break;
         //case kY1D:
         //   fillFirst = 15154;
         //   //fillFirst = 15260;
         //   //fillLast  = 15420;
         //   break;
         //case kB2D:
         //   fillFirst = 15154;
         //   break;
         //case kY2U:
         //   fillFirst = 15154;
         //   break;
         //}

         TGraphErrors *graph;

         // Polarization
         sprintf(hName, "hPolarVsFill_%s_%s", strPolId.c_str(), strBeamE.c_str());
         graph = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grPolarVsMeas");

         utils::RemoveOutliers(graph, 2, 3);
         utils::BinGraph(graph, (TH1*) o[hName]);

         ((TH1*) o[hName])->GetListOfFunctions()->Remove(graph);

         TF1 *funcPolarVsFill = new TF1("funcPolarVsFill", "[0]");
         funcPolarVsFill->SetParameter(0, 50);
         ((TH1*) o[hName])->Fit("funcPolarVsFill");
         delete funcPolarVsFill;

         // Polarization vs time
         sprintf(hName, "hPolarVsTime_%s_%s", strPolId.c_str(), strBeamE.c_str());
         graph = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grPolarVsTime");

         TF1 *funcPolarVsTime = new TF1("funcPolarVsTime", "[0] + [1]*x");
         //funcPolarVsTime->SetParameter(0, 0);
         funcPolarVsTime->SetParNames("offset", "slope");
         graph->Fit("funcPolarVsTime");
         delete funcPolarVsTime;

         // Profiles r
         // H target = vert profile
         sprintf(hName, "hRVsFill_H_%s_%s", strPolId.c_str(), strBeamE.c_str());
         graph = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grRVsMeasH");

         utils::RemoveOutliers(graph, 2, 3);
         utils::BinGraph(graph, (TH1*) o[hName]);

         ((TH1*) o[hName])->GetListOfFunctions()->Remove(graph);

         TF1 *funcRVsFill = new TF1("funcRVsFill", "[0]", fillFirst, fillLast);
         funcRVsFill->SetParameter(0, 0.02);
         ((TH1*) o[hName])->Fit("funcRVsFill", "R");
         delete funcRVsFill;

         // V target = horiz profile
         sprintf(hName, "hRVsFill_V_%s_%s", strPolId.c_str(), strBeamE.c_str());
         graph = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grRVsMeasV");

         utils::RemoveOutliers(graph, 2, 3);
         utils::BinGraph(graph, (TH1*) o[hName]);

         ((TH1*) o[hName])->GetListOfFunctions()->Remove(graph);

         funcRVsFill = new TF1("funcRVsFill", "[0]", fillFirst, fillLast);
         funcRVsFill->SetParameter(0, 0.02);
         ((TH1*) o[hName])->Fit("funcRVsFill", "R");
         delete funcRVsFill;

         // t0
         sprintf(hName, "hT0VsFill_%s_%s", strPolId.c_str(), strBeamE.c_str());
         graph = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grT0VsMeas");

         //utils::RemoveOutliers(graph, 2, 3);
         utils::BinGraph(graph, (TH1*) o[hName]);

         ((TH1*) o[hName])->GetListOfFunctions()->Remove(graph);

         //if ((EPolarimeterId) i == kB1U || (EPolarimeterId) i == kY2U) {
            TF1 *funcT0VsFill = new TF1("funcT0VsFill", "[0] + [1]*x");
            funcT0VsFill->SetParameters(0, 0);
            funcT0VsFill->SetParNames("offset", "slope");
            ((TH1*) o[hName])->Fit("funcT0VsFill");
            delete funcT0VsFill;
         //}

         // DL
         sprintf(hName, "hDLVsFill_%s_%s", strPolId.c_str(), strBeamE.c_str());
         graph = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grDLVsMeas");

         utils::RemoveOutliers(graph, 2, 3);
         utils::BinGraph(graph, (TH1*) o[hName]);

         ((TH1*) o[hName])->GetListOfFunctions()->Remove(graph);

         //if ((EPolarimeterId) i == kB1U || (EPolarimeterId) i == kY2U) {
         //TF1 *funcDLVsFill = new TF1("funcDLVsFill", "[0] + [1]*x", fillFirst, fillLast);
         TF1 *funcDLVsFill = new TF1("funcDLVsFill", "[0]", fillFirst, fillLast);
         funcDLVsFill->SetParameters(0, 0);
         //funcDLVsFill->SetParNames("offset", "slope");
         funcDLVsFill->SetParNames("const");
         ((TH1*) o[hName])->Fit("funcDLVsFill", "R");
         delete funcDLVsFill;
         //}

         sprintf(hName, "hT0VsChannel_%s_%s", strPolId.c_str(), strBeamE.c_str());
         TH1* hT0VsChannel = (TH1*) o[hName];

         sprintf(hName, "hDLVsChannel_%s_%s", strPolId.c_str(), strBeamE.c_str());
         TH1* hDLVsChannel = (TH1*) o[hName];

         sprintf(hName, "hT0VsDLMean_%s_%s", strPolId.c_str(), strBeamE.c_str());
         TGraphErrors* grT0VsDLMean = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grT0VsDLMean");

         for (UShort_t iCh=1; iCh<=N_SILICON_CHANNELS; iCh++)
         {
            sprintf(hName, "hT0VsMeas_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
            graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grT0VsMeas");
            graphErrs->ComputeRange(xmin, ymin, xmax, ymax);
            ydelta = (ymax - ymin)*0.1;
            ((TH1*) o[hName])->GetYaxis()->SetLimits(ymin-ydelta, ymax+ydelta);

            sprintf(hName, "hDLVsMeas_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
            graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grDLVsMeas");
            graphErrs->ComputeRange(xminDL, yminDL, xmaxDL, ymaxDL);
            ydeltaDL = (ymaxDL - yminDL)*0.1;

            // T0
            sprintf(hName, "hT0_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
            TH1* hT0 = (TH1*) o[hName];

            hT0VsChannel->SetBinContent(iCh, hT0->GetMean());
            hT0VsChannel->SetBinError(iCh, hT0->GetRMS());

            // Dead layer
            sprintf(hName, "hDL_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
            TH1* hDL = (TH1*) o[hName];

            hDLVsChannel->SetBinContent(iCh, hDL->GetMean());
            hDLVsChannel->SetBinError(iCh, hDL->GetRMS());

            // t0 vs DL
            sprintf(hName, "hT0VsDL_%s_%s_%02d", strPolId.c_str(), strBeamE.c_str(), iCh);
            //graphErrs = (TGraphErrors*) ((TH1*) o[hName])->GetListOfFunctions()->FindObject("grT0VsDL");
            ((TH1*) o[hName])->GetXaxis()->SetLimits(yminDL-ydeltaDL, ymaxDL+ydeltaDL);
            ((TH1*) o[hName])->GetYaxis()->SetLimits(ymin-ydelta, ymax+ydelta);

            UInt_t nPoints = grT0VsDLMean->GetN();
            grT0VsDLMean->SetPoint(nPoints, hDL->GetMean(), hT0->GetMean());
            //grT0VsDLMean->SetPointError(nPoints, hDL->GetRMS(), hT0->GetRMS());
         }
      }
   }


   for (IterBeamEnergy iBE=gRunConfig.fBeamEnergies.begin(); iBE!=gRunConfig.fBeamEnergies.end(); ++iBE) {

      string strBeamE = RunConfig::AsString(*iBE);

      // Blue
      TH1* hPolarU = (TH1*) o["hPolarVsFill_B1U_" + strBeamE];
      TH1* hPolarD = (TH1*) o["hPolarVsFill_B2D_" + strBeamE];

      ((TH1*) o["hPolarBlueRatioVsFill_" + strBeamE])->Divide(hPolarU, hPolarD);

      //TF1 *funcRatioVsFill = new TF1("funcRatioVsFill", "[0]", 15154, fMaxFill);
      TF1 *funcRatioVsFill = new TF1("funcRatioVsFill", "[0]", fMinFill, fMaxFill);
      funcRatioVsFill->SetParNames("const");
      ((TH1*) o["hPolarBlueRatioVsFill_" + strBeamE])->Fit("funcRatioVsFill", "R");
      delete funcRatioVsFill;

      // Yellow
      hPolarU = (TH1*) o["hPolarVsFill_Y2U_" + strBeamE];
      hPolarD = (TH1*) o["hPolarVsFill_Y1D_" + strBeamE];

      ((TH1*) o["hPolarYellowRatioVsFill_" + strBeamE])->Divide(hPolarU, hPolarD);

      //funcRatioVsFill = new TF1("funcRatioVsFill", "[0]", 15220, fMaxFill);
      funcRatioVsFill = new TF1("funcRatioVsFill", "[0]", fMinFill, fMaxFill);
      funcRatioVsFill->SetParNames("const");
      ((TH1*) o["hPolarYellowRatioVsFill_" + strBeamE])->Fit("funcRatioVsFill", "R");
      delete funcRatioVsFill;
   }
}


void MAsymRunHists::UpdateLimits()
{
   char hName[256];

   for (IterPolarimeterId iPolId=gRunConfig.fPolarimeters.begin(); iPolId!=gRunConfig.fPolarimeters.end(); ++iPolId) {
      for (IterBeamEnergy iBE=gRunConfig.fBeamEnergies.begin(); iBE!=gRunConfig.fBeamEnergies.end(); ++iBE) {
         string strPolId = RunConfig::AsString(*iPolId);
         string strBeamE = RunConfig::AsString(*iBE);

         sprintf(hName, "hPolarVsTime_%s_%s", strPolId.c_str(), strBeamE.c_str());

         ((TH1*) o[hName])->GetXaxis()->SetLimits(fMinTime, fMaxTime);
      }
   }
}
