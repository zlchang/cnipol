/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#ifndef ChannelCalib_h
#define ChannelCalib_h

#include <map>
#include <vector>

#include "TObject.h"

enum EFitStatus {
   kUNKNOWN,
   kDLFIT_OK,
   kDLFIT_FAIL,
   kDL_OUTLIER,
   kT0_OUTLIER,
   kCHI2_OUTLIER
};

class ChannelCalib;

typedef std::vector<ChannelCalib>                        ChannelCalibVec;
typedef std::map<UShort_t, ChannelCalib>                 ChannelCalibMap;
typedef std::map<UShort_t, ChannelCalib>::iterator       ChannelCalibMapIter;
typedef std::map<UShort_t, ChannelCalib>::const_iterator ChannelCalibMapConstIter;
typedef std::pair<const UShort_t, ChannelCalib>          ChannelCalibPair;


/** */
class ChannelCalib : public TObject
{
public:

   Float_t    fACoef;
   Float_t    fACoefErr;
   Float_t    fAChi2Ndf;
   Float_t    fICoef;
   Float_t    fICoefErr;
   Float_t    fIChi2Ndf;
   Float_t    fDLWidth;       // Dead layer
   Float_t    fDLWidthErr;
   Float_t    fT0Coef;        // T0
   Float_t    fT0CoefErr;
   Float_t    fAvrgEMiss;
   Float_t    fAvrgEMissErr;
   Float_t    fEMeasDLCorr;   // a correction factor for E_meas = C_alpha * ADC * fEMeasDLCorr
   Float_t    fBananaChi2Ndf;
   EFitStatus fFitStatus;

public:

   ChannelCalib();
   ~ChannelCalib();

   EFitStatus GetFitStatus() const;   
   Float_t    GetBananaChi2Ndf() const { return fBananaChi2Ndf; }
   void       CopyAlphaCoefs(const ChannelCalib &chCalib);
   void       Print(const Option_t* opt="") const;
   void       PrintAsPhp(FILE *f=stdout) const;
   void       ResetToZero();

   ClassDef(ChannelCalib, 1)
};

#endif
