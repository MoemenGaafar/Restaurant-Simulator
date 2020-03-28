#include "VIPOrder.h"

VIPOrder::VIPOrder(int id) : Order(id) {
	type = TYPE_VIP;
	priority = 0;  //This should be replaced with the convenient priority formula
}

void VIPOrder::setPriority(double pr) {
	priority = pr;
}

double VIPOrder::getPriority() const {
	return priority;
}