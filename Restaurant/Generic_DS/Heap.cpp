#pragma once
#include <iostream>
#include "Heap.h"
#include <string>
using namespace std;

 template <class T>
 class Heap {


	int Heap <T>:: getLeftChildIndex(int ParentIndex) {
		return 2 * ParentIndex + 1;
	}
	int Heap <T>:: getRightChildIndex(int ParentIndex) {
		return 2 * ParentIndex + 2;

	}
	int Heap <T>:: getParentIndex(int ChildIndex) {
		return (ChildIndex - 1) / 2;
	}

	boolean Heap<T>:: hasRightChild(int index) {
		return getRightChildIndex(index) < size; 
	}
	boolean Heap<T>:: hasLeftChild(int index) {
		return getLeftChildIndex(index) < size; 
	 }
	boolean Heap<T>:: hasParent(int index) {
		return getParentIndex(index) >= 0;
	}
	void Heap<T>::swap(int firstindex, int secondindex) {
		int tool = elements[firstindex];
		elements[firstindex] = elements[secondindex];
		elements[secondindex] = tool; 
	}

	void Heap<T>::ensureExtraCapacity() {
		if (size == capacity) {
			elements = Arrays.copyOf(elements, capacity * 2);
			capacity *= 2;
		}

	}
	T Heap<T>::peek() {
		if (size == 0) throw new IllegalStateException();
		return elements[0];
	}

	T Heap<T>::ExtractMax() {
		if (size == 0) throw new IllegalStateException();
		int item = elements[0];
		elements[0] = elements[size - 1];
		size--;
		heapifyDown();
		return item;

	}
	void Heap<T>::Insert(T item) {
		ensureExtraCapacity();
		elements[size] = item;
		size++;
		heapifyup();
	}
	void Heap<T>::heapifyup() {
		int index = size - 1;
		while (hasParent(index) && elements[getParentIndex(index)] < elements[index]) {
			swap(getParentIndex(index), index);
			index = getParentIndex(index);
		}
	}
	void Heap<T>::heapifydown() {
		int index = 0;
		while (hasLeftChild(index)) {
			int greaterChildIndex = getRightChildIndex(index);
			if (hasRightChild(index) && elements[getRightChildIndex(index)] < elements[getLeftChildIndex(index))] {
				smallerChildIndex = getLeftChildIndex(index);
			}
			if (elements[index] > elements[greaterChildIndex]) {
				break;
			else  swap(index, greaterChildIndex);
			}
			index = greaterChildIndex;
		}

	}
};