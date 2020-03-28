#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, double money, int dishes): Event(eTime, oID)
{
	OrdType = oType;
	OrdMoney = money;
	OrdDishes = dishes;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1
	Order* pOrd = new Order(EventTime, OrderID, OrdType, OrdMoney, OrdDishes);

	switch (pRest->getMode()) {  //MOEMEN: I added this switch-case statement so that the DEMO mode would remain functioning for testing
	case MODE_INTR: {

		switch (OrdType) {
		case TYPE_NRM:
			pRest->AddtoNormalOrders(pOrd);
			break;
		case TYPE_VGAN:
			pRest->AddtoVeganOrders(pOrd);
			break;
		case TYPE_VIP:
			pRest->AddtoVIPOrders(pOrd);
		}

		break;
	}

	case MODE_DEMO: {
		pRest->AddtoDemoQueue(pOrd);
	}
	}
}
