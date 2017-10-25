#ifndef Sequence_H
#define Sequence_H

#include <iostream>
#include <string>
typedef unsigned long ItemType;

class Sequence
{
public:
	Sequence();
	Sequence(const Sequence &old);
	Sequence &operator=(const Sequence &old);
	~Sequence();
	bool empty() const;
	int size() const;
	bool insert(int pos, const ItemType& value);
	int insert(const ItemType& value);
	bool erase(int pos);
	int remove(const ItemType& value);
	bool get(int pos, ItemType& value) const;
	bool set(int pos, const ItemType& value);
	int find(const ItemType& value) const;
	void swap(Sequence& other);
	void dump() const;

private:
	struct Node
	{
		ItemType value;
		Node *next, *prev;
	};
	Node *head;
	int m_size;
	void addToFront(ItemType v);
	void addItem(int pos, ItemType v);
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);
#endif // !Sequence_H