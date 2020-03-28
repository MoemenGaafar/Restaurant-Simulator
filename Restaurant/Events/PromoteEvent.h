#pragma once

#include "Event.h"


//class for the cancellation event of normal orders
class PromoteEvent : public Event
{
private:
	double extraMoney;
public:
	PromoteEvent(int eTime, int oIDm, double oMoney);
	virtual void Execute(Restaurant* pRest);	//override execute function
};

