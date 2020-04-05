#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Node.h"

template <typename T>
class LinkedList
{
private:
	Node<T> *Head;	//Pointer to the head of the list
	Node<T> *Tail;  //Pointer to the tail of the list 
	int count;	//Number of nodes in the list
public:


	LinkedList()
	{
		count = 0;
		Head = nullptr;
		Tail = nullptr; 
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll(); 
	}
	////////////////////////////////////////////////////////////////////////

	Node<T>* getHead() const {
		return Head;
	}

	Node<T>* getTail() const {
		return Tail;
	}

	void setHead(Node<T>* h) {
		Head = h; 
	}

	void setTail(Node<T>* t) {
		Tail = t;
	}

	int getCount() const {
		return count;
	} 

	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/

	
	void PrintList()	const
	{
		cout<<"\nList has "<<count<<" nodes";
		cout<<"\nprinting list contents:\n\n";
		Node<T> *p = Head;

		while(p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "*\n";
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	* 
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T &data)
	{
		Node<T> *R = new Node<T>(data);
		if (!Head)
			Tail = R; 
		R->setNext(Head);
		Head = R;
		count++;

	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T> *P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
		count = 0;
	}



	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]InsertEnd 
	//inserts a new node at end of the list
	void InsertEnd(const T &data) {
		Node<T> *R = new Node<T>(data);
		R->setNext(nullptr);
		if (Tail) {
			Tail->setNext(R);
			Tail = R; 
			count++;
		}
		else
			InsertBeg(data);
		
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(T Key) {
		Node<T> *P = Head;
		while (P) {
			if (P->getItem() == Key) return true;
			P = P->getNext();
		}
		return false;
	}

	//Overloading this function to find Order IDs
	bool Find(int Key) {
		Node<Order*>* P = Head;
		while (P) {
			if (P->getItem()->GetID() == Key) return true;
			P = P->getNext();
		}
		return false;
	}

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T &value) {
		Node<T> *P = Head;
		int c = 0;
		while (P) {
			if (P->getItem() == value) c++;
			P = P->getNext();
		}
		return c;
	}

	//[4] DeleteFirst
	//Deletes the first node in the list
	void DeleteFirst() {
		Node<T> *P = Head;
		if (P) {
			if (P == Tail)
				Tail = nullptr; 
			Head = P->getNext();
			delete P;
			count--;
		}
	}


	//[5] DeleteLast
	//Deletes the last node in the list
	void DeleteLast() {
		Node<T> *P = Head;
		if (P) {
			while (P->getNext()->getNext()) {
				P = P->getNext();
			}
			Node<T> *R = P->getNext();
			delete R;
			Tail = P;
			P->setNext(nullptr);
			count--;
		}
	}

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(const T &value) {
		Node<T> *P = Head;
		Node<T> *R;
		while (P) {
			if (P->getItem() == value) {
				if (P == Head) {
					DeleteFirst(); 
					return true;
				}
				else {
					if (P == Tail)
						Tail = R; 
					R->setNext(P->getNext());
					delete P;
					count--;
					return true;
				}
			}
			R = P;
			P = P->getNext();
		}
		return false;
	}

	//Overriding this function to delete an order using its ID
	bool DeleteNode(int value) {
		Node<Order*>* P = Head;
		Node<Order*>* R;
		while (P) {
			if (P->getItem()->GetID() == value) {
				if (P == Head) {
					DeleteFirst(); return true;
				}
				else {
					if (P == Tail)
						Tail = R;
					R->setNext(P->getNext());
					delete P;
					count--;
					return true;
				}
			}
			R = P;
			P = P->getNext();
		}
		return false;
	}

	//Overriding this function to delete an IDholder using its ID
	bool DeleteNode(int value, bool idholder) {
		Node<IDholder*>* P = Head;
		Node<IDholder*>* R;
		while (P) {
			if (P->getItem()->getID() == value) {
				if (P == Head) {
					DeleteFirst(); return true;
				}
				else {
					if (P == Tail)
						Tail = R;
					R->setNext(P->getNext());
					delete P;
					count--;
					return true;
				}
			}
			R = P;
			P = P->getNext();
		}
		return false;
	}

	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNodes(const T &value) {
		bool found = false;
		Node<T> *P = Head;
		while (P && P == Head && P->getItem() == value) {
			P = P->getNext();
			DeleteFirst(); 
			found = true;
		}
		while (P && P->getNext()) {
			if (P->getNext()->getItem() == value) {
				if (P->getNext() == Tail)
					Tail = P; 
				Node<T>* R = P->getNext();
				P->setNext(R->getNext());
				delete R;
				found = true;
				count--;
			}
			else {
				P = P->getNext();
			}
		}
		return found;
	}

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	void Merge(const LinkedList& L) {
		
		Tail->setNext(L.Head()); 
		Tail = L.Tail(); 
		count = count + L.getCount();
	}
	

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse() {
		Node<T>* P = Head;
		Node<T>* R = P;
		if (P) {
			while (P->getNext()) {
				R = P->getNext();
			    P->setNext(R->getNext());
				R->setNext(Head);
				Head = R;
			}
		}
	}


	//[10] ReturnAndRemove
	//Finds the first node with the passed value, returns a pointer to it and removes it from the linked list
	T ReturnAndRemove(T value) {
		Node<T>* P = Head;
		Node<T>* R;
		while (P) {
			if (P->getItem() == Key) {
				if (P == Head) {
					Head = P->getNext();
					if (P == Tail)
						Tail = nullptr; 
				}
				else {
					if (P == Tail)
						Tail = R; 
					R->setNext(P->getNext());
				}
				count--;
				return P->getItem();
			}
			R = P;
			P = P->getNext();
		}
	}

	//Overloading this function to find, return and remove an order based on its ID
	Order* ReturnAndRemove(int Key) {
		Node<Order*>* P = Head;
		Node<Order*>* R;
		while (P) {
			if (P->getItem()->GetID() == Key) {
				if (P == Head) {
					Head = P->getNext();
					if (P == Tail)
						Tail = nullptr;
				}
				else {
					if (P == Tail)
						Tail = R; 
					R->setNext(P->getNext());
				}
				count--;
				return P->getItem();
			}
			R = P;
			P = P->getNext();
		}
	}

	//Overloading this function to find, return and remove the first order of a particular type
	Order* ReturnAndRemove(ORD_TYPE Key) {
		Node<Order*>* P = Head;
		Node<Order*>* R;
		while (P) {
			if (P->getItem()->GetType() == Key) {
				if (P == Head) {
					Head = P->getNext();
					if (P == Tail)
						Tail = nullptr;
				}
				else {
					if (P == Tail)
						Tail = R; 
					R->setNext(P->getNext());
				}
				count--;
				return P->getItem();
			}
			R = P;
			P = P->getNext();
		}
	}

	//[11]Return
	//Finds the first node with the passed value, returns a pointer to it
	Node<T>* Return(T value) {
		Node<T>* P = Head;
		while (P) {
			if (P->getItem() == Key) {
				return P;
			}
			P = P->getNext();
		}
	}

	//Overloading this function to find and return an order based on its ID
	Node<Order*>* Return(int Key) {
		Node<Order*>* P = Head;
		while (P) {
			if (P->getItem()->GetID() == Key) {
				return P;
			}
			P = P->getNext();
		}
	}


	//[12] DeleteAndReturnFirst
	//Returns and deletes the first node in the list
	T DeleteAndReturnFirst() {
		
		if (Head) {
			Node<T>* P = Head;
			Head = P->getNext();
			if (P == Tail)
				Tail = nullptr;
			count--;
			return P->getItem(); 
			
		}
	}

	//[13] DeleteAndReturnLargest
	//Returns and deletes the node with teh largest item 
	T DeleteAndReturnLargest() {
		Node<T>* Largest = Head;
		if (Head)
		{
			Node<T>* P = Head->getNext();
			Node<T>* R = Head;
			Node<T>* Previous;
			while (P)
			{
				if (P->getItem() > Largest->getItem())
				{
					Largest = P;
					Previous = R;
				}
				R = P;
				P = P->getNext();
			}
			if (Largest == Head)
			{
				Head = Largest->getNext(); 
				count--;
				if (Largest == Tail)
					Tail = nullptr; 
			}
			else
			{
				if (Largest == Tail)
					Tail = Previous;
				Previous->setNext(Largest->getNext());
				count--;
			}
			

			return Largest->getItem();
		}
	}

	//Overloading this function to find and return order with largest priority
	Order* DeleteAndReturnLargest(bool order) {
		Node<Order*>* Largest = Head;
		if (Head)
		{
			Node<Order*>* P = Head->getNext();
			Node<Order*>* R = Head; 
			Node<Order*>* Previous;
			while (P)
			{
				if (P->getItem()->getPriority() > Largest->getItem()->getPriority())
				{
					Largest = P;
					Previous = R;
				}
				R = P;
				P = P->getNext();
			}
			if (Largest == Head)
			{
				Head = Largest->getNext(); 
				count--; 
				if (Largest == Tail)
					Tail = nullptr;
			}
			else
			{
				if (Largest == Tail)
					Tail = Previous; 
				Previous->setNext(Largest->getNext());
				count--;
			}

			return Largest->getItem();
		}
		
	}

	
};





#endif	

