#include "GUI.h"

//////////////////////////////////////////////////////////////////////////////////////////
GUI::GUI()
{
	DrawingItemsCount = 0;
	pWind = new window(WindWidth+15,WindHeight,0,0); 
	pWind->ChangeTitle("The Restautant");


	//Set color for each order type
	DrawingColors[TYPE_NRM] =  RED;	//normal-order color
	DrawingColors[TYPE_VGAN] = DARKBLUE;		//vegan-order color
	DrawingColors[TYPE_VIP] = 	VIOLET;		//VIP-order color			
	pWind->SetBrush(KHAKI);
	pWind->DrawRectangle(0, 0.5*(WindHeight - StatusBarHeight), 0.5*WindWidth, WindHeight - StatusBarHeight);
	ClearStatusBar();
	ClearDrawingArea(); 
	DrawRestArea();  
	
}
//////////////////////////////////////////////////////////////////////////////////////////
GUI::~GUI()
{
	delete pWind;
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== INPUT FUNCTIONS ====================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::waitForClick() const
{
	int x,y;
	pWind->WaitMouseClick(x, y);	//Wait for mouse click
}
//////////////////////////////////////////////////////////////////////////////////////////
string GUI::GetString() const 
{
	string Label;
	char Key;
	while(1)
	{
		pWind->WaitKeyPress(Key);
		if(Key == 27 )	//ESCAPE key is pressed
			return "";	//returns nothing as user has cancelled label
		if(Key == 13 )	//ENTER key is pressed
			return Label;
		if((Key == 8) && (Label.size() >= 1))	//BackSpace is pressed
			Label.resize(Label.size() -1 );			
		else
			Label += Key;
		
		PrintMessage(Label);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== OUTPUT FUNCTIONS ===================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::PrintMessage(string msg, int line, bool erase ) const	//Prints a message on status bar
{
	if (line > 7)
		return; 

	if (erase)
		ClearStatusBar();	//First clear the status bar

	pWind->SetPen(DARKRED);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight) + line*17, msg);
	
	

}
//////////////////////////////////////////////////////////////////////////////////////////
void GUI::DrawString(const int iX, const int iY, const string Text)
{
	pWind->SetPen(DARKRED);
	pWind->SetFont(18, BOLD , BY_NAME, "Arial");   
	pWind->DrawString(iX, iY, Text);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GUI::ClearStatusBar() const
{
	pWind->SetPen(WHITE, 3);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, WindHeight - StatusBarHeight , WindWidth, WindHeight);	

	pWind->SetPen(BROWN, 3);
	pWind->DrawLine(0, WindHeight - StatusBarHeight , WindWidth, WindHeight - StatusBarHeight);	
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::ClearDrawingArea() const
{
	// Clearing the Drawing area
	pWind->SetPen(KHAKI, 3);
	pWind->SetBrush(KHAKI);
	pWind->DrawRectangle(0, 0, WindWidth, 0.5* (WindHeight - StatusBarHeight));
	pWind->DrawRectangle(0.5 * WindWidth, 0.5 * (WindHeight - StatusBarHeight), WindWidth, WindHeight - StatusBarHeight);
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::DrawRestArea() const
{
	int L = RestWidth / 2;

	// 1- Drawing the brown square of the Rest
	pWind->SetPen(BROWN);
	pWind->SetBrush(BROWN);
	pWind->DrawRectangle(RestStartX, RestStartY, RestEndX, RestEndY);

	// 2- Drawing the 2 brown crossed lines (for making 4 regions)
	pWind->SetPen(BROWN, 3);
	pWind->DrawLine(0, YHalfDrawingArea, WindWidth, YHalfDrawingArea);
	pWind->DrawLine(WindWidth/2, MenuBarHeight, WindWidth/2, WindHeight-StatusBarHeight);

	// 3- Drawing the 2 white crossed lines (inside the Rest)
	pWind->SetPen(WHITE);
	pWind->DrawLine(WindWidth/2, YHalfDrawingArea - RestWidth/2, WindWidth/2, YHalfDrawingArea + RestWidth/2);
	pWind->DrawLine(WindWidth/2 - RestWidth/2, YHalfDrawingArea, WindWidth/2 + RestWidth/2, YHalfDrawingArea);

	// 4- Drawing the 4 white squares inside the Rest (one for each region)
	/*pWind->SetPen(WHITE);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(RestStartX , RestStartY , RestStartX + 2*L/2, RestStartY + 2*L/2);
	pWind->DrawRectangle(RestStartX + L/3, RestEndY - L/3, RestStartX + 2*L/3, RestEndY - 2*L/3);
	pWind->DrawRectangle(RestEndX - 2*L/3, RestStartY + L/3, RestEndX - L/3, RestStartY + 2*L/3);
	pWind->DrawRectangle(RestEndX - 2*L/3, RestEndY - L/3, RestEndX - L/3, RestEndY - 2*L/3);*/

	// 5- Writing regions labels
	pWind->SetPen(WHITE);
	pWind->SetFont(20, BOLD , BY_NAME, "Arial");
	pWind->DrawString(RestStartX + (int)(0.1*L), RestStartY + 5*L/12, "WAIT");

	pWind->DrawString(WindWidth/2 + (int)(0.1*L), RestStartY + 5*L/12, "COOK");

	pWind->DrawString(WindWidth/2 + (int)(0.1*L), YHalfDrawingArea + 5*L/12, "SRVG"); 

	pWind->DrawString(RestStartX + (int)(0.1*L), YHalfDrawingArea + 5*L/12, "DONE");

}
//////////////////////////////////////////////////////////////////////////////////////////
//Draws the passed item in its region
//region count in the numbers of items drawn so far in that item's region
void GUI::DrawSingleItem(const DrawingItem* pDitem, int RegionCount) const       // It is a private function
{

	if (RegionCount > MaxRegionOrderCount) 
		return; //no more items can be drawn in this region

	int DrawDistance = RegionCount;
	int YPos = 1;
	if(RegionCount>=MaxHorizOrders )	//max no. of orders to draw in one line
	{
		DrawDistance = (RegionCount-1)%MaxHorizOrders + 1;
		YPos = (RegionCount-1) / MaxHorizOrders + 1; 
	}

	GUI_REGION Region = pDitem->region;

	int x, y, refX, refY;
	//First calculate x,y position of the order on the output screen
	//It depends on the region and the order distance
	switch (Region)
	{
	case ORD_REG:
		refX = (WindWidth/2 - RestWidth/2);
		refY = YHalfDrawingArea - OrderHeight; //
		x = refX - DrawDistance*OrderWidth - DrawDistance; //(Distance)
		y = refY - YPos*OrderHeight - YPos; // YPos
		break;
	case COOK_REG:
		refX = (WindWidth/2 + RestWidth/2);
		refY = YHalfDrawingArea - OrderHeight; //
		x = refX + (DrawDistance-1)*OrderWidth + DrawDistance; //(Distance)
		y = refY - YPos*OrderHeight - YPos; // YPos
		break;
	case SRV_REG:
		refX = (WindWidth/2 + RestWidth/2);
		refY = YHalfDrawingArea + OrderHeight; //
		x = refX + (DrawDistance-1)*OrderWidth + DrawDistance; //(Distance)
		y = refY + (YPos-1)*OrderHeight + YPos; // YPos
		break;
	default:
		break;
	}

	// Drawing the item
	pWind->SetPen(pDitem->clr);
	pWind->SetFont(20,BOLD, MODERN);
	pWind->DrawInteger(x,y,pDitem->ID);
}


//////////////////////////////////////////////////////////////////////////////////////////
/* A function to draw all items in DrawingList and ensure there is no overflow in the drawing*/
void GUI::DrawAllItems() 
{

	//Prepare counter for each region
	int RegionsCounts[REG_CNT]={0};	//initlaize all counters to zero

	DrawingItem* pDitem;
	for(int i=0; i<DrawingItemsCount; i++)
	{
		pDitem = DrawingList[i];
		RegionsCounts[pDitem->region]++;
		DrawSingleItem(DrawingList[i],RegionsCounts[pDitem->region]);

	}

}

void GUI::UpdateInterface() 
{
	ClearDrawingArea();
	DrawRestArea();
	DrawAllItems();
}

/*
	AddOrderForDrawing: Adds a new item related to the passed Order to the drawing list
*/
//Adds waiting and inservice orders 
void GUI::AddToDrawingList(Order* pOrd, ORD_STATUS order_status)
{

	DrawingItem* pDitem = new DrawingItem;
	pDitem->ID = pOrd->GetID();
	pDitem->clr = DrawingColors[pOrd->GetType()];
	GUI_REGION reg;

	switch (order_status)
	{
	case WAIT:
		reg = ORD_REG;	//region of waiting orders
		break;
	case SRV:
		reg = SRV_REG;	//region of waiting orders
		break;
	}

	pDitem->region = reg;

	DrawingList[DrawingItemsCount++] = pDitem;
}

//Adds waiting and finished orders 
void GUI::DrawFinishedOrder(Order* pOrd)
{
	static int orderNum = 1;

	if (orderNum > MaxRegionOrderCount)
		return; //no more items can be drawn in this region

	int DrawDistance = orderNum;
	int YPos = 1;
	if (orderNum >= MaxHorizOrders)	//max no. of orders to draw in one line
	{
		DrawDistance = (orderNum - 1) % MaxHorizOrders + 1;
		YPos = (orderNum - 1) / MaxHorizOrders + 1;
	}

	//First calculate x,y position of the order on the output screen
	int refX = (WindWidth / 2 - RestWidth / 2);
	int refY = YHalfDrawingArea + OrderHeight; //
	int x = refX - DrawDistance * OrderWidth - DrawDistance; //(Distance)
	int y = refY + (YPos - 1) * OrderHeight + YPos; // YPos

	// Drawing the item
	pWind->SetPen(DrawingColors[pOrd->GetType()]);
	pWind->SetFont(20, BOLD, MODERN);
	pWind->DrawInteger(x, y, pOrd->GetID());
	orderNum++;
}

//Adds available cooks 
void GUI::AddToDrawingList(Cook* pC)
{

	DrawingItem* pDitem = new DrawingItem;
	pDitem->ID = pC->GetID();
	pDitem->clr = DrawingColors[pC->GetType()];
	pDitem->region = COOK_REG;

	DrawingList[DrawingItemsCount++] = pDitem;

}

void GUI::ResetDrawingList()
{
	for(int i=0; i<DrawingItemsCount; i++)
		delete DrawingList[i];

	DrawingItemsCount = 0;
}

 


PROG_MODE	GUI::getGUIMode() const
{
	PROG_MODE Mode;
	do
	{
		PrintMessage("Select GUI mode: (1)Interactive, (2)StepByStep, (3)Silent ");
		string S = GetString();
		Mode = (PROG_MODE) (atoi(S.c_str())-1);
	}
	while(Mode< 0 || Mode >= MODE_CNT);
	
	return Mode;
}
