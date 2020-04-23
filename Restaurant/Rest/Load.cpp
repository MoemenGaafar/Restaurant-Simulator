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

	//a key to find out if the speeds and break times of cooks of the same type are constant or different (default case is constant)
	string speeds_breaks = "c"; 
	pGUI->PrintMessage("Please specify if the speeds and break times of the cooks are constant or different \n [c] means constant \n [d] means different");
	speeds_breaks = pGUI->GetString();
	speeds_breaks[0] = tolower(speeds_breaks[0]); //to eliminate the capital letters case
	while (speeds_breaks != "c" && speeds_breaks != "d") //if the user accidentally entered a wrong character
	{
		pGUI->PrintMessage("Error input! \n Please specify if the speeds and break times of the cooks are constant or different \n [c] means constant \n [d] means different");
		speeds_breaks = pGUI->GetString();
		speeds_breaks[0] = tolower(speeds_breaks[0]);
	}
	if (speeds_breaks == "c") { //normal case (the speeds of every type is constant)
		//Extract the first few lines (the constants):
		Datafile >> N >> G >> V >> SN >> SG >> SV >> BO >> BN >> BG >> BV >> AutoP >> numOfEvents;
		pRest->setAuto_p(AutoP); //setting the auto promotion parameter of the restaurant
		pRest->setCooksCount(N, G, V); //setting the cooks' count of the restaurant 


		//constructing the normal cooks queue
		for (int i = 0; i < N; i++) {
			Cook* cook_ptr = new Cook(i + 1, TYPE_NRM, SN, BO, BN);
			pRest->AddNormalCook(cook_ptr);
		}
		//constructing the vegan cooks queue
		for (int i = 0; i < G; i++) {
			Cook* cook_ptr = new Cook(N + 1 + i, TYPE_VGAN, SG, BO, BG);
			pRest->AddVeganCook(cook_ptr);
		}
		//constructing the VIP cooks queue
		for (int i = 0; i < V; i++) {
			Cook* cook_ptr = new Cook(N + G + 1 + i, TYPE_VIP, SV, BO, BV);
			pRest->AddVIPCook(cook_ptr);
		}
	}
	if (speeds_breaks == "d") {
		Datafile >> N >> G >> V;
		int* Nspeeds = new int[N]; //array to hold the speeds of normal cooks
		int* Gspeeds = new int[G]; //array to hold the speeds of vegan cooks
		int* Vspeeds = new int[V]; //array to hold the speeds of vip cooks
		//filling out the arrays:
		for (int i = 0; i < N; i++) {
			Datafile >> Nspeeds[i]; 
		}
		for (int i = 0; i < G; i++) {
			Datafile >> Gspeeds[i];
		}
		for (int i = 0; i < V; i++) {
			Datafile >> Vspeeds[i];
		}
		Datafile >> BO;

		int* Nbreaks = new int[N]; //array to hold the break times of normal cooks
		int* Gbreaks = new int[G]; //array to hold the break times of vegan cooks
		int* Vbreaks = new int[V]; //array to hold the break times of vip cooks
		//filling out the arrays:
		for (int i = 0; i < N; i++) {
			Datafile >> Nbreaks[i];
		}
		for (int i = 0; i < G; i++) {
			Datafile >> Gbreaks[i];
		}
		for (int i = 0; i < V; i++) {
			Datafile >> Vbreaks[i];
		}
		Datafile >> AutoP >> numOfEvents;
		pRest->setAuto_p(AutoP); //setting the auto promotion parameter of the restaurant
		pRest->setCooksCount(N, G, V); //setting the cooks' count of the restaurant 

		//constructing the normal cooks queue
		for (int i = 0; i < N; i++) {
			Cook* cook_ptr = new Cook(i + 1, TYPE_NRM, Nspeeds[i], BO, Nbreaks[i]);
			pRest->AddNormalCook(cook_ptr);
		}
		//constructing the vegan cooks queue
		for (int i = 0; i < G; i++) {
			Cook* cook_ptr = new Cook(N + 1 + i, TYPE_VGAN, Gspeeds[i], BO, Gbreaks[i]);
			pRest->AddVeganCook(cook_ptr);
		}
		//constructing the VIP cooks queue
		for (int i = 0; i < V; i++) {
			Cook* cook_ptr = new Cook(N + G + 1 + i, TYPE_VIP, Vspeeds[i], BO, Vbreaks[i]);
			pRest->AddVIPCook(cook_ptr);
		}
		//deleting the dynamically allocated arrays to free-up the memory:
		delete Nspeeds, Gspeeds, Vspeeds, Nbreaks, Gbreaks, Vbreaks;

	}


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
	Datafile.close();
	pGUI->PrintMessage("Your Events are successfully loaded. Let the game begin!"); 
};