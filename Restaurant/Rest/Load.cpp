#include "Load.h"
#include "Restaurant.h"

Load::Load(Restaurant* Rest)
{
	pRest = Rest;

};
Load:: ~Load() {};
void Load::Excuete()
{
	//Get a pointer to the user interface
	GUI* pGUI = pRest->GetGUI();
	
	pGUI->PrintMessage("To begin type in your Events' file name with directory, using 2 backslashes. Click enter when done.", 2, 0);

	filename = pGUI->GetString();

	ifstream Datafile;
	Datafile.open(filename.c_str());

	//Loop if file failed to open
	while (!Datafile) {
		pGUI->PrintMessage("Events's file failed to load. Please re-enter file name with directory");
		filename = pGUI->GetString();
		Datafile.open(filename.c_str());
	}

	//Extract the first few lines (the constants):
	Datafile >> N >> G >> V >> SN >> SG >> SV >> BO >> BN >> BG >> BV >> AutoP >> numOfEvents;
	pRest->setAuto_p(AutoP); //setting the auto promotion parameter of the restaurant
	pRest->setCooksCount(N, G, V); //setting the cooks' count of the restaurant 

	//Constructing the events queue:
	for (int i = 0; i < numOfEvents; i++) {

		Datafile >> event;
		eventtype = toupper(event);
		switch (eventtype) {
		case 'R':
		{
			ORD_TYPE order_type;
			Datafile >> order >> TS >> ID >> SIZE >> MONEY;
			ordertype = toupper(order);
			switch (ordertype) {
			case 'N': order_type = TYPE_NRM;
				break;
			case 'G': order_type = TYPE_VGAN;
				break;
			case 'V': order_type = TYPE_VIP;
				break;
			}
			Event* R_event_ptr = new ArrivalEvent(TS, ID, order_type, MONEY, SIZE);
			pRest->AddEvent(R_event_ptr);
			break;
		}
		case 'X':
		{
			Datafile >> TS >> ID;
			Event* X_event_ptr = new CancelEvent(TS, ID);
			pRest->AddEvent(X_event_ptr);
			break;
		}
		case 'P':
		{
			Datafile >> TS >> ID >> EXMONEY;
			Event* P_event_ptr = new PromoteEvent(TS, ID, EXMONEY);
			pRest->AddEvent(P_event_ptr);
			break;
		}

		}
	}
	//constructing the normal cooks queue
	for (int i = 0; i < N; i++) {
		Cook* cook_ptr = new Cook(i+1, TYPE_NRM, SN, BO, BN);
		pRest->AddNormalCook(cook_ptr);
	}
	//constructing the vegan cooks queue
	for (int i = 0; i < G; i++) {
		Cook* cook_ptr = new Cook(N+1+i, TYPE_VGAN, SG, BO, BG);
		pRest->AddVeganCook(cook_ptr);
	}
	//constructing the VIP cooks queue
	for (int i = 0; i < V; i++) {
		Cook* cook_ptr = new Cook(N+G+1+i, TYPE_VIP, SV, BO, BV);
		pRest->AddVIPCook(cook_ptr);
	}

	Datafile.close();
	pGUI->PrintMessage("Your Events are successfully loaded. Let the game begin!"); 
};