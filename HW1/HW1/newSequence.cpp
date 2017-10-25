#include <iostream>
#include "newSequence.h"
using namespace std;

Sequence::Sequence()// Create an empty sequence (i.e., one whose size() is 0).
{
	m_size = 0;
	m_items = DEFAULT_MAX_ITEMS;
	sequence = new ItemType[m_items];
}

Sequence::Sequence(int items)// Create an empty sequence (i.e., one whose size() is 0).
{
	m_size = 0;
	m_items = items;
	sequence = new ItemType[m_items];
}

Sequence::Sequence(const Sequence &ptr)
{
	m_size = ptr.m_size;
	m_items = ptr.m_items;
	sequence = new ItemType[m_items];
	for (int i = 0; i < m_size; i++)
		sequence[i] = ptr.sequence[i];
}

Sequence &Sequence::operator=(const Sequence &old)
{
	if (&old == this)
		return *this;
	delete[] sequence;
	m_size = old.m_size;
	m_items = old.m_items;
	sequence = new ItemType[m_items];
	for (int i = 0; i < m_size; i++)
		sequence[i] = old.sequence[i];
	return *this;
}

Sequence::~Sequence()
{
	delete [] sequence;
}

bool Sequence::empty() const  // Return true if the sequence is empty, otherwise false.
{
	return (m_size == 0);
}

int Sequence::size() const    // Return the number of items in the sequence.
{
	return m_size;
}

bool Sequence::insert (int pos, const ItemType& value)
{
	if (size() == m_items)
		return false;
	if(pos >= 0 && pos <= size())
	{
		for (int i = size() - 1; i >= pos; i--)
			sequence[i + 1] = sequence[i];
		sequence[pos] = value;
		m_size++;
		return true;
	}
	return false;
}

// Insert value into the sequence so that it becomes the item at
// position pos.  The original item at position pos and those that
// follow it end up at positions one higher than they were at before.
// Return true if 0 <= pos <= size() and the value could be
// inserted.  (It might not be, if the sequence has a fixed capacity,
// (e.g., because it's implemented using a fixed-size array) and is
// full.)  Otherwise, leave the sequence unchanged and return false.
// Notice that if pos is equal to size(), the value is inserted at the
// end.

int Sequence::insert(const ItemType& value)
{
	if (size() == m_items)
		return -1;
	int p = 0;
	for ( int i = 0; i < size(); i++)
	{
		if (value <= sequence[i])
		{
			p = i;
			break;
		}
	}
	if (value > sequence[p])
	{
		p = size();
	}
	insert(p, value);
	return p;
}
// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item at position
// p.  The original item at position p and those that follow it end
// up at positions one higher than before.  Return p if the value
// was actually inserted.  Return -1 if the value was not inserted
// (perhaps because the sequence has a fixed capacity and is full).

bool Sequence::erase(int pos)
{
	if (pos >= size() || pos < 0)
		return false;
	else
	{
		for (int i = pos + 1; i < size(); i++)
			sequence[i-1] = sequence[i];
		m_size--;
		return true;
	}
	
}
// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed this item end up at
// positions one lower than they were at before), and return true.
// Otherwise, leave the sequence unchanged and return false.

int Sequence::remove(const ItemType& value)
{
	int count = 0;
	if (size() != 0)
	{
		for (int i = 0; i < size(); i++)
		{
			if (sequence[i] == value)
			{
				erase(i);
				count++;
			}
		}
	}
	return count;
}
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).

bool Sequence::get (int pos, ItemType& value) const
{
	if (pos >= 0 && pos < size())
	{
		value = sequence[pos];
		return true;
	}
	else
		return false;
}
// If 0 <= pos < size(), copy into value the item at position pos
// in the sequence and return true.  Otherwise, leave value unchanged
// and return false.

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos >= 0 && pos < size())
	{
		sequence[pos] = value;
		return true;
	}
	else
		return false;
}
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.

int Sequence::find(const ItemType& value) const
{
	int p = 0;
	bool if_find = false;
	for (int i = 0; i < size(); i++)
	{
		if (value == sequence[i])
		{
			if_find = true;
			p = i;
			break;
		}
	}
	if (!if_find)
	p = -1;
	return p;
}
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.

void Sequence::swap(Sequence& other)
{
	Sequence tmpnum(other);
	other = *this;
	*this = tmpnum;
}
// Exchange the contents of this sequence with the other one.