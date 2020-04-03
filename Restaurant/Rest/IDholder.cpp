#include"IDholder.h"

IDholder::IDholder(int i, ORD_TYPE t) {
	ID = i; 
	holderType = t; 
}

IDholder:: ~IDholder() {}; 

int IDholder::getID() {
	return ID; 
}

ORD_TYPE IDholder::getHolderType() {
	return holderType; 
}