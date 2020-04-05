#ifndef LOAD
#define LOAD
#include <string> 
#include <iostream>
#include <fstream>

using namespace std;

class Restaurant; //Forward class declaration 

class Load
{
	//Load class loads the events, cooks, and information 
	//from a text file entered by the user to an owner Restaurant object. 
public: 

	Load(Restaurant* Rest);
	~Load();
	void Excuete(); //Takes text file from user and loads data to restaurant  

private:

	string filename; //Text file name to be taken from user
	int N, G, V,
		SN, SG, SV,
		BO, BN, BG, BV,
		AutoP, numOfEvents, eventtype, ordertype,
		TS, ID, SIZE, MONEY, EXMONEY; //Variables to be extracted from file 
	char event, order;
	Restaurant* pRest; //Pointer to restaurant where file loading occurs 

	

};


#endif 
