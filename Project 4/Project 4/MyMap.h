#ifndef MyMap_h
#define MyMap_h
// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#include "support.h"

template<typename KeyType, typename ValueType>
class MyMap
{
private:

	struct Node
	{
		Node(const KeyType& key, const ValueType& value) // Initialize the Node's m_key and m_value to key and value
			:m_key(key), m_value(value), left(nullptr), right(nullptr) // Set the left and right pointers to null pointer
		{}

		KeyType m_key;
		ValueType m_value;
		Node* left;
		Node* right;
	};

	Node* m_root;	// Creaate a root pointer
	int m_size;

	void FreeTree(Node* curr)	// Use this helper function to destroy all the nodes in the tree
	{
		if (curr == nullptr)
			return;

		FreeTree(curr->left);
		FreeTree(curr->right);

		delete curr;
	}

	const ValueType* findValue(const KeyType& key, Node* curr) const // Use this helper function to find a value in the tree
	{
		if (curr == nullptr)
			return nullptr;
		else if (curr->m_key == key)
			return &(curr->m_value);
		else if (curr->m_key > key)
			return findValue(key, curr->left);
		else
			return findValue(key, curr->right);
	}

public:

	MyMap()
		:m_root(nullptr), m_size(0) // Initialize the size to 0
	{
	}

	~MyMap()
	{
		clear(); // Call clear function
	}

	void clear()
	{
		FreeTree(m_root); // Use helper function FreeTree to destory the tree
	}

	int size() const
	{
		return m_size; // return the size of the tree
	}

	void associate(const KeyType& key, const ValueType& value) // add a new leaf to the tree
	{
		if (m_root == nullptr) // if tree is empty make a new tree/root
		{
			m_root = new Node(key, value);
			m_size++;
			return;
		}

		Node* curr = m_root;
		for (;;) //traverse through the tree until you find an empty spot to add a new leaf
		{
			if (curr->m_key == key) // if the key already exists in the tree, change its value to the given value
			{
				curr->m_value = value;
				return;
			}
			else if (curr->m_key > key)
			{
				if (curr->left != nullptr)
				{
					curr = curr->left; // if the current node has a left child move down to its child
				}
				else
				{
					curr->left = new Node(key, value); // add a new node to the tree
					m_size++;
					return;
				}
			}

			else
			{
				if (curr->right != nullptr) // if the current node has a right child move down to its child
				{
					curr = curr->right;
				}
				else
				{
					curr->right = new Node(key, value); // add a new node to the tree
					m_size++;
					return;
				}
			}
		}
	}

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const
	{
		return findValue(key, m_root); // Use helper function findValue to find a value given the key and the root of the tree
	}

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;
};
#endif /* MyMap_h */


//#ifndef MYMAP_INCLUDED
//#define MYMAP_INCLUDED
//
//#include <map>  // YOU MUST NOT USE THIS HEADER IN CODE YOU TURN IN
//#include "support.h"
//// In accordance with the spec, YOU MUST NOT TURN IN THIS CLASS TEMPLATE,
//// since you are not allowed to use any STL associative containers, and
//// this implementation uses std::map.
//
//// This code is deliberately obfuscated.
//
//// If you can not get your own MyMap class template working, you may use
//// this one during development in order to let you proceed with implementing
//// the other classes for this project; you can then go back to working on
//// fixing your own MyMap class template.
//
//template <typename KeyType, typename ValueType>
//class MyMap
//{
//public:
//	MyMap(const MyMap &other) = delete;
//	MyMap &operator=(const MyMap &other) = delete;
//	typedef KeyType O; using O1 = int; void clear() { l01.clear(); }
//	using l0 = ValueType; MyMap() {}using l10 = O const; O1 size()
//		const {
//		return l01.size();
//	}using ll0 = l0 const; using Const =
//		MyMap<O, l0>; void associate(l10&Using, ll0&first) {
//		l01[
//			Using] = first;
//	}using l1 = std::map<O, l0>; using l00 = Const
//		const; ll0*find(l10&l11)const {
//		auto first = l01.find(l11);
//		return(first != l01.end() ? &first->second : 0);
//	}l0*find(l10&
//		l01) {
//		return(l0*)(*(l00*)(this)).find(l01);
//	}private:l1
//		l01;
//};
//
//#endif // MYMAP_INCLUDED
