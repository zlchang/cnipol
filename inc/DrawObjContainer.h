/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#ifndef DrawObjContainer_h
#define DrawObjContainer_h

#include <map>
#include <iostream>

#include "TCanvas.h"
#include "TClass.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH2F.h"
#include "TKey.h"
#include "TObject.h"
#include "TPaveStats.h"
#include "TPRegexp.h"
#include "TSystem.h"

//#include "ChannelEvent.h"

class ChannelEvent;
class DrawObjContainer;

typedef std::map<std::string, TObject*> ObjMap;
typedef std::map<std::string, TObject*>::iterator ObjMapIter;
typedef std::map<std::string, TObject*>::const_iterator ObjMapConstIter;
typedef std::map<std::string, DrawObjContainer*> DrawObjContainerMap;
typedef std::map<std::string, DrawObjContainer*>::iterator DrawObjContainerMapIter;
typedef std::map<std::string, DrawObjContainer*>::const_iterator DrawObjContainerMapConstIter;


/** */
class DrawObjContainer : public TObject
{
public:

   //TFile  *f;
   TDirectory          *fDir;
   ObjMap               o;
   DrawObjContainerMap  d;

public:

   DrawObjContainer();
   DrawObjContainer(TDirectory *dir);
   virtual ~DrawObjContainer();

   void  SetDir(TDirectory *dir);
   void  ReadFromDir();
   void  ReadFromDir(TDirectory *dir);
   void  Add(DrawObjContainer* oc);
   //virtual void Print(const Option_t* opt="") const;
   virtual void Fill(ChannelEvent *ch, std::string cutid="");
   virtual void FillPreProcess(ChannelEvent *ch);
   void  Print(const Option_t* opt="") const;
   virtual void PreFill(std::string sid="");
   virtual void PostFill();
   virtual void SaveAllAs(TCanvas &c, std::string pattern="^.*$", std::string path="./");
   void  Draw(Option_t* option = "")
      { ((TObject*)this)->Draw(option); }
   void  Draw(TCanvas &c);
   Int_t Write(const char* name = 0, Int_t option = 0, Int_t bufsize = 0);
   Int_t Write(const char* name = 0, Int_t option = 0, Int_t bufsize = 0) const;
   void  Delete(Option_t* option="");

   ClassDef(DrawObjContainer, 1)
};

#endif
