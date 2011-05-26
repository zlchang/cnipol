
#include "AnaInfo.h"

#include "TString.h"
#include "TSystem.h"

#include "AsymGlobals.h"
#include "DbEntry.h"
#include "MseRunInfo.h"
#include "RunInfo.h"

using namespace std;


/** */
AnaInfo::AnaInfo() :
   fRunName          (""),
   enel              (400),
   eneu              (900),
   widthl            (-30),
   widthu            (3),
   fModes            (MODE_NORMAL),
   FEEDBACKMODE      (0),
   RAWHISTOGRAM      (0),
   DMODE             (0),
   TMODE             (0),
   AMODE             (0),
   BMODE             (1),
   ZMODE             (0),
   MESSAGE           (0),
   CBANANA           (2),
   UPDATE            (0),
   QUICK_MODE        (0),
   MMODE             (1),
   NTMODE            (0),
   RECONFMODE        (1),
   RAMPMODE          (0),
   STUDYMODE         (0),
   fSaveTrees        (0),
   MassSigma         (3),
   MassSigmaAlt      (2),
   OneSigma          (CARBON_MASS_PEAK_SIGMA),
   tshift            (0),
   inj_tshift        (0),
   dx_offset         (0),
   WCMRANGE          (999.05),
   MassLimit         (8),
   nEventsProcessed  (0),
   nEventsTotal      (0),
   thinout           (1),
   fFastCalibThinout (0.10),
   reference_rate    (1),
   //target_count_mm   (0.11),
   target_count_mm   (1),   // Need to get the real value
   fAnaDateTime      (0),
   fAnaTimeReal      (0),
   fAnaTimeCpu       (0),
   fAlphaCalibRun(""), fDlCalibRun(""), fAsymEnv(),
   fFileRunInfo(0), fFileRunConf(0), fFileStdLog(0),
   fFileStdLogName("stdoe.log"), fFlagCopyResults(kFALSE), fFlagUseDb(kFALSE), fFlagUpdateDb(kFALSE),
   fUserGroup(gSystem->GetUserInfo())
{
   Init();
}


/**
 * Default Values for Run Condition
 */
AnaInfo::AnaInfo(string runId) :
   fRunName          (runId),
   enel              (400),
   eneu              (900),
   widthl            (-30),
   widthu            (3),
   fModes            (MODE_NORMAL),
   FEEDBACKMODE      (0),
   RAWHISTOGRAM      (0),
   DMODE             (0),
   TMODE             (0),
   AMODE             (0),
   BMODE             (1),
   ZMODE             (0),
   MESSAGE           (0),
   CBANANA           (2),
   UPDATE            (0),
   QUICK_MODE        (0),
   MMODE             (1),
   NTMODE            (0),
   RECONFMODE        (1),
   RAMPMODE          (0),
   STUDYMODE         (0),
   fSaveTrees        (0),
   MassSigma         (3),
   MassSigmaAlt      (2),
   OneSigma          (CARBON_MASS_PEAK_SIGMA),
   tshift            (0),
   inj_tshift        (0),
   dx_offset         (0),
   WCMRANGE          (999.05),
   MassLimit         (8),
   nEventsProcessed  (0),
   nEventsTotal      (0),
   thinout           (1),
   fFastCalibThinout (0.10),
   reference_rate    (1),
   //target_count_mm   (0.11),
   target_count_mm   (1),   // Need to get the real value
   fAnaDateTime      (0),
   fAnaTimeReal      (0),
   fAnaTimeCpu       (0),
   fAlphaCalibRun(""), fDlCalibRun(""), fAsymEnv(),
   fFileRunInfo(0), fFileRunConf(0), fFileStdLog(0),
   fFileStdLogName("stdoe.log"), fFlagCopyResults(kFALSE), fFlagUseDb(kFALSE), fFlagUpdateDb(kFALSE),
   fUserGroup(gSystem->GetUserInfo())
{
   Init();
}


/** */
AnaInfo::~AnaInfo()
{
   if (fFileRunInfo) fclose(fFileRunInfo);
   if (fFileRunConf) fclose(fFileRunConf);
   if (fFileStdLog)  fclose(fFileStdLog);

   delete fUserGroup; fUserGroup = 0;
}


/** */
void AnaInfo::Init()
{
   const char* tmpEnv;

   tmpEnv = getenv("CNIPOL_DIR");

   if (tmpEnv) fAsymEnv["CNIPOL_DIR"] = tmpEnv;
   else        fAsymEnv["CNIPOL_DIR"] = ".";

   tmpEnv = getenv("CNIPOL_DATA_DIR");

   if (tmpEnv) fAsymEnv["CNIPOL_DATA_DIR"] = tmpEnv;
   else        fAsymEnv["CNIPOL_DATA_DIR"] = ".";

   tmpEnv = getenv("CNIPOL_RESULTS_DIR");

   if (tmpEnv) fAsymEnv["CNIPOL_RESULTS_DIR"] = tmpEnv;
   else        fAsymEnv["CNIPOL_RESULTS_DIR"] = ".";
}


/** */
void AnaInfo::MakeOutDir()
{
   if (GetOutDir().size() > 200) {
      printf("ERROR: Output directory name is too long\n");
   }

   //printf("s: %s, %s, %s\n", ifile, RunID, fAsymEnv["CNIPOL_RESULTS_DIR"].c_str());

   //umask(0);
   //if (mkdir(GetOutDir().c_str(), 0777) < 0)
   //   printf("WARNING: Perhaps dir already exists: %s\n", GetOutDir().c_str());

   if (gSystem->mkdir(GetOutDir().c_str()) < 0)
      gSystem->Warning("   AnaInfo::AnaInfo", "Directory %s already exists", GetOutDir().c_str());
   else {
      gSystem->Info("   AnaInfo::AnaInfo", "Created directory %s", GetOutDir().c_str());
      gSystem->Chmod(GetOutDir().c_str(), 0775);
   }
}

string AnaInfo::GetRunName()         const { return fRunName; }
string AnaInfo::GetRawDataFileName() const { return fAsymEnv.find("CNIPOL_DATA_DIR")->second + "/" + fRunName + ".data"; }
string AnaInfo::GetImageDir()        const { return GetOutDir() + "/images"; }
string AnaInfo::GetRunInfoFileName() const { return GetOutDir() + "/runconfig.php"; }
string AnaInfo::GetRunConfFileName() const { return GetOutDir() + "/config_calib.dat"; }
string AnaInfo::GetStdLogFileName()  const { return GetOutDir() + "/" + fFileStdLogName; }
string AnaInfo::GetRootFileName()    const { return GetOutDir() + "/" + fRunName + ".root"; }
FILE*  AnaInfo::GetRunInfoFile()     const { return fFileRunInfo; }
FILE*  AnaInfo::GetRunConfFile()     const { return fFileRunConf; }

string AnaInfo::GetAlphaCalibRun()   const { return fAlphaCalibRun; }
string AnaInfo::GetDlCalibRun()      const { return fDlCalibRun; }

Bool_t AnaInfo::HasAlphaBit() const  {
   return (fModes & (AnaInfo::MODE_ALPHA^AnaInfo::MODE_CALIB))  == (AnaInfo::MODE_ALPHA^AnaInfo::MODE_CALIB);
 }

Bool_t AnaInfo::HasCalibBit()   const { return (fModes & AnaInfo::MODE_CALIB)   == AnaInfo::MODE_CALIB; }
Bool_t AnaInfo::HasGraphBit()   const { return (fModes & AnaInfo::MODE_GRAPH)   == AnaInfo::MODE_GRAPH; }
Bool_t AnaInfo::HasNormalBit()  const { return (fModes & AnaInfo::MODE_NORMAL)  == AnaInfo::MODE_NORMAL; }
Bool_t AnaInfo::HasScalerBit()  const { return (fModes & AnaInfo::MODE_SCALER)  == AnaInfo::MODE_SCALER; }
Bool_t AnaInfo::HasRawBit()     const { return (fModes & AnaInfo::MODE_RAW)     == AnaInfo::MODE_RAW; }
Bool_t AnaInfo::HasRunBit()     const { return (fModes & AnaInfo::MODE_RUN)     == AnaInfo::MODE_RUN; }
Bool_t AnaInfo::HasTargetBit()  const { return (fModes & AnaInfo::MODE_TARGET)  == AnaInfo::MODE_TARGET; }
Bool_t AnaInfo::HasProfileBit() const { return (fModes & AnaInfo::MODE_PROFILE) == AnaInfo::MODE_PROFILE; }
Bool_t AnaInfo::HasAsymBit()    const { return (fModes & AnaInfo::MODE_ASYM)    == AnaInfo::MODE_ASYM; }
Bool_t AnaInfo::HasKinematBit() const { return (fModes & AnaInfo::MODE_KINEMAT) == AnaInfo::MODE_KINEMAT; }


string AnaInfo::GetResultsDir() const
{
   return fAsymEnv.find("CNIPOL_RESULTS_DIR")->second;
}


/** */
string AnaInfo::GetOutDir() const
{
   return fAsymEnv.find("CNIPOL_RESULTS_DIR")->second + "/" + fRunName;
}


/** */
string AnaInfo::GetAlphaCalibFile() const
{
   if (fAlphaCalibRun.empty()) {
      cout << "Alpha calibration run not defined" << endl;
      return "";
   }

   string path = fAsymEnv.find("CNIPOL_RESULTS_DIR")->second;
   path += "/" + fAlphaCalibRun + "/" + fAlphaCalibRun + ".root";
   return path;
}


/** */
string AnaInfo::GetDlCalibFile() const
{
   if (fDlCalibRun.empty()) {
      cout << "Dead layer calibration run not defined" << endl;
      return "";
   }

   string path = fAsymEnv.find("CNIPOL_RESULTS_DIR")->second;
   path += "/" + fDlCalibRun + "/" + fDlCalibRun + ".root";
   return path;
}


/** */
string AnaInfo::GetRootTreeFileName(UShort_t trid) const
{
   string filename;
   filename.reserve(GetOutDir().size() + fRunName.size() + 20);
   sprintf(&filename[0], "%s/%s_tree_%02d.root", GetOutDir().c_str(), fRunName.c_str(), trid);
   return filename;
}


/** */
void AnaInfo::ProcessOptions()
{
   // The run name must be specified
   if (fRunName.empty()) {
      gSystem->Error("   AnaInfo::ProcessOptions", "Run name has to be specified");
      PrintUsage();
      exit(0);
   }

   // Check whether the raw data file exists
	TString fileName = fRunName + ".data";

   if (!gSystem->FindFile(fAsymEnv["CNIPOL_DATA_DIR"].c_str(), fileName ) )
	{
      gSystem->Error("   AnaInfo::ProcessOptions",
		   "Raw data file \"%s.data\" not found in %s\n", fRunName.c_str(), fAsymEnv["CNIPOL_DATA_DIR"].c_str());
      exit(0);
	}

   MakeOutDir();

   fFileRunInfo = fopen(GetRunInfoFileName().c_str(), "w");
   gSystem->Chmod(GetRunInfoFileName().c_str(), 0775);

   fFileRunConf = fopen(GetRunConfFileName().c_str(), "w");
   gSystem->Chmod(GetRunConfFileName().c_str(), 0775);

   // Set default standard log output
   if (!fFileStdLogName.empty()) {
      freopen(GetStdLogFileName().c_str(), "w", stdout);
      fclose(stderr);
      stderr = stdout;
      gSystem->Chmod(GetStdLogFileName().c_str(), 0775);
   }

   //freopen(GetStdLogFileName().c_str(), "w", stderr);
   //setbuf(stdout, NULL);
   //fFileStdLogBuf.open(GetStdLogFileName().c_str(), ios::out|ios::ate|ios::app);

   if (HasAlphaBit()) {
      fAlphaCalibRun     = fRunName;
      fDlCalibRun        = "";
      gRunInfo->fMeasType = kMEASTYPE_ALPHA;
   }
}


/** */
TBuffer & operator<<(TBuffer &buf, AnaInfo *&rec)
{
   if (!rec) return buf;
   //printf("operator<<(TBuffer &buf, AnaInfo *rec) : \n");
   rec->Streamer(buf);
   return buf;
}


/** */
TBuffer & operator>>(TBuffer &buf, AnaInfo *&rec)
{
   //if (!rec) return buf;
   //printf("operator<<(TBuffer &buf, AnaInfo *rec) : \n");
   // if object has been created already delete it
   //free(rec);
   //rec = (AnaInfo *) realloc(rec, sizeof(AnaInfo));
   rec->Streamer(buf);
   return buf;
}


/** */
void AnaInfo::Print(const Option_t* opt) const
{
   gSystem->Info("AnaInfo::Print", "Print members:");
   PrintAsPhp();
}


/** */
void AnaInfo::PrintAsPhp(FILE *f) const
{ //{{{
   fprintf(f, "$rc['fRunName']                     = \"%s\";\n", fRunName.c_str());
   fprintf(f, "$rc['enel']                         = %d;\n",     enel);
   fprintf(f, "$rc['eneu']                         = %d;\n",     eneu);
   fprintf(f, "$rc['widthl']                       = %d;\n",     widthl);
   fprintf(f, "$rc['widthu']                       = %d;\n",     widthu);
   fprintf(f, "$rc['fSaveTrees']                   = \"%s\";\n", fSaveTrees.to_string().c_str());
   fprintf(f, "$rc['nEventsProcessed']             = %u;\n",     nEventsProcessed);
   fprintf(f, "$rc['nEventsTotal']                 = %u;\n",     nEventsTotal);
   fprintf(f, "$rc['thinout']                      = %u;\n",     thinout);
   fprintf(f, "$rc['fAnaDateTime']                 = %u;\n",     (UInt_t) fAnaDateTime);
   fprintf(f, "$rc['fAnaTimeReal']                 = %f;\n",     fAnaTimeReal);
   fprintf(f, "$rc['fAnaTimeCpu']                  = %f;\n",     fAnaTimeCpu);
   fprintf(f, "$rc['fAlphaCalibRun']               = \"%s\";\n", fAlphaCalibRun.c_str());
   fprintf(f, "$rc['fDlCalibRun']                  = \"%s\";\n", fDlCalibRun.c_str());

   stringstream ssEnvs("");

   ssEnvs << "array(";

   for (Str2StrMap::const_iterator ienv=fAsymEnv.begin(); ienv!=fAsymEnv.end(); ienv++) {
      ssEnvs << "'" << ienv->first << "'"  << " => " << "\"" << ienv->second << "\"";
      ssEnvs << (ienv != (--fAsymEnv.end()) ? ", " : "");
   }

   ssEnvs << ")";

   fprintf(f, "$rc['fAsymEnv']                     = %s;\n", ssEnvs.str().c_str());

   fprintf(f, "$rc['fFileStdLogName']              = \"%s\";\n", fFileStdLogName.c_str());
   fprintf(f, "$rc['fFlagCopyResults']             = %d;\n", fFlagCopyResults);
   fprintf(f, "$rc['fFlagUseDb']                   = %d;\n", fFlagUseDb);
   fprintf(f, "$rc['fFlagUpdateDb']                = %d;\n", fFlagUpdateDb);

   fprintf(f, "$rc['fUserGroup_fUser']             = \"%s\";\n", fUserGroup->fUser.Data());
   fprintf(f, "$rc['fUserGroup_fRealName']         = \"%s\";\n", fUserGroup->fRealName.Data());

   // Various printouts. Should be combined with Print()?
   //cout << "Input data file:               " << GetRawDataFileName() << endl;
   //cout << "Max events to process:         " << gMaxEventsUser << endl;
   fprintf(f, "\n");
} //}}}


/** */
void AnaInfo::PrintUsage()
{
   cout << endl;
   cout << "Options:" << endl;
   cout << " -h, -?                          : Print this help" << endl;
   cout << " -r, -f, --run-name <run_name>   : Name of run with raw data in $CNIPOL_DATA_DIR directory" << endl;
   cout << " -n <number>                     : Maximum number of events to process"
        << " (default \"-n 0\" all events)" << endl;
   cout << " -s <number>                     : Only every <number> event will be"
        << " processed (default \"-s 1\" no skip)" << endl;
   cout << " -o <filename>                   : Output hbk file (!)" << endl;
   //cout << " -r <filename>                   : ramp timing file" << endl;
   cout << " -l, --log=[filename]            : Optional log file to redirect stdout and stderr" << endl;
   cout << " -t <time shift>                 : TOF timing shift in [ns], addition to TSHIFT defined in run.db (!)" << endl;
   cout << " -e <lower:upper>                : Kinetic energy range (default [400:900] keV) (!)" << endl;
   //cout << " -B                              : create banana curve on" << endl;
   //cout << " -G                              : mass mode on " << endl;
   cout << " -a, --no-error-detector         : Anomaly check off (!)" << endl;
   cout << " -b                              : Feedback mode on (!)" << endl;
   cout << " -D                              : Dead layer mode on (!)" << endl;
   cout << " -d <dlayer>                     : Additional deadlayer thickness [ug/cm2] (!)" << endl;
   //cout << " -T                              : T0 study    mode on " << endl;
   //cout << " -A                              : A0,A1 study mode on " << endl;
   //cout << " -Z                              : without T0 subtraction" << endl;
   cout << " -F <file>                       : Overwrite conf file defined in run.db (!)" << endl;
   cout << " -W <lower:upper>                : Const width banana cut (!)" << endl;
   cout << " -m <sigma>                      : Banana cut by <sigma> from 12C mass [def]:3 sigma (!)" << endl;
   cout << " -U                              : Update histogram" << endl;
   cout << " -N                              : Store Ntuple events (!)" << endl;
   cout << " -R <bitmask>                    : Save events in Root trees, " <<
           "e.g. \"-R 101\"" << endl;
   cout << " -q, --quick                     : Skips the main loop. Use for a quick check" << endl;
   cout << " -C, --mode-alpha, --alpha       : Use when run over alpha run data" << endl;
   cout << "     --mode-calib, --calib       : Update calibration constants" << endl;
   cout << "     --mode-normal               : Default set of histograms" << endl;
   cout << "     --mode-no-normal            : Turn off the default set of histograms" << endl;
   cout << "     --mode-scaler, --scaler     : Fill and save scaler histograms (from V124 memory)" << endl;
   cout << "     --mode-raw, --raw           : Fill and save raw histograms" << endl;
   cout << "     --mode-run                  : Fill and save bunch, lumi and other run related histograms" << endl;
   cout << "     --mode-target, --target     : Fill and save target histograms" << endl;
   cout << "     --mode-full                 : Fill and save all histograms" << endl;
   cout << " -g, --graph                     : Save histograms as images" << endl;
   cout << "     --copy                      : Copy results to server (?)" << endl;
   cout << "     --use-db                    : Run info will be retrieved from and saved into database" << endl;
   cout << "     --update-db                 : Update run info in database" << endl;
   cout << endl;
   cout << "Options marked with (!) are not really supported" << endl;
   cout << "Options marked with (?) need more work" << endl;
   cout << endl;
}


/** */
void AnaInfo::Streamer(TBuffer &buf)
{
   TString tstr;

   if (buf.IsReading()) {
      //printf("reading AnaInfo::Streamer(TBuffer &buf) \n");
      buf >> tstr; fRunName = tstr.Data();
      buf >> enel;
      buf >> eneu;
      buf >> widthl;
      buf >> widthu;
      buf >> fModes;
      buf >> nEventsProcessed;
      buf >> nEventsTotal;
      buf >> thinout;
      buf >> fFastCalibThinout;
      buf >> fAnaDateTime >> fAnaTimeReal >> fAnaTimeCpu;
      buf >> tstr; fAlphaCalibRun  = tstr.Data();
      buf >> tstr; fDlCalibRun     = tstr.Data();
      buf >> tstr; fFileStdLogName = tstr.Data();
      buf >> fFlagCopyResults >> fFlagUseDb >> fFlagUpdateDb;
      //buf >> fUserGroup;
   } else {
      //printf("writing AnaInfo::Streamer(TBuffer &buf) \n");
      tstr = fRunName; buf << tstr;
      buf << enel;
      buf << eneu;
      buf << widthl;
      buf << widthu;
      buf << fModes;
      buf << nEventsProcessed;
      buf << nEventsTotal;
      buf << thinout;
      buf << fFastCalibThinout;
      buf << fAnaDateTime << fAnaTimeReal << fAnaTimeCpu;
      tstr = fAlphaCalibRun;  buf << tstr;
      tstr = fDlCalibRun;     buf << tstr;
      tstr = fFileStdLogName; buf << tstr;
      buf << fFlagCopyResults << fFlagUseDb << fFlagUpdateDb;
      //buf << fUserGroup;
   }
}


/** Deprecated. */
/*
void AnaInfo::Update(DbEntry &rundb)
{
   // If user didn't specify his/her calibration file use the one from the DB
   if (fAlphaCalibRun.empty())
      fAlphaCalibRun = rundb.fFields["ALPHA_CALIB_RUN_NAME"];

   if (fDlCalibRun.empty())
      fDlCalibRun = rundb.fFields["DL_CALIB_RUN_NAME"];
}
*/


/** */
void AnaInfo::Update(MseRunInfoX& run)
{
	// A fix for alpha calib runs - Maybe this should go to the process options
   // method
   if (HasAlphaBit()) {
      //fAlphaCalibRun = fRunName;
      fAlphaCalibRun           = "";
      fDlCalibRun              = "";
      run.alpha_calib_run_name = "";
      run.dl_calib_run_name    = "";
   }

   if (!fAlphaCalibRun.empty())
      run.alpha_calib_run_name = fAlphaCalibRun;
   else if (!run.alpha_calib_run_name.empty())
      fAlphaCalibRun = run.alpha_calib_run_name;

   if (fAlphaCalibRun.empty()) {
      if (!HasAlphaBit()) {
         gSystem->Error("   AnaInfo::Update", "Alpha calibration run must be specified");
         exit(0);
      }
   } else
      gSystem->Info("   AnaInfo::Update", "Using alpha calibration run %s", fAlphaCalibRun.c_str());


   // Set DL calib files
   if (HasCalibBit()) {
      fDlCalibRun           = "";
      run.dl_calib_run_name = "";
   }

   if (!fDlCalibRun.empty())
      run.dl_calib_run_name = fDlCalibRun;
   else if (!run.dl_calib_run_name.empty())
      fDlCalibRun = run.dl_calib_run_name;

   if (fDlCalibRun.empty()) {
      if (!HasCalibBit()) {
         gSystem->Warning("   AnaInfo::Update", "Calibration run is not specified.\n" \
                          "\tOption --calib should be used");
      }
   } else {
      gSystem->Info("   AnaInfo::Update", "Using calibration run %s", run.dl_calib_run_name.c_str());
   }
}


/** */
void AnaInfo::CopyResults()
{
   if (!fFlagCopyResults) return;

   string cmd = "rsync -av " + GetOutDir() + " bluepc:/usr/local/polarim/root/";
   //string cmd = "ls -l";

   //system(cmd.c_str());
   char result[1000];
   FILE *fp = popen( cmd.c_str(), "r");

   while (fgets(result, sizeof(result), fp) != NULL ) { printf("%s", result); }
   //fread(result, 1, sizeof(result), fp);
   //printf("%s", result);

   pclose(fp);
}
