#pragma once
#include "Order.h"

class NormalOrder : public Order {
private:
	const int AutoP;  //the number of timesteps after which a Normal order is automatically promoted to VIP
public:
	NormalOrder(int id, const int AutoP);
};