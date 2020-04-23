#pragma once

#include "..\Defs.h"

#pragma once
class Cook
{
	int ID;   
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	const int nOrdersUntilBreak; //number of dishes a cook prepares before taking a break
	const int breakDuration; //duration of break after preparing n dishes
	int ordersLeftUntilBreak; //actual number of dishes the cook has left before taking the break
	int FinishTime; //time till a cook is available 
	int HealthTime; //timestep at which a cook is treated from an injury

public:

	bool speedIsOdd; //if the speed is odd (to be used when cook gets injured)

	Cook();
	Cook(int, ORD_TYPE, int, int, int);
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	void setID(int);
	void setType(ORD_TYPE) ;
	int getFinishTime() const;
	void setFinishTime(int t);
	int getSpeed();
	void setSpeed(int);
	void minusOrdersTillBreak();
	int getOrdersTillBreak();
	const int getBreakDuration();
	void resetOrderstillBreak();
	int getHealthTime(); 
	void setHealthTime(int); 

	
};
