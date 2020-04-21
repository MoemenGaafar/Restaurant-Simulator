
#include <iostream>
#include <string>
using namespace std;


template <class T>
 class Heap {
	private int capacity;
	private int size;
	T [] elements = new T [capacity];

	private int getLeftChildIndex(int ParentIndex);
	private int getRightChildIndex(int ParentIndex);
	private int getParentIndex(int ChildIndex);

	private boolean hasRightChild(int index);
	private boolean hasLeftChild(int index);
	private boolean hasParent(int index);

	private void swap(int firstindex, int secondindex);

	private void ensureExtraCapacity();
		
	public T peek();
	public T ExtractMax();
	public void Insert(T item);
	public void heapifyup();
	public void heapifydown();
}; 
