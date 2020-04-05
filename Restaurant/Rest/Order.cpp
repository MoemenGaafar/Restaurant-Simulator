#include "Order.h"

Order::Order(int arrivalTime, int id, ORD_TYPE ordType, double money, int dishes)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = ordType;
	totalMoney = money;
	status = WAIT;
	nDishes = dishes;
	ArrTime = arrivalTime;

	if (type == TYPE_VIP)
		priority = (money/dishes) - arrivalTime;  //VIP orders with higher priority are served first 
	else
		priority = 0;
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


ORD_TYPE Order::GetType() const
{
	return type;
}


void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::setPriority(double pr) {
	priority = pr;
}

double Order::getPriority() const {
	return priority;
}

int Order::getArrTime() const {
	return ArrTime;
}

void Order::setArrTime(int t) {
	ArrTime = t; 
}

int Order::getServTime() const {
	return ServTime; 
}

void Order::setServTime(int t) {
	ServTime = t; 
}

int Order::getFinishTime() const {
	return FinishTime; 
}

void Order::setFinishTime(int t) {
	FinishTime = t; 
}

double Order::getTotalMoney() const {
	return totalMoney;
}

int Order::getNDishes() const {
	return nDishes;
}