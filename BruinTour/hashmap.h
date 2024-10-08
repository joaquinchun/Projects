#pragma once

#include <string>
#include <list>
#include <iostream>
using namespace std;

template <typename T>
class HashMap
{
public:
	HashMap(double max_load = 0.75)
		: myMaxLoad(max_load), mySize(0), myMaxSize(10)
	{
		myArray = new list<Node*>[10]();
	}
	~HashMap()
	{
		for (size_t i = 0; i < myMaxSize; ++i) {
			for (auto& nodePtr : myArray[i]) {
				delete nodePtr;
			}
		}
		delete[] myArray;
	}
	int size() const
	{
		return mySize;
	}
	void insert(const std::string& key, const T& value)
	{
		if (static_cast<double>(mySize + 1) > myMaxSize * myMaxLoad)
			resize();

		size_t hash = std::hash<std::string>()(key) % myMaxSize;

		if (myArray[hash].size() != 0)
			for (auto listIt = myArray[hash].begin(); listIt != myArray[hash].end(); ++listIt)
				if ((*listIt)->nodeKey == key)
				{
					(*listIt)->nodeValue = value;
					return;
				}
		Node* tempNode = new Node(key, value);
		myArray[hash].push_back(tempNode);
		myList.push_back(tempNode);
		++mySize;
	}
	T& operator[](const std::string& key)
	{
		return find(key);
	}
	const T* find(const std::string& key) const
	{
		size_t hash = std::hash<std::string>()(key) % myMaxSize;
		if (myArray[hash].size() == 0)
			return nullptr;
		for (auto listIt = myArray[hash].begin(); listIt != myArray[hash].end(); ++listIt)
			if ((*listIt)->nodeKey == key)
				return &((*listIt)->nodeValue);
		return nullptr;
	}
	T* find(const std::string& key)
	{
		const auto& hm = *this;
		return const_cast<T*>(hm.find(key));
	}
	HashMap(const HashMap&) = delete;
	HashMap& operator=(const HashMap&) = delete;
private:
	struct Node
	{
		Node(const std::string& k, const T& v)
			: nodeKey(k), nodeValue(v) {}
		std::string nodeKey;
		T nodeValue;
	};
	void resize()
	{
		delete[] myArray;
		myMaxSize *= 2;
		myArray = new list<Node*>[myMaxSize]();
		for (auto& nodePtr : myList)
		{
			size_t hash = std::hash<std::string>()(nodePtr->nodeKey) % myMaxSize;
			myArray[hash].push_back(nodePtr);
		}
	}
	double myMaxLoad;
	unsigned int mySize;
	unsigned int myMaxSize;
	list<Node*>* myArray;
	list<Node*> myList;
};
