#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\LinkedList.h"
#include "..\Generic_DS\Heap.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancelEvent.h"
#include "..\Events\PromoteEvent.h"
#include <string>
#include "Order.h"
#include "Load.h"
#include <cstdlib> 
#include <time.h>



// This is the maestro of the project
class Restaurant
{
private:
	GUI* pGUI; //Pointer to user interface 
	PROG_MODE mode; 
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	Load* pLoad = new Load(this);
	int Auto_p; // Waiting time counts before a Normal gets promoted to VIP for no extra charge 
	int Auto_promoted_count = 0; //Number of normal orders to get auto-promoted to VIP
	double Avg_wait = 0, Avg_serv = 0; //Average waiting and service times of finished orders 

		
	LinkedList<Order*> normalOrders; //Contains all unassigned Normal orders
	Queue<Order*> veganOrders; //Contains all unassigned Vegan orders
	Heap<Order*> VIPOrders; //Contains all unassigned VIP orders

	LinkedList<Order*> waitingOrders; //Contains all waiting orders

	int NOrdersCount = 0, GOrdersCount = 0, VOrdersCount = 0; //Number of added Normal, Vegan, and VIP orders 
	int doneNOrders = 0, doneGOrders = 0, doneVOrders = 0; //Number of served Normal, Vegan, and VIP orders
	int veganInWait = 0; //Number of vegan orders in waiting
	LinkedList<Order*> inServiceOrders; //Contains all in-service orders

	int finishedOrdersCount = 0;  //Number of finished orders 

	Heap<Cook*> normalCooks; //Contains all available normal cooks
	Heap<Cook*> veganCooks; //Contains all availabe vegan cooks
	Heap<Cook*> VIPCooks; //Contains all availabe VIP cooks 

	Queue<Cook*> InormalCooks; //Contains all injured normal cooks
	Queue<Cook*> IveganCooks; //Contains all injured vegan cooks
	Queue<Cook*> IVIPCooks; //Contains all injured VIP cooks 

	LinkedList<Cook*> idleCooks; //Contains all in-service and in-break Cooks

	LinkedList<Cook*> availableCooks; //Contains all available cooks

	int NCooksCount, GCooksCount, VCooksCount; //Number of added Normal, Vegan, and VIP cooks
	int availableNCooks, availableGCooks, availableVCooks; //Number of available Normal, Vegan, and VIP cooks
	int injuredNCooks = 0, injuredGCooks = 0, injuredVCooks = 0; //Number of idle injured Normal, Vegan, and VIP cooks

public:

	Restaurant();
	~Restaurant();

	void RunSimulation(); //Starts simulation, loads events file, calls a simulation mode function 

	//Basic setters and getters 
	void setAuto_p(int p);
	int getAuto_p();
	void setCooksCount(int n, int g, int v); 
	int getNCooksCount(); 
	int getGCooksCount();
	int getVCooksCount();
	GUI* GetGUI(); //Returns a pointer to restaurant's GUI
	PROG_MODE getMode(); 

	ORD_TYPE typeFinder(int key); //Returns the type of the order ID	
	

	//Functions that add to the corresponding queues of cooks 
	void AddNormalCook(Cook* pC);
	void AddVeganCook(Cook* pC);
	void AddVIPCook(Cook* pC);


	//Functions that add to the corresponding list of orders
	void AddtoNormalOrders(Order* po);
	void AddtoVeganOrders(Order* po);
	void AddtoVIPOrders(Order* po);

	//Functions that add orders/cooks to inservice and finished/ inservice and available lists 
	void AddtoFinishedOrders(Order* po, int TimeStep); 
	void AddtoInserviceOrdersCooks(Order* po, Cook* pc, int TimeStep); 
	void ReturntoAvailableCooks(Cook* pc, int CurrentTimeStep);
	void AddEvent(Event* pEvent); //Adds events to events queue 
	void CancelNormalOrder(int id); //Cancels a normal order using its ID
	void PromoteNormalOrder(int id, double extraMoney); //Promotes a normal order using its ID
 

	// Adds cooks and orders to GUI drawing list 
	void FillDrawingList();

	// Functions to run simulation modes 
	void modeInteractive(); 
	void modeStep(); 
	void modeSilent(); 

	// Functions to excuete simulation actions in current timestep
	void ExecuteEvents(int TimeStep);	//Executes all events at current timestep
	void ReturnHealedCooks(int TimeStep); //Returns healed cooks from injured lists to available lists
	void AssignOrders(int TimeStep);  //Moves waiting orders to inservice orders 
	void ServeOrders(int TimeStep); //Moves inservice orders to finished orders
	void FreeCooks(int TimeStep); //Moves idle cooks to available cooks
	void Statusbar(); //Updates information on status bar


};

#endif

