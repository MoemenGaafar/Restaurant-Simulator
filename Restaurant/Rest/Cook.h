#pragma once

#include "..\Defs.h"

#pragma once
class Cook
{
	int ID;   
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int nDishesUntilBreak; //number of dishes a cook prepares before taking a break
	int breakDuration; //duration of break after preparing n dishes
	int FinishTime; //time till a cook finishes an order 

public:
	Cook();
	Cook(int, ORD_TYPE, int, int, int);
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	void setID(int);
	void setType(ORD_TYPE) ;
	int getFinishTime() const;
	void setFinishTime(int t);
};
