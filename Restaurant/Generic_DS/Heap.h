#ifndef _HEAP_
#define _HEAP_

#include <iostream>
#include <string>
#include <math.h>
using namespace std;


template <class T>
class Heap {

	int capacity; // Max size of the heap's array
	int size = 1; // Current number of used array cells, initialized at 1 because elements[0] will remain unused 
	T* elements; // Dynamic array that holds the elements 



	int getLeftChildIndex(int ParentIndex); //Returns the index of an element's left child 
	int getRightChildIndex(int ParentIndex); //Returns the index of an element's right child 
	int getParentIndex(int ChildIndex); //Returns the index of an element's parent

	bool hasRightChild(int index); //Checks if element has a right child
	bool hasLeftChild(int index); //Checks if element has a left child
	bool hasParent(int index); //Checks if element has parent

	void swap(int firstindex, int secondindex); // Swaps the elements at two different positions 
	void heapifyup(bool isPointer); // Moves up an element for the tail of the array to its correct position 
	void heapifydown(bool isPointer); // Moves down an element for the head of the array to its correct position

public:

	Heap(int c); // Constructor that takes an int for capacaity 

	bool isEmpty(); // Checks if the heap is empty (by checking whether size = 1)
	int getCount(); // The count of elements in the heap
	T peek();  // Returns the max element of the heap without removing it 
	T ExtractMax(bool isPointer = 1); // Returns the element with the highest priority
	void Insert(T item, bool isPointer = 1); //inserts an elemnt in its correct place 

	
	// Functions to perform priority comparisions for Cook* and Order* types
	
	bool Gthan(Order* a, Order* b);
	bool Sthan(Order* a, Order* b);
	bool Gthan(Cook* a, Cook* b);
	bool Sthan(Cook* a, Cook* b);


	

};


template <class T>
bool Heap<T>::Gthan(Order* a, Order* b)
{
	return a->getPriority() > b->getPriority();
}

template <class T>
bool Heap<T>::Sthan(Order* a, Order* b)
{
	return a->getPriority() < b->getPriority();
}

template <class T>
bool Heap<T>::Gthan(Cook* a, Cook* b)
{
	return a->getSpeed() > b->getSpeed();
}

template <class T>
bool Heap<T>::Sthan(Cook* a, Cook* b)
{
	return a->getSpeed() < b->getSpeed();
}


template <class T>
Heap<T>::Heap(int c)
{
	capacity = c;
	elements = new T[capacity]; //initializes the array elements with a max. length of capacity
}

template <class T>
int Heap<T>::getLeftChildIndex(int ParentIndex)
{
	return 2 * ParentIndex;
}

template <class T>
int Heap <T>::getRightChildIndex(int ParentIndex)
{
	return 2 * ParentIndex + 1;
}

template <class T>
int Heap <T>::getParentIndex(int ChildIndex)
{
	return floor(ChildIndex / 2);
}

template <class T>
bool Heap<T>::hasRightChild(int index)
{
	return getRightChildIndex(index) < size;
}

template <class T>
bool Heap<T>::hasLeftChild(int index)
{
	return getLeftChildIndex(index) < size;
}

template <class T>
bool  Heap<T>::hasParent(int index)
{
	return getParentIndex(index) > 0;
}

template <class T>
void Heap<T>::swap(int firstindex, int secondindex)
{
	T tool = elements[firstindex];
	elements[firstindex] = elements[secondindex];
	elements[secondindex] = tool;
}

template <class T>
bool Heap<T>::isEmpty()
{
	return size == 1; 
}

template <class T>
int Heap<T>::getCount()
{
	return size - 1; // The size of the array is the count of used elements; since element 0 is unused Count = size - 1
}


template <class T>
T Heap<T>::peek()
{
	if (size != 1)
		return elements[1];
}

template <class T>
T Heap<T>::ExtractMax(bool isPointer)
{
	if (size != 1) {
		T item = elements[1];
		elements[1] = elements[size - 1];
		size--;
		heapifydown(isPointer);
		return item;
	}
	return NULL;
}

template <class T>
void Heap<T>::Insert(T item, bool isPointer) {
	if (size == capacity) {
		return;
	}
	elements[size] = item;
	size++;
	heapifyup(isPointer);
}

template <class T>
void Heap<T>::heapifyup(bool isPointer) {
	int index = size - 1;
	if (isPointer)
	{
		while (hasParent(index) && Sthan(elements[getParentIndex(index)], elements[index])) {
			swap(getParentIndex(index), index);
			index = getParentIndex(index);
		}
	} 
	else
	{
		while (hasParent(index) && (elements[getParentIndex(index)]< elements[index])) {
			swap(getParentIndex(index), index);
			index = getParentIndex(index);
		}
	}
}


template <class T>
void Heap<T>::heapifydown(bool isPointer)
{
	int index = 1;
	T temp = elements[index];
	int greaterChildIndex;

	if (isPointer)
	{

		while (hasLeftChild(index))
		{
			greaterChildIndex = getLeftChildIndex(index);
			if (hasRightChild(index) && Gthan(elements[getRightChildIndex(index)], elements[getLeftChildIndex(index)]))
				greaterChildIndex = getRightChildIndex(index);

			if (Gthan(temp, elements[greaterChildIndex]))
				break;
			else
				swap(index, greaterChildIndex);

			index = greaterChildIndex;
		}

	} 
	else
	{
		while (hasLeftChild(index))
		{
			greaterChildIndex = getLeftChildIndex(index);
			if (hasRightChild(index) && (elements[getRightChildIndex(index)] > elements[getLeftChildIndex(index)]))
				greaterChildIndex = getRightChildIndex(index);

			if (temp > elements[greaterChildIndex])
				break;
			else
				swap(index, greaterChildIndex);

			index = greaterChildIndex;
		}
	}



}

#endif
