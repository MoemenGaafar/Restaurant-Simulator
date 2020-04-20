#include "Cook.h"


Cook::Cook()
	: nOrdersUntilBreak(0), breakDuration(0)
{
}

Cook::Cook(int id, ORD_TYPE inputType, int inputSpeed, int inputNOrdersUntilBreak, int inputBreakDuration) 
	: nOrdersUntilBreak(inputNOrdersUntilBreak), breakDuration (inputBreakDuration) {
	ID = id; 
	type = inputType;
	speed = inputSpeed;
	ordersLeftUntilBreak = inputNOrdersUntilBreak;
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

int Cook::getSpeed() {
	return speed;
}

void Cook::minusOrdersTillBreak() {
	ordersLeftUntilBreak--;
}

int Cook::getOrdersTillBreak() {
	return ordersLeftUntilBreak;
}

const int Cook::getBreakDuration() {
	return breakDuration;
}

void Cook::resetOrderstillBreak() {
	ordersLeftUntilBreak = nOrdersUntilBreak;
}



