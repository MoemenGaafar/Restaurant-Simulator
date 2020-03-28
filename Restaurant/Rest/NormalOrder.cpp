#include "NormalOrder.h"

NormalOrder::NormalOrder(int id, const int autoP) : Order(id) , AutoP(autoP)  {
	type = TYPE_NRM;
}