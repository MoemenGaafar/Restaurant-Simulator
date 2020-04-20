#ifndef _IDHOLDER 
#define _IDHOLDER 
#include "../Defs.h"

class IDholder
{
	//This class is made to hole the ID and type of orders and cooks. 
	//This data will be used to facilitate the process of drawing waiting orders and available cooks. 

	int ID; 
	ORD_TYPE holderType; 

public: 

	IDholder(int i, ORD_TYPE t); 
	~IDholder(); 

	int getID(); 
	ORD_TYPE getHolderType(); 




};


#endif 
