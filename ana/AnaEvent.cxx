/*****************************************************************************
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "AnaEvent.h"

ClassImp(AnaEvent)

using namespace std;

/** Default constructor. */
AnaEvent::AnaEvent() : TObject(), fEventId(), fChannels()
{
};


/** Default destructor. */
AnaEvent::~AnaEvent()
{
};


/**
 *
 */
void AnaEvent::Print(const Option_t* opt) const
{
   opt = "";

   printf("AnaEvent:\n");
}
