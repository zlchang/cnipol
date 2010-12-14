/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "Calibrator.h"

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
void Calibrator::Calibrate(DrawObjContainer *c)
{
	Warning("Calibrate", "Executing Calibrate()");
}


/** */
TFitResultPtr Calibrator::Calibrate(TH1 *h, TH1D *hMeanTime)
{
   //TFitResultPtr result = 0;
   return 0;
}


/** */
void Calibrator::Print(const Option_t* opt) const
{
   opt = "";

   printf("Calibrator:\n");

   ChannelCalibMap::const_iterator mi;
   ChannelCalibMap::const_iterator mb = fChannelCalibs.begin();
   ChannelCalibMap::const_iterator me = fChannelCalibs.end();

   for (mi=mb; mi!=me; mi++) {
		mi->second.Print();
      printf("\n");
   }
}


/** */
void Calibrator::PrintAsPhp(FILE *f) const
{
   ChannelCalibMap::const_iterator mi;
   ChannelCalibMap::const_iterator mb = fChannelCalibs.begin();
   ChannelCalibMap::const_iterator me = fChannelCalibs.end();

   UShort_t chId;

   for (mi=mb; mi!=me; mi++) {
  
      chId = mi->first;

      fprintf(f, "$rc['calib'][%d]['ACoef']          = %f;\n", chId, mi->second.fACoef);
      fprintf(f, "$rc['calib'][%d]['ACoefErr']       = %f;\n", chId, mi->second.fACoefErr);
      fprintf(f, "$rc['calib'][%d]['ICoef']          = %f;\n", chId, mi->second.fICoef);
      fprintf(f, "$rc['calib'][%d]['ICoefErr']       = %f;\n", chId, mi->second.fICoefErr);
      fprintf(f, "$rc['calib'][%d]['DLCoef']         = %f;\n", chId, mi->second.fDLCoef);
      fprintf(f, "$rc['calib'][%d]['DLCoefErr']      = %f;\n", chId, mi->second.fDLCoefErr);
      fprintf(f, "$rc['calib'][%d]['T0Coef']         = %f;\n", chId, mi->second.fT0Coef);
      fprintf(f, "$rc['calib'][%d]['T0CoefErr']      = %f;\n", chId, mi->second.fT0CoefErr);
      fprintf(f, "$rc['calib'][%d]['fAvrgEMiss']     = %f;\n", chId, mi->second.fAvrgEMiss);
      fprintf(f, "$rc['calib'][%d]['fAvrgEMissErr']  = %f;\n", chId, mi->second.fAvrgEMissErr);
      fprintf(f, "$rc['calib'][%d]['fBananaChi2Ndf'] = %f;\n", chId, mi->second.fBananaChi2Ndf);
   }
}


/** */
void Calibrator::PrintAsConfig(FILE *f) const
{ //{{{
   ChannelCalibMap::const_iterator mi;
   ChannelCalibMap::const_iterator mb = fChannelCalibs.begin();
   ChannelCalibMap::const_iterator me = fChannelCalibs.end();

   UShort_t chId;
   const ChannelCalib *ch;

   for (mi=mb; mi!=me; mi++) {
  
      chId = mi->first;
      ch = &mi->second;

      fprintf(f, "Channel%02d=%5.3f %5.3f %7.1f %4.1f %5.2f %5.3f %4.1f %4.1f %4.3G %4.3G %4.3G %4.3G %4.3G\n",
         chId, ch->fT0Coef, ch->fACoef, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.);
   }

	// XXX need to add 8 more default channels here

} //}}}
