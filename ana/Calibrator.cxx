/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "Calibrator.h"
#include "ChannelCalib.h"

#include "Asym.h"
#include "AsymGlobals.h"
#include "RunInfo.h"


ClassImp(Calibrator)

using namespace std;

/** Default constructor. */
Calibrator::Calibrator() : TObject(), fChannelCalibs()
{
}


/** Default destructor. */
Calibrator::~Calibrator()
{
}


/** */
ChannelCalib* Calibrator::GetAverage()
{
	//Info("GetAverage", "Executing GetAverage()");

   ChannelCalib *ch = new ChannelCalib();
   ch->ResetToZero();

   ChannelCalibMap::const_iterator mi;
   ChannelCalibMap::const_iterator mb = fChannelCalibs.begin();
   ChannelCalibMap::const_iterator me = fChannelCalibs.end();

   UInt_t nChannels = 0;

   for (mi=mb; mi!=me; ++mi) {
      if (RunConst::IsSiliconChannel(mi->first) && !isnan(mi->second.fT0Coef) && !isinf(mi->second.fT0Coef) &&
          !gRunInfo->fDisabledChannels[mi->first-1] && mi->second.fBananaChi2Ndf < 1e3)
      {
         ch->fT0Coef        += mi->second.fT0Coef;
         ch->fT0CoefErr     += mi->second.fT0CoefErr;
         ch->fDLWidth       += mi->second.fDLWidth;
         ch->fDLWidthErr    += mi->second.fDLWidthErr;
         ch->fBananaChi2Ndf += mi->second.fBananaChi2Ndf;
         nChannels++;
      }
   }

   if (nChannels) {
      ch->fT0Coef        /= nChannels;
      ch->fT0CoefErr     /= nChannels;
      ch->fDLWidth       /= nChannels;
      ch->fDLWidthErr    /= nChannels;
      ch->fBananaChi2Ndf /= nChannels;
   }

   return ch;
}


/** */
void Calibrator::CopyAlphaCoefs(Calibrator &calibrator)
{
   ChannelCalibMap::const_iterator mi;
   ChannelCalibMap::const_iterator mb = calibrator.fChannelCalibs.begin();
   ChannelCalibMap::const_iterator me = calibrator.fChannelCalibs.end();

   for (mi=mb; mi!=me; ++mi) {

      ChannelCalibMap::iterator iChCalib = fChannelCalibs.find(mi->first);

      if (iChCalib != fChannelCalibs.end()) {
         iChCalib->second.CopyAlphaCoefs(mi->second);
      } else {
         ChannelCalib newChCalib;
         newChCalib.CopyAlphaCoefs(mi->second);
         fChannelCalibs[mi->first] = newChCalib;
      }
   }
}


/** */
void Calibrator::Calibrate(DrawObjContainer *c)
{
	Info("Calibrate", "Executing Calibrate()");
}


/** */
void Calibrator::CalibrateFast(DrawObjContainer *c)
{
	Info("CalibrateFast", "Executing CalibrateFast()");
}


/** */
TFitResultPtr Calibrator::Calibrate(TH1 *h, TH1D *&hMeanTime, UShort_t chId, Bool_t wideLimits)
{
   return 0;
}


/** */
void Calibrator::Print(const Option_t* opt) const
{ //{{{
   opt = "";

   printf("Calibrator:\n");

   ChannelCalibMap::const_iterator mi;
   ChannelCalibMap::const_iterator mb = fChannelCalibs.begin();
   ChannelCalibMap::const_iterator me = fChannelCalibs.end();

   for (mi=mb; mi!=me; mi++) {
		mi->second.Print();
      printf("\n");
   }
} //}}}


/** */
void Calibrator::PrintAsPhp(FILE *f) const
{ //{{{
   ChannelCalibMap::const_iterator mi;
   ChannelCalibMap::const_iterator mb = fChannelCalibs.begin();
   ChannelCalibMap::const_iterator me = fChannelCalibs.end();

   for (mi=mb; mi!=me; mi++) {
  
      UShort_t chId = mi->first;

      fprintf(f, "$rc['calib'][%d] = ", chId);
      mi->second.PrintAsPhp(f);
      fprintf(f, ";\n");
   }
} //}}}


/** */
void Calibrator::PrintAsConfig(FILE *f) const
{ //{{{
   ChannelCalibMap::const_iterator mi;
   ChannelCalibMap::const_iterator mb = fChannelCalibs.begin();
   ChannelCalibMap::const_iterator me = fChannelCalibs.end();

   UShort_t chId;
   const ChannelCalib *ch;

   for (mi=mb; mi!=me; mi++) {
  
      chId =  mi->first;
      ch   = &mi->second;

      fprintf(f, "Channel%02d=%5.3f %5.3f %7.1f %4.1f %5.2f %5.3f %4.1f %4.1f %4.3G %4.3G %4.3G %4.3G %4.3G\n",
         chId, -1*ch->fT0Coef, ch->fACoef*ch->fEMeasDLCorr, ch->fAvrgEMiss,
         10., 100., ch->fACoef, 0., 0., 0., 0., 0., 0., 0.);
   }

	// XXX need to add 8 more default channels here

} //}}}
