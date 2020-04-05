#include <cstdlib>
#include <time.h>
#include <cstdio>
#include <iostream>
#include <math.h>
#include "Restaurant.h"




Restaurant::Restaurant()
{
	pGUI = NULL;
}

Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
	delete pLoad;
}

///////////////////////////////////// FUNCTION TO RUN RESTAURANT SIMULATION /////////////////
void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	pGUI->PrintMessage("Welcome to your restaurant!");
	pLoad->Excuete(); //Takes event file from user and loads it into restaurant
	Sleep(1500);
	mode = pGUI->getGUIMode();

	switch (mode)	
	{
	  case MODE_INTR:
		  modeInteractive(); 
		  break;

	  case MODE_STEP:
		  modeStep(); 
		  break; 

	  case MODE_SLNT:
		  modeSilent(); 
		  break; 	
	}
	//End program and print statistics
	pGUI->PrintMessage("This is was all for your restaurant!", 2);
	string line3 = "Orders: " + to_string(NOrdersCount + GOrdersCount + VOrdersCount)
		+ " [ Norm: " + to_string(NOrdersCount) + ", Veg: " + to_string(GOrdersCount) +
		", VIP: " + to_string(VOrdersCount) + " ]"; 
	string line4 = "Cooks: " + to_string(NCooksCount + GCooksCount + VCooksCount)
		+ " [ Norm: " + to_string(NCooksCount) + ", Veg: " + to_string(GCooksCount) +
		", VIP: " + to_string(VCooksCount) + " ]";
	string line5 = "Avg Wait = " + to_string(Avg_wait) 
		+ ", " + "Avg Serv = " + to_string(Avg_serv); 	
	string line6 = "Auto-promoted = " + to_string(Auto_promoted_count); 
	//In next phase, print lines 3, 4, 5 to file too
	pGUI->PrintMessage(line3, 3, 0);
	pGUI->PrintMessage(line4, 4, 0);
	pGUI->PrintMessage(line5, 5, 0);
	pGUI->PrintMessage(line6, 6, 0); 
	pGUI->waitForClick();
//	Sleep(8000); 
}

///////////////////////////// RESTAURANT SETTERS AND GETTERS ///////////////////////
void Restaurant::setAuto_p(int p)
{
	Auto_p = p;
}

int Restaurant::getAuto_p()
{
	return Auto_p;
}

void Restaurant::setCooksCount(int n, int g, int v)
{
	NCooksCount = n; 
	GCooksCount = g; 
	VCooksCount = v; 
}

int Restaurant::getNCooksCount()
{
	return NCooksCount; 
}
int Restaurant::getGCooksCount()
{
	return GCooksCount; 
}

int Restaurant::getVCooksCount()
{
	return VCooksCount; 
}

GUI* Restaurant::GetGUI()
{
	return pGUI;
}

PROG_MODE Restaurant::getMode() {
	return mode;
}


//Determines type of order 
ORD_TYPE Restaurant::typeFinder(int key) {
	if (normalOrders.Find(key)) return TYPE_NRM;
	if (VIPOrders.Find(key)) return TYPE_VIP;
	else return TYPE_VGAN;
}
///////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////// ADD TO COOKS FUNCTIONS  ////////////////////////////

void Restaurant::AddNormalCook(Cook* pC)
{
	if (pC->GetType() == TYPE_NRM)
	{
		normalCooks.enqueue(pC);
		availableCooks.InsertEnd(pC);
	} 
}

void Restaurant::AddVeganCook(Cook* pC)
{
	if (pC->GetType() == TYPE_VGAN)
	{
		veganCooks.enqueue(pC);
		availableCooks.InsertEnd(pC);
	}
}
void Restaurant::AddVIPCook(Cook* pC)
{
	if (pC->GetType() == TYPE_VIP)
	{
		VIPCooks.enqueue(pC);
		availableCooks.InsertEnd(pC);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  ADD TO ORDERS FUNCTIONS  /////////////////////////////

void Restaurant::AddtoNormalOrders(Order* po) {
	normalOrders.InsertEnd(po);
	NOrdersCount++; 
	waitingOrders.InsertEnd(po);
}

void Restaurant::AddtoVeganOrders(Order* po) {
	veganOrders.enqueue(po);
	GOrdersCount++; 
	veganInWait++; 
	waitingOrders.InsertEnd(po);
}

void Restaurant::AddtoVIPOrders(Order* po) {
	VIPOrders.InsertEnd(po);
	VOrdersCount++; 
	waitingOrders.InsertEnd(po);
}

void Restaurant::AddtoFinishedOrders(Order* po, int TimeStep) {

	if (po)
	{
		finishedOrdersCount++;
		//Adds finished order's service and waiting time to average services and waiting 
		//times multiplied by number of previously finished orders
		Avg_wait = Avg_wait * finishedOrdersCount +( double(po->getServTime()) - (double)po->getArrTime());
		Avg_serv = Avg_serv * finishedOrdersCount + (double(TimeStep) - (double)po->getServTime());

		pGUI->DrawFinishedOrder(po);

		//Divides sum of average waiting and service times calculated above by new finished orders count
		Avg_wait = Avg_wait / finishedOrdersCount;
		Avg_serv = Avg_serv / finishedOrdersCount;
		delete po;
	}
}

void Restaurant::AddtoInserviceOrders(Order* po, int TimeStep)
{
	//Decrement vegan orders in wait counter
	if (po->GetType() == TYPE_VGAN)
		veganInWait--; 
	//Set status to in-service 
	po->setStatus(SRV); 
	//Set servicing time 
	po->setServTime(TimeStep); 
	//In next phase use TimeStep + ceil of order's ndishes/ assigned cook's speed to set order's FinishTime here
	//Inserts to list of inservice orders
	inServiceOrders.InsertEnd(po);
	
	//In next phase this function will be renamed AddtoInserviceOrdersCooks; will take both order and assigned cook; will set order FinishTime, CookFinishTime; will move cook to inservice cooks
}

/////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  EVENT HANDLING FUNCTIONS   /////////////////////////////

//Adds events to event queue

void Restaurant::AddEvent(Event* pEvent)
{
	EventsQueue.enqueue(pEvent);
}


void Restaurant::CancelNormalOrder(int id) {
	normalOrders.DeleteNode(id); 
	waitingOrders.DeleteNode(id); 
}

void Restaurant::PromoteNormalOrder(int id, double extraMoney) {
	//Move from normal orders list to VIP orders list
	Order* oldNormal = normalOrders.ReturnAndRemove(id);
	if (oldNormal)
	{
		Order* newVIP = new Order(oldNormal->getArrTime(), id, TYPE_VIP, oldNormal->getTotalMoney() + extraMoney, oldNormal->getNDishes());
		VIPOrders.InsertEnd(newVIP);
		//Change type in waiting orders list 
		Node<Order*>* originalOrderNode = waitingOrders.Return(id);
		originalOrderNode->setItem(newVIP);
	}

}

////////////////////////////////////////////////////////////////////////////////////

//////////////////////  ADD TO GUI DRAWING LIST FUNCTION  /////////////////////////
void Restaurant::FillDrawingList()
{

	//Initialize pointers to IDholder and Order 
	Order* pOrd;
	Cook* pCook; 

	//Add inservice orders from linked list of inservice orders 
	Node<Order*>* P = inServiceOrders.getHead();
	while (P) {
		pOrd = P->getItem();
		pGUI->AddToDrawingList(pOrd, SRV);
		P = P->getNext();
	};

	//Add waiting Orders 
	P = waitingOrders.getHead();
	while (P) {
		pOrd = P->getItem();
		if (pOrd)
			if (pOrd -> getStatus() == WAIT)
				pGUI->AddToDrawingList(pOrd, WAIT);
		P = P->getNext();
	};

	//Add available cooks 
	Node<Cook*>* C = availableCooks.getHead();
	while (C) {
		pCook = C->getItem();
		pGUI->AddToDrawingList(pCook);
		C = C->getNext();
	};
}

///////////////////////// SIMULATION MODES FUNCTIONS /////////////////////////////////

void Restaurant::modeInteractive()
{
	//TimeStep counter and char to print it 
	int CurrentTimeStep = 1;
	char timestep[10];

	/* Loop till all events are excueted and all orders are finished:

	The loop's condition checks that all events' have been excueted, no orders are in service, 
	and no orders are in any of the waiting lists. The last check that no orders are in the 
	waiting list is necassary for the case when all events are excueted and all cooks are in breaks,
	hence although the waiting lists have are not empty, no orders are being serviced.

	*/
	while (!EventsQueue.isEmpty() || inServiceOrders.getHead() || normalOrders.getHead() ||veganInWait != 0 || VIPOrders.getHead())
	{
		//print current timestep
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);
		//Exceute events in this timestep
		ExecuteEvents(CurrentTimeStep);
		//Serve Orders
		ServeOrders(CurrentTimeStep);
		//Assign Orders
		AssignOrders(CurrentTimeStep);
		//Update GUI
		FillDrawingList();
		pGUI->UpdateInterface();
		pGUI->ResetDrawingList();
		//Update Status bar
		Statusbar(); 
		//Wait for user click and advance timestep
		pGUI->waitForClick();
		CurrentTimeStep++;	
	}
}

void Restaurant::modeStep()
{
	//TimeStep counter and char to print it 
	int CurrentTimeStep = 1;
	char timestep[10];

	//Loop till all events are excueted and all orders are finished 
	while (!EventsQueue.isEmpty() || inServiceOrders.getHead() || normalOrders.getHead() || veganInWait != 0 || VIPOrders.getHead())
	{
		//print current timestep
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);
		//Excuete events in this time step
		ExecuteEvents(CurrentTimeStep);
		//Serve Orders
		ServeOrders(CurrentTimeStep);
		//Assign Orders
		AssignOrders(CurrentTimeStep);
		//Update GUI
		FillDrawingList();
		pGUI->UpdateInterface();
		pGUI->ResetDrawingList();
		//Update statusbar 
		Statusbar(); 
		//Wait 1 second and advance timestep 
		Sleep(1000);
		CurrentTimeStep++;	
	}
}

void Restaurant::modeSilent()
{
	//TimeStep counter  
	int CurrentTimeStep = 1;

	//Loop till all events are excueted and all orders are finished 
	while (!EventsQueue.isEmpty() || inServiceOrders.getHead() || normalOrders.getHead() || veganInWait != 0 || VIPOrders.getHead())
	{
		//Excuete events in this time step
		ExecuteEvents(CurrentTimeStep);
		//Assign Orders
		AssignOrders(CurrentTimeStep); 
		//Serve Orders
		ServeOrders(CurrentTimeStep); 
		//Advance timestep
		CurrentTimeStep++;	
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////

///////////////////////// SIMULATION TIMESTEP FUNCTIONS //////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}

//Moves waiting orders to inservice orders
void Restaurant::AssignOrders(int TimeStep)
{
	Order* pOrd; 
	//Auto-promote normal orders that have waited more than Auto_p time steps
	while (normalOrders.getHead()) 
	{
		if (TimeStep - normalOrders.getHead()->getItem()->getArrTime() == Auto_p)
		{
			PromoteNormalOrder(normalOrders.getHead()->getItem()->GetID(), 0);
			Auto_promoted_count++; 
		}
		else break; 
	}
	//Assign 1 normal order
	if (normalOrders.getHead())
	{
		pOrd = normalOrders.DeleteAndReturnFirst();
		AddtoInserviceOrders(pOrd, TimeStep);
	} 
	//Assign 1 vegan order
	if (!veganOrders.isEmpty())
	{
		veganOrders.dequeue(pOrd);
		AddtoInserviceOrders(pOrd, TimeStep);
	}
	//Assign 1 VIP order
	if (VIPOrders.getHead())
	{
		pOrd = VIPOrders.DeleteAndReturnLargest(1);
		AddtoInserviceOrders(pOrd, TimeStep);
	}
}

//Moves inservice orders to finished orders
void Restaurant::ServeOrders(int TimeStep)
{
	if (TimeStep % 5 != 0)
		return; 
	Order* pOrd;
	//Serve 1 normal order
	if (inServiceOrders.getHead())
	{
		pOrd = inServiceOrders.ReturnAndRemove(TYPE_NRM);
		if(pOrd)
			AddtoFinishedOrders(pOrd, TimeStep);
	}
	//Serve 1 vegan order
	if (inServiceOrders.getHead())
	{
		pOrd = inServiceOrders.ReturnAndRemove(TYPE_VGAN);
		if(pOrd)
			AddtoFinishedOrders(pOrd, TimeStep);
	}
	//Serve 1 VIP order
	if (inServiceOrders.getHead())
	{
		pOrd = inServiceOrders.ReturnAndRemove(TYPE_VIP);
		if(pOrd)
			AddtoFinishedOrders(pOrd, TimeStep);
	}
}

void Restaurant::Statusbar()
{
	int NO = normalOrders.getCount(); 
	int VO = VIPOrders.getCount();
	string line3 = "Orders Waiting: " + to_string(NO + VO + veganInWait)
		+ " [ Norm: " + to_string(NO) + ", Veg: " + to_string(veganInWait) +
		", VIP: " + to_string(VO) + " ]";
	string line4 = "Cooks: " + to_string(NCooksCount + GCooksCount + VCooksCount)
		+ " [ Norm: " + to_string(NCooksCount) + ", Veg: " + to_string(GCooksCount) +
		", VIP: " + to_string(VCooksCount) + " ]";
	pGUI->PrintMessage(line3, 3, 0); 
	pGUI->PrintMessage(line4, 4, 0);

	
}

//////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////// RESTAURANT ENDS HERE //////////////////////////////////////////////
