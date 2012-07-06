
#ifndef AnaFillResult_h
#define AnaFillResult_h

#include <map>
#include <time.h>
#include <fstream>

#include "TObject.h"

#include "Asym.h"
#include "AnaFillExternResult.h"
#include "AnaMeasResult.h"
#include "EventConfig.h"
#include "DrawObjContainer.h"
#include "TargetUId.h"
#include "MeasInfo.h"


class AnaFillResult;
class AnaGlobResult;

typedef std::map<UInt_t, AnaFillResult>                   AnaFillResultMap;
typedef std::map<UInt_t, AnaFillResult>::iterator         AnaFillResultMapIter;
typedef std::map<UInt_t, AnaFillResult>::const_iterator   AnaFillResultMapConstIter;


/** */
class AnaFillResult : public TObject
{
protected:

   UInt_t               fFillId;
   AnaGlobResult       *fAnaGlobResult;
   time_t               fStartTime;
   AnaFillExternResult *fAnaFillExternResult;

   PolId2TGraphMap            fPCPolarGraphs;
   PolId2TGraphMap            fPCPolarInjGraphs;
   PolId2TgtOrient2TGraphMap  fPCProfRGraphs;

   TFitResultPtr        fPCPolarFitRes;
   TFitResultPtr        fPolProfRFitRes;

   //TH1F                *fAsymVsBunchId_X;
   Double_t             fFlattopEnergy;
   EFillType            fFillType;

public:

   AnaMeasResultMap             fAnaMeasResults;
   MeasInfoMap                  fMeasInfos;

   PolId2ValErrMap              fPCPolars;          // (Nominal) intensity weighted polarization measurement results
   PolId2ValErrMap              fPCPolarUnWs;       // 
   PolId2TgtOrient2ValErrMap    fPCProfRUnWs;       //!
   TargetUId2ValErrMap          fPCPolarsByTargets;
   PolId2ValErrMap              fPCProfPolars;      // Polarization as measured from the sweep measurements P = P_0/sqrt(1 + R)
   RingId2ValErrMap             fHJPolars;
   RingId2ValErrMap             fHJAsyms;

   RingId2ValErrMap             fBeamPolars;       //!
   RingId2ValErrMap             fBeamPolarP0s;     //!
   RingId2ValErrMap             fBeamPolarDecays;  //!
   RingId2ValErrMap             fBeamCollPolars;   //! not used
   
   PolId2ValErrMap              fSystProfPolar;
   PolId2ValErrMap              fSystJvsCPolar;
   RingId2ValErrMap             fSystUvsDPolar;

   String2TgtOrientMap          fMeasTgtOrients;   // a stupid temporary fix
   String2TargetIdMap           fMeasTgtIds;       // a stupid temporary fix
   String2RingIdMap             fMeasRingIds;      // a stupid temporary fix

   RingId2TgtOrient2ValErrMap   fPolProfRs;
   RingId2TgtOrient2ValErrMap   fPolProfPMaxs;
   RingId2TgtOrient2ValErrMap   fPolProfPs;

public:

   AnaFillResult(UInt_t fillId = 0);
   ~AnaFillResult();

   time_t GetStartTime() const;
   time_t GetLumiOnTime();
   time_t GetLumiOffTime();

   void Print(const Option_t* opt="") const;
   //void PrintAsPhp(FILE *f=stdout) const;

   UInt_t               GetFillId() const { return fFillId; }
   Double_t             GetFlattopEnergy() const { return fFlattopEnergy; }
   EFillType            GetFillType() const { return fFillType; }
   TGraphErrors*        GetGrBluIntens() const;
   TGraphErrors*        GetGrYelIntens() const;
   TGraphErrors*        GetIntensGraph(ERingId ringId) const;
   ValErrPair           GetIntensDecay(ERingId ringId) const;
   TGraphErrors*        GetRotCurStarGraph(ERingId ringId) const;
   TGraphErrors*        GetRotCurPhenixGraph(ERingId ringId) const;
   TGraphErrors*        GetSnakeCurGraph(ERingId ringId) const;
   AnaFillExternResult* GetAnaFillExternResult() const;
   void                 AddMeasResult(AnaMeasResult &result);
   void                 AddMeasResult(EventConfig &mm, AnaGlobResult *globRes=0);
   void                 AddGraphMeasResult(EventConfig &mm, DrawObjContainer &ocIn);
   void                 AddExternInfo(std::ifstream &file);
   void                 Process(DrawObjContainer *ocOut=0);
   Bool_t               IsValidFlattopMeas(const MeasInfo &measInfo);
   ValErrPair           GetPCPolar(EPolarimeterId polId, PolId2ValErrMap *normJC=0) const;
   ValErrPair           GetPCPolarUnW(EPolarimeterId polId, PolId2ValErrMap *normJC=0);
   TGraphErrors*        GetPCPolarGraph(EPolarimeterId polId) const;
   TGraphErrors*        GetPCPolarInjGraph(EPolarimeterId polId);
   ValErrPair           GetPCPolarP0(EPolarimeterId polId) const;
   ValErrPair           GetPCPolarSlope(EPolarimeterId polId) const;
   //ValErrPair           GetPCPolarPmax(EPolarimeterId polId, ETargetOrient tgtOrient);
   ValErrPair           GetHJPolar(EPolarimeterId polId);
   ValErrPair           GetHJPolar(ERingId ringId) const;
   ValErrPair           GetHJAsym(EPolarimeterId polId);
   ValErrPair           GetHJAsym(ERingId ringId);
   ValErrPair           GetBeamPolar(ERingId ringId) const;
   ValErrPair           GetBeamPolarP0(ERingId ringId) const;
   ValErrPair           GetBeamPolarDecay(ERingId ringId) const;
   ValErrPair           GetSystUvsDPolar(ERingId ringId);
   ValErrPair           GetSystJvsCPolar(EPolarimeterId polId);
   ValErrPair           GetSystProfPolar(EPolarimeterId polId);
   TGraphErrors*        GetPCProfRGraph(EPolarimeterId polId, ETargetOrient tgtOrient) const;
   ValErrPair           GetPCProfR(ERingId ringId, ETargetOrient tgtOrient) const;
   ValErrPair           GetPCProfR(EPolarimeterId polId, ETargetOrient tgtOrient) const;
   ValErrPair           GetPCProfR0(EPolarimeterId polId, ETargetOrient tgtOrient);
   ValErrPair           GetPCProfRSlope(EPolarimeterId polId, ETargetOrient tgtOrient);
   ValErrPair           GetPCProfPMax(ERingId ringId, ETargetOrient tgtOrient) const;
   ValErrPair           GetPCProfP(ERingId ringId, ETargetOrient tgtOrient) const;
   //ValErrPair           GetProfPolarSyst(EPolarimeterId polId);
   void                 CalcBeamPolar(Bool_t doNorm=kFALSE);
   RingId2ValErrMap     CalcSystUvsDPolar(PolId2ValErrMap &normJC);
   PolId2ValErrMap      CalcSystJvsCPolar(PolId2ValErrMap &normJC);
   PolId2ValErrMap      CalcSystProfPolar(PolId2ValErrMap &normPP);
   ValErrPair           CalcAvrgPolar(EPolarimeterId polId);
   ValErrPair           CalcAvrgPolarUnweighted(EPolarimeterId polId);
   ValErrPair           CalcAvrgPolProfPolar(EPolarimeterId polId);
   ValErrPair           CalcAvrgProfRUnweighted(EPolarimeterId polid, ETargetOrient tgtOrient);
   ValErrPair           CalcAvrgPolProfR(ERingId ringId, ETargetOrient tgtOrient);
   ValErrPair           CalcAvrgPolProfPMax(ERingId ringId, ETargetOrient tgtOrient);
   ValErrPair           CalcPolProfP(ValErrPair R, ValErrPair PMax);
   void                 CalcAvrgAsymByBunch(const AnaMeasResult &amr, const MeasInfo &mi, DrawObjContainer &ocOut) const;
   void                 UpdateExternGraphRange();
   void                 FitExternGraphs();
   void                 FitPolarGraphs();
   void                 SetHJPolar(ERingId ringId, ValErrPair ve);
   void                 SetHJAsym(ERingId ringId, ValErrPair ve);
   void                 AppendToPCPolarGraph(EPolarimeterId polId, Double_t x, Double_t y, Double_t xe, Double_t ye);
   void                 AppendToPCPolarInjGraph(EPolarimeterId polId, Double_t x, Double_t y, Double_t xe, Double_t ye);
   void                 AppendToPCProfRGraph(EPolarimeterId polId, ETargetOrient tgtOrient, Double_t x, Double_t y, Double_t xe, Double_t ye);

   ClassDef(AnaFillResult, 1)
};

#endif
