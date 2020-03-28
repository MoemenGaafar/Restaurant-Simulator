#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Node.h"

template <typename T>
class LinkedList
{
private:
	Node<T> *Head;	//Pointer to the head of the list
	//You can add tail pointer too (depending on your problem)
	int count;	//Number of nodes in the list
public:


	LinkedList()
	{
		count = 0;
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll(); 
	}
	////////////////////////////////////////////////////////////////////////
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
	//inserts a new node at end if the list
	void InsertEnd(const T &data) {
		Node<T> *R = new Node<T>(data);
		R->setNext(nullptr);
		Node<T> *P = Head;
		if (P) {
			while (P->getNext()) {
				P = P->getNext();
			}
			P->setNext(R);
		}
		else
			InsertBeg(data);
		count++;
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
					DeleteFirst(); return true;
				}
				else {
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
	Node<T>* getHead() const {
		return Head;
	}

	int getCount() const {
		return count;
	}

	void Merge(const LinkedList& L) {
		Node<T>* P = Head;
		while (P->getNext()) {
			P = P->getNext();
		}
		Node<T>* LNode = L.getHead();
		T NodeVal;
			while (LNode) {
				NodeVal = LNode->getItem();
				Node<T>* Q = new Node<T>(NodeVal);
				P->setNext(Q);
				P = P->getNext();
				LNode = LNode->getNext();
			}
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
	T* ReturnAndRemove(T value) {
		Node<T>* P = Head;
		Node<T>* R;
		while (P) {
			if (P->getItem() == Key) {
				if (P == Head) {
					Head = P->getNext();
				}
				else {
					R->setNext(P->getNext());
				}
				count--;
				return &P->getItem();
			}
			R = P;
			P = P->getNext()->getItem();
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
				}
				else {
					R->setNext(P->getNext());
				}
				count--;
				return P->getItem();
			}
			R = P;
			P = P->getNext();
		}
	}
};




#endif	

