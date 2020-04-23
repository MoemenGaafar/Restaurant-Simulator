#include <cstdlib>
#include <time.h>
#include <cstdio>
#include <iostream>
#include <math.h>
#include<fstream>
#include<iomanip>
#include "Restaurant.h"


ofstream outputfile;

Restaurant::Restaurant():VIPOrders(500), normalCooks(500), VIPCooks(500), veganCooks(500)
{
	pGUI = NULL;
	srand(time(NULL));
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
	outputfile.open("output.txt");
	outputfile << setw(6) << left << "FT" << setw(6) << left << "ID" << setw(6) << left << "AT" << setw(6) << left << "WT" << setw(6) << left << "ST" << endl;
	pGUI = new GUI;
	pGUI->PrintMessage("Welcome to your restaurant!");
	pLoad->Excuete(); //Takes event file from user and loads it into restaurant
	Sleep(1500);
	mode = pGUI->getGUIMode();
	availableNCooks = NCooksCount;
	availableGCooks = GCooksCount;
	availableVCooks = VCooksCount;

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
	pGUI->PrintMessage(line3, 3, 0);
	pGUI->PrintMessage(line4, 4, 0);
	pGUI->PrintMessage(line5, 5, 0);
	pGUI->PrintMessage(line6, 6, 0); 
	Sleep(4000);
	outputfile << line3 << endl;
	outputfile << line4 << endl;
	outputfile << line5 << endl;
	outputfile << line6 << endl;
	outputfile.close();
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
	else return TYPE_VGAN;
}
///////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////// ADD TO COOKS FUNCTIONS  ////////////////////////////

void Restaurant::AddNormalCook(Cook* pC)
{
	if (pC->GetType() == TYPE_NRM)
	{
		normalCooks.Insert(pC);
		availableCooks.InsertEnd(pC);
	} 
}

void Restaurant::AddVeganCook(Cook* pC)
{
	if (pC->GetType() == TYPE_VGAN)
	{
		veganCooks.Insert(pC);
		availableCooks.InsertEnd(pC);
	}
}
void Restaurant::AddVIPCook(Cook* pC)
{
	if (pC->GetType() == TYPE_VIP)
	{
		VIPCooks.Insert(pC);
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
	VIPOrders.Insert(po);
	VOrdersCount++; 
	waitingOrders.InsertEnd(po);
}

void Restaurant::AddtoFinishedOrders(Order* po, int TimeStep) {

	if (po)
	{
		
		//Adds finished order's service and waiting time to average services and waiting 
		//times multiplied by number of previously finished orders
		Avg_wait = Avg_wait * finishedOrdersCount +( double(po->getServTime()) - (double)po->getArrTime());
		Avg_serv = Avg_serv * finishedOrdersCount + (double(TimeStep) - (double)po->getServTime());


		//output file 
		int ST = TimeStep - po->getServTime();
		int WT =po->getServTime()- po->getArrTime();
		outputfile << setw(6) << left <<po->getFinishTime()<< setw(6) << left << po->GetID() << setw(6) << left << po->getArrTime() << setw(6) << left << WT<< setw(6) << left <<ST<< endl;
		//Draw the finished order
		if (mode != MODE_SLNT)
			pGUI->DrawFinishedOrder(po);

		//Increment the finished orders count
		finishedOrdersCount++;

		//Divides sum of average waiting and service times calculated above by new finished orders count
		Avg_wait = Avg_wait / finishedOrdersCount;
		Avg_serv = Avg_serv / finishedOrdersCount;

		//Increment served orders counters
		switch (po->GetType()) {
		case TYPE_NRM: doneNOrders++; break;
		case TYPE_VGAN: doneGOrders++; break;
		case TYPE_VIP: doneVOrders++; break;
		}
		delete po;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  COOKS FUNCTIONS  /////////////////////////////////////

void Restaurant::AddtoInserviceOrdersCooks(Order* po, Cook* pc, int TimeStep)
{
	//ORDER
	//Decrement vegan orders in wait counter
	if (po->GetType() == TYPE_VGAN)
		veganInWait--; 
	//Set status to in-service 
	po->setStatus(SRV); 
	//Set servicing time
	po->setServTime(TimeStep); 
	//Calculate order finish time
	int finishtime = TimeStep + ceil(po->getNDishes() / (double)pc->getSpeed());
	//Set order finish time
	po->setFinishTime(finishtime);

	
	//COOK
	//Decrement the ordersTillBreak counter of the cook by one
	pc->minusOrdersTillBreak();
	//Check if the cook requires a break after the order and add it to the cook's finish time
	if (pc->getOrdersTillBreak() == 0) finishtime += pc->getBreakDuration();
	//Set the cook's finish time
	pc->setFinishTime(finishtime);
	//Remove the cook from the drawing list
	availableCooks.DeleteNode(*pc);

	//Determine if cook will get injured while preparing this order 
	if (pc->getHealthTime() == -1) // assuming an injured cook will be extra careful to not get injured again
	{
		int Injury = rand() % OrdersPerInjury; //assuming the probablity that a cook gets injured while preparing an order in 1/OrdersPerInjury percent
		if (Injury == 0)
			pc->setHealthTime(0);
	}

	//Insert to list of inservice orders and idle cooks
	inServiceOrders.InsertEnd(po);
	idleCooks.InsertEnd(pc);
	
}

void Restaurant::ReturntoAvailableCooks(Cook* pc, int CurrentTimeStep) {
	//If the cook is returning from a break, reset his ordersTillBreak counter
	if (pc->getOrdersTillBreak() == 0)
	{
		pc->resetOrderstillBreak();
		if (pc->getHealthTime() > 0)
		{
			if (pc -> speedIsOdd)
				pc->setSpeed(1 + pc->getSpeed() * 2);
			else 
				pc->setSpeed(pc->getSpeed() * 2);
			pc->setHealthTime(-1); 
		}
	}

	//If the cook gor injured while preparing last order, send it to respective list of injured cooks
	if (pc->getHealthTime() == 0)
	{
		switch (pc->GetType()) {
		case TYPE_NRM:
			InormalCooks.enqueue(pc);
			injuredNCooks++;  break;
		case TYPE_VGAN:
			IveganCooks.enqueue(pc);
			injuredGCooks++; break;
		case TYPE_VIP:
			IVIPCooks.enqueue(pc);
			injuredVCooks++;
		}

		// Set time the cook will be treated to current time + time for full treatment
		pc->setHealthTime(CurrentTimeStep + TreatmentTime); 

	}
	//Otherwise return cook it to the available cooks lists
	else
	{
		availableCooks.InsertEnd(pc);
		//Increment the counters of available cooks
		switch (pc->GetType()) {
		case TYPE_NRM:
			normalCooks.Insert(pc);
			availableNCooks++;  break;
		case TYPE_VGAN:
			veganCooks.Insert(pc);
			availableGCooks++; break;
		case TYPE_VIP:
			VIPCooks.Insert(pc);
			availableVCooks++;
		}
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  EVENT HANDLING FUNCTIONS   /////////////////////////////

//Adds events to event queue

void Restaurant::AddEvent(Event* pEvent)
{
	EventsQueue.enqueue(pEvent);
}


void Restaurant::CancelNormalOrder(int id) {
	Node <Order*>* pNode = normalOrders.Return(id); 
	Order* pOrd = pNode->getItem(); 
	normalOrders.DeleteNode(id);
	delete pOrd; 
}

void Restaurant::PromoteNormalOrder(int id, double extraMoney) {
	//Move from normal orders list to VIP orders list
	Order* oldNormal = normalOrders.ReturnAndRemove(id);
	if (oldNormal)
	{
		Order* newVIP = new Order(oldNormal->getArrTime(), id, TYPE_VIP, oldNormal->getTotalMoney() + extraMoney, oldNormal->getNDishes());
		VIPOrders.Insert(newVIP);
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
		if (pOrd -> getStatus() == WAIT && pOrd -> GetID() > 0)
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
	while (!EventsQueue.isEmpty() || inServiceOrders.getHead() || normalOrders.getHead() || veganInWait != 0 || !VIPOrders.isEmpty())
	{
		//print current timestep
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);
		//Exceute events in this timestep
		ExecuteEvents(CurrentTimeStep);
		//Serve Orders
		ServeOrders(CurrentTimeStep);
		//Return Available Cooks
		FreeCooks(CurrentTimeStep);
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
	while (!EventsQueue.isEmpty() || inServiceOrders.getHead() || normalOrders.getHead() || veganInWait != 0 || !VIPOrders.isEmpty())
	{
		//print current timestep
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);
		//Excuete events in this time step
		ExecuteEvents(CurrentTimeStep);
		//Serve Orders
		ServeOrders(CurrentTimeStep);
		//Return Available Cooks
		FreeCooks(CurrentTimeStep);
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
	while (!EventsQueue.isEmpty() || inServiceOrders.getHead() || normalOrders.getHead() || veganInWait != 0 || !VIPOrders.isEmpty())
	{
		//Excuete events in this time step
		ExecuteEvents(CurrentTimeStep);
		//Assign Orders
		AssignOrders(CurrentTimeStep); 
		//Return Available Cooks
		FreeCooks(CurrentTimeStep);
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

//Returns healed cooks from injured lists to available lists
void Restaurant::ReturnHealedCooks(int CurrentTimeStep)
{
	Cook* pCook;
	while (!InormalCooks.isEmpty())
	{
		InormalCooks.peekFront(pCook); 
		if (CurrentTimeStep == pCook->getHealthTime())
		{
			InormalCooks.dequeue(pCook);
			injuredNCooks--;
			pCook->setHealthTime(-1);
			normalCooks.Insert(pCook);
			availableNCooks++;			
			availableCooks.InsertEnd(pCook);
		}
		else break;
	}
	while (!IVIPCooks.isEmpty())
	{
		IVIPCooks.peekFront(pCook);
		if (CurrentTimeStep == pCook->getHealthTime())
		{
			IVIPCooks.dequeue(pCook);
			injuredVCooks--;
			pCook->setHealthTime(-1);
			VIPCooks.Insert(pCook);
			availableVCooks++;
			availableCooks.InsertEnd(pCook);
		}
		else break;
	}
	while (!IveganCooks.isEmpty())
	{
		IveganCooks.peekFront(pCook);
		if (CurrentTimeStep == pCook->getHealthTime())
		{
			IveganCooks.dequeue(pCook);
			injuredGCooks--;
			pCook->setHealthTime(-1);
			veganCooks.Insert(pCook);
			availableVCooks++;
			availableCooks.InsertEnd(pCook);
		}
		else break;
	}
}

//Moves waiting orders to inservice orders
void Restaurant::AssignOrders(int TimeStep)
{
	Order* pOrd; 
	Cook* pCook;

	//Returns healed cooks from injured lists to available lists
	ReturnHealedCooks(TimeStep); 

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

	string line6 = "Assigned Orders in the last time step: ";
	//Assign VIP orders to available VIP cooks
	while (!VIPOrders.isEmpty() && !VIPCooks.isEmpty())
	{
		pOrd = VIPOrders.ExtractMax();
		pCook = VIPCooks.ExtractMax();
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "V" + to_string(pCook->GetID()) + "(V" + to_string(pOrd->GetID()) + ") ";
		availableVCooks--;
	}
	//If there are still VIP orders, assign normal cooks to them
	while (!VIPOrders. isEmpty() && !normalCooks.isEmpty()) {
		pOrd = VIPOrders.ExtractMax();
		pCook = normalCooks.ExtractMax();
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "N" + to_string(pCook->GetID()) + "(V" + to_string(pOrd->GetID()) + ") ";
		availableNCooks--;
	}
	//If there are still VIP orders, assign vegan cooks to them
	while (!VIPOrders.isEmpty() && !veganCooks.isEmpty()) {
		pOrd = VIPOrders.ExtractMax();
		pCook = veganCooks.ExtractMax();
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "G" + to_string(pCook->GetID()) + "(V" + to_string(pOrd->GetID()) + ") ";
		availableGCooks--;
	}


	//If there are still VIP orders , assign injured VIP cooks to theme
	while (!VIPOrders.isEmpty() && !IVIPCooks.isEmpty())
	{
		pOrd = VIPOrders.ExtractMax();
		IVIPCooks.dequeue(pCook);
		pCook->setSpeed(floor(pCook->getSpeed() / 2)); 
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "V" + to_string(pCook->GetID()) + "(V" + to_string(pOrd->GetID()) + ") ";
		injuredVCooks--; 
	}
	//If there are still VIP orders, assign injured normal cooks to them
	while (!VIPOrders.isEmpty() && !InormalCooks.isEmpty()) {
		pOrd = VIPOrders.ExtractMax();
		InormalCooks.dequeue(pCook);
		pCook->setSpeed(floor(pCook->getSpeed() / 2));
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "N" + to_string(pCook->GetID()) + "(V" + to_string(pOrd->GetID()) + ") ";
		injuredNCooks--;
	}
	//If there are still VIP orders, assign injured vegan cooks to them
	while (!VIPOrders.isEmpty() && !IveganCooks.isEmpty()) {
		pOrd = VIPOrders.ExtractMax();
		IveganCooks.dequeue(pCook);
		pCook->setSpeed(floor(pCook->getSpeed() / 2));
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "G" + to_string(pCook->GetID()) + "(V" + to_string(pOrd->GetID()) + ") ";
		injuredGCooks--;
	}


	//Assign vegan orders to available vegan cooks
	while (!veganOrders.isEmpty() && !veganCooks.isEmpty())
	{
		veganOrders.dequeue(pOrd);
		pCook = veganCooks.ExtractMax();
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "G" + to_string(pCook->GetID()) + "(G" + to_string(pOrd->GetID()) + ") ";
		availableGCooks--;
	}

	//If there are still vegan orders assign them to injured vegan cooks 
	while (!veganOrders.isEmpty() && !IveganCooks.isEmpty())
	{
		veganOrders.dequeue(pOrd);
		IveganCooks.dequeue(pCook);
		pCook->setSpeed(floor(pCook->getSpeed() / 2));
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "G" + to_string(pCook->GetID()) + "(G" + to_string(pOrd->GetID()) + ") ";
		injuredGCooks--;
	}


	//Assign normal orders to available normal cooks
	while (normalOrders.getHead() && !normalCooks.isEmpty())
	{
		pOrd = normalOrders.DeleteAndReturnFirst();
		pCook = normalCooks.ExtractMax();
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "N" + to_string(pCook->GetID()) + "(N" + to_string(pOrd->GetID()) + ") ";
		availableNCooks--;
	} 
	//If there are still normal orders, assign VIP cooks to them
	while (normalOrders.getHead() && !VIPCooks.isEmpty()) {
		pOrd = normalOrders.DeleteAndReturnFirst();
		pCook = VIPCooks.ExtractMax(); 
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "V" + to_string(pCook->GetID()) + "(N" + to_string(pOrd->GetID()) + ") ";
		availableVCooks--;
	}

	//If there are still normal orders, assign normal orders to injured normal cooks
	while (normalOrders.getHead() && !normalCooks.isEmpty())
	{
		pOrd = normalOrders.DeleteAndReturnFirst();
		InormalCooks.dequeue(pCook);
		pCook->setSpeed(floor(pCook->getSpeed() / 2));
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "N" + to_string(pCook->GetID()) + "(N" + to_string(pOrd->GetID()) + ") ";
		injuredNCooks--;
	}
	//If there are still normal orders, assign injured VIP cooks to them
	while (normalOrders.getHead() && !VIPCooks.isEmpty()) {
		pOrd = normalOrders.DeleteAndReturnFirst();
		IVIPCooks.dequeue(pCook);
		pCook->setSpeed(floor(pCook->getSpeed() / 2));
		AddtoInserviceOrdersCooks(pOrd, pCook, TimeStep);
		line6 += "V" + to_string(pCook->GetID()) + "(N" + to_string(pOrd->GetID()) + ") ";
		injuredVCooks--;
	}
	//Print assigned orders info
	pGUI->PrintMessage(line6, 6, 0);
}

//Moves inservice orders to finished orders
void Restaurant::ServeOrders(int TimeStep)
{
	Node<Order*>* nodeOrd = inServiceOrders.getHead();
	Order* pOrd;
	while (nodeOrd)
	{
		if (nodeOrd->getItem()->getFinishTime() <= TimeStep) {
			pOrd = inServiceOrders.ReturnAndRemove(nodeOrd->getItem()->GetID());
			AddtoFinishedOrders(pOrd, TimeStep);
		}
		nodeOrd = nodeOrd->getNext();
	}
}

//Returns cooks who are done with their orders or breaks into the availableCooks list
void Restaurant::FreeCooks(int TimeStep) {
	Node<Cook*>* nodeCook = idleCooks.getHead();
	Cook* pCook;
	while (nodeCook)
	{
		if (nodeCook->getItem()->getFinishTime() <= TimeStep) {
			pCook = idleCooks.ReturnAndRemove((double)nodeCook->getItem()->GetID());
			ReturntoAvailableCooks(pCook, TimeStep);
		}
		nodeCook = nodeCook->getNext();
	}
}

void Restaurant::Statusbar()
{
	int NO = normalOrders.getCount(); 
	int VO = VIPOrders.getCount();
	string line3 = "Orders Waiting: " + to_string(NO + VO + veganInWait)
		+ " [ Norm: " + to_string(NO) + ", Veg: " + to_string(veganInWait) +
		", VIP: " + to_string(VO) + " ]";
	string line4 = "Available Cooks: " + to_string(availableNCooks + availableGCooks + availableVCooks)
		+ " [ Norm: " + to_string(availableNCooks) + ", Veg: " + to_string(availableGCooks) +
		", VIP: " + to_string(availableVCooks) + " ]";
	string line5 = "Served Orders: " + to_string(doneNOrders + doneGOrders + doneVOrders)
		+ " [ Norm: " + to_string(doneNOrders) + ", Veg: " + to_string(doneGOrders) +
		", VIP: " + to_string(doneVOrders) + " ]";
	string line7 = "Injured Cooks: " + to_string(injuredNCooks + injuredGCooks + injuredVCooks)
		+ " [ Norm: " + to_string(injuredNCooks) + ", Veg: " + to_string(injuredGCooks) +
		", VIP: " + to_string(injuredVCooks) + " ]";
	pGUI->PrintMessage(line3, 3, 0); 
	pGUI->PrintMessage(line4, 4, 0);
	pGUI->PrintMessage(line5, 5, 0);
	pGUI->PrintMessage(line7, 7, 0);
}

//////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////// RESTAURANT ENDS HERE //////////////////////////////////////////////
