#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int nDishes; //Number of dishes in the order
	double totalMoney;	//Total order money
	int ArrTime, ServTime, FinishTime; //arrival, service start times
	double priority; //only applicaple to VIP orders
	
	
public:
	Order(int arrivalTime , int ID, ORD_TYPE ordType, double money, int dishes);
	virtual ~Order();

	int GetID();

	ORD_TYPE GetType() const;

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	
	void setPriority(double pr);
	double getPriority() const;

	int getArrTime() const;
	void setArrTime(int t); 
	
	int getServTime() const;
	void setServTime(int t);

	int getFinishTime() const;
	void setFinishTime(int t);

	double getTotalMoney() const;

	int getNDishes() const;
	//
	// TODO: Add More Member Functions As Needed
	//

};

#endif