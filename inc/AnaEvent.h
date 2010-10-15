/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#ifndef AnaEvent_h
#define AnaEvent_h

#include <map>
#include <vector>

#include "TObject.h"

#include "AnaEventId.h"
#include "ChannelData.h"

class AnaEvent;

typedef std::vector<AnaEvent> AnaEventVec;
typedef std::map<AnaEventId, AnaEvent> AnaEventMap;

/**
 *
 */
class AnaEvent : public TObject
{
public:

	AnaEventId     fEventId;
   //GntChannelVec fChannels;
   ChannelDataMap fChannels;

public:

   AnaEvent();
   ~AnaEvent();

   virtual void Print(const Option_t* opt="") const;

   ClassDef(AnaEvent, 1)
};

#endif
