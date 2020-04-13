#include "PromoteEvent.h"
#include "..\Rest\Restaurant.h"


PromoteEvent::PromoteEvent(int eTime, int oID, double oMoney) : Event(eTime, oID)
{
	extraMoney = oMoney;
}

void PromoteEvent::Execute(Restaurant* pRest)
{
	//Only Normal Orders can be promoted so we first check the order type using its ID
	if (pRest->typeFinder(OrderID) != TYPE_NRM) return;
	
	//Call the promotion function
	pRest->PromoteNormalOrder(OrderID, extraMoney);

}


