#pragma once

#include "Event.h"


//class for the cancellation event of normal orders
class CancelEvent: public Event
{
public:
	CancelEvent(int eTime, int oID);
	virtual void Execute(Restaurant *pRest);	//override execute function
};

