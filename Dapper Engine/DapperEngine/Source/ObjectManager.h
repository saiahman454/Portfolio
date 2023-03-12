#pragma once
#include <vector>
































/*
template <typename T>
class Manager
{
public:
	typedef void (*functionPtr) (T param); // void function ptr
	typedef std::vector<T> vector;
	
	Manager() //: list_(new vector)
	{}
	// This ctor initializes the first object in the vector
	Manager(T object) : list_(new vector)
	{
		list_.push_back(object); // Pointer -> ?
	}
	void Update()
	{
		while (list_)
		{
			list_[i];
		}
	}
	/* Using a funtionPtr allows for each object type to have a unique update
	void Update(functionPtr update)
	{
		// Lambda
		for (T t : list_) // Updating every T t in the private  member var list_
		{
			update(t);
		}
	}
	void addObj(T object)
	{
		list_.push_back(object);
	}
	T getObj(int index)
	{
		return list_[index];
	}
	T& getRef(int index)
	{
		return &(list_[index]);
	}
		std::vector<T>* getVector()
	{
		return &list_;
	}

	int sizeT() { return list_.size(); }

private:
	std::vector<T> list_;
	int renderReady_ = 0;
};
*/
void ManagerTest();