#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"


//class for the arrival event
class ArrivalEvent: public Event
{
	//info about the order ralted to arrival event
	ORD_TYPE OrdType;		//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
	int OrdDishes;
public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType, double money, int dishes);
	//Add more constructors if needed
	
	virtual void Execute(Restaurant *pRest);	//override execute function

};

#endif