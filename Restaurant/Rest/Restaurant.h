#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\LinkedList.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancelEvent.h"
#include "..\Events\PromoteEvent.h"
#include<fstream>
#include<string>
#include "Order.h"

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	PROG_MODE mode;
	int Auto_p;
	///Phase 1 members///
	LinkedList<Order*> normalOrders; //Contains all unassigned Normal orders
	Queue<Order*> veganOrders; //Contains all unassigned Vegan orders
	LinkedList<Order*> VIPOrders; //Contains all unassigned VIP orders
	LinkedList<Order*> inServiceOrders; //Contains all in-service orders
	Queue<Cook*> normalCooks; //Contains all available normal cooks
	Queue<Cook*> veganCooks; //Contains all availabe vegan cooks
	Queue<Cook*> VIPCooks; //Contains all availabe normal cooks
	LinkedList<Cook*> busyCooks; //Contains cooks working on orders
	Queue<Cook*> inBreakCooks; //Contains cooks taking breaks
	//////////////////////

	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	
	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	
	Restaurant();
	~Restaurant();
	void setAuto_p(int p);
	int getAuto_p();
	void FileLoading();
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void FillDrawingList();
	PROG_MODE getMode();

/// ===================    Phase 1 Simulation functions.  ================= 
	void Phase1Simulator(); //Main Simulator Function

	//Three functions that add to the corresponding list of orders
	void AddtoNormalOrders(Order* po);
	void AddtoVeganOrders(Order* po);
	void AddtoVIPOrders(Order* po);

	ORD_TYPE typeFinder(int key); //Returns the type of the order ID
	void CancelNormalOrder(int id); //Cancels a normal order using its ID
	void PromoteNormalOrder(int id, double extraMoney); //Promotes a normal order using its ID
/// ================================================================================================== 


/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 

	void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue

/// ================================================================================================== 



};

#endif