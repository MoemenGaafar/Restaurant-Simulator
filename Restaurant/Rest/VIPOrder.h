#pragma once
#include "Order.h"

class VIPOrder : public Order {
private:
	double priority;
public:
	VIPOrder(int id);
	void setPriority(double pr);
	double getPriority() const;
};