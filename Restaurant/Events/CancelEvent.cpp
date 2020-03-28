#include "CancelEvent.h"
#include "..\Rest\Restaurant.h"


CancelEvent::CancelEvent(int eTime, int oID): Event(eTime, oID)
{
}

void CancelEvent::Execute(Restaurant* pRest)
{
	//Only Normal Orders can be cancelled so we first check the order type using its ID
	if (pRest->typeFinder(OrderID) != TYPE_NRM) return;
	
	//Call the cancellation function
	pRest->CancelNormalOrder(OrderID);
}


