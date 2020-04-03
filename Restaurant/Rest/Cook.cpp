#include "Cook.h"


Cook::Cook()
{
}

Cook::Cook(int id, ORD_TYPE inputType, int inputSpeed, int inputNDishesUntilBreak, int inputBreakDuration) {
	ID = id; 
	type = inputType;
	speed = inputSpeed;
	nDishesUntilBreak = inputNDishesUntilBreak;
	breakDuration = inputBreakDuration;
}

Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

int Cook::getFinishTime() const {
	return FinishTime;
}

void Cook::setFinishTime(int t) {
	FinishTime = t;
}



