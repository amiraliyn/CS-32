#include <iostream>
#include "Sequence.h"
using namespace std;

Sequence::Sequence() // Create an empty sequence (i.e., one whose size() is 0).
{
	head = nullptr;
	m_size = 0;
}

Sequence::Sequence(const Sequence &old)	 // Use the get and insert functions to copy everything 
{
	head = nullptr; // Create an empty sequence
	m_size = 0;
	for (int new_pos = 0; new_pos < old.size(); new_pos++) // Loop through the given LinkedList and use get and insert functions
	{													   // to make a copy of the old LinkedList into the new sequence
		ItemType new_value;
		old.get(new_pos, new_value);
		insert(new_pos, new_value);
	}
}

Sequence &Sequence::operator=(const Sequence &old)
{
	if (&old == this)	// Avoid aliasing
		return *this;
	Sequence temp(old);	// Use copy constructor to replicate the information in old sequence into a temporary sequence
	swap(temp);			// Swap the information between the sequence and the temporary sequence and then get rid
	return *this;		// of the temporary one when you exit the function
}

Sequence::~Sequence() // Destroys every node
{
	Node *p = head; // Make a new node which points to the top node
	while (p != nullptr) // Travers through the LinkedList as long as your node is pointing at something
	{
		Node *n = p->next;	// Make a new node which point to the node after p
		delete p;			// Get rid of p
		p = n;				// Pass value of n to p
	}
}

bool Sequence::empty() const	// Return true if the sequence is empty, otherwise false.
{
	return m_size == 0;
}

int Sequence::size() const	// Return the number of items in the sequence.
{
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value) // Insert the given value in a specific position
{
	if (pos < 0 || pos > size()) // Return false if 0 > pos or pos > size() and the value could not be inserted 
		return false;
	addItem(pos, value); // Call addItem function to add a new node to LinkedList
	return true;
}

int Sequence::insert(const ItemType& value)	// Insert the given value in an ascending order
{
	int pos;
	Node *p = head;
	for (pos = 0; pos < size() && value > p->value; pos++)	// Determine the position where the value should be inserted
		p = p->next;
	addItem(pos, value);
	return pos;
}

bool Sequence::erase(int pos)	// Erase the value in the given position
{
	if (pos < 0 || pos >= size())
		return false;
	Node *killMe = head;
	int count = 0;
	while (killMe != nullptr)
	{
		if (count == pos)
		{
			if (killMe->prev != nullptr)	// Check every possible position and make sure the previous and next nodes are connected appropriately
			{
				if (killMe->next != nullptr)
				{
					killMe->prev->next = killMe->next;
					killMe->next->prev = killMe->prev;
				}		
				else
					killMe->prev->next = nullptr;
			}
				
			else
			{
				if (killMe->next != nullptr)
				{
					head = killMe->next;
					killMe->next->prev = nullptr;
				}
				else
					head = nullptr;	
			}	
			delete killMe;
			m_size--;
			break;
		}
		count++;
		killMe = killMe->next;
	}
	return true;
}

int Sequence::remove(const ItemType& value) // Erase all items from the sequence that == value.  Return the
{											// number of items removed (which will be 0 if no item == value).
	int count = 0;
	int pos = 0;
	Node *p = head;
	while(p != nullptr)
	{
		if (p->value == value)	// If the value at position p == value delete it 
		{
			p = p->next;
			count++;
			erase(pos);
		}
		else				   // Otherwise go to the next position
		{
			pos++;
			p = p->next;
		}
	}
	return count;
}

bool Sequence::get(int pos, ItemType& value) const	// If 0 <= pos < size(), copy into value the item at position pos                                           
{													// in the sequence and return true.  Otherwise, leave value unchanged
	if (pos < 0 || pos >= size())				    // and return false.
		return false;
	Node *p = head;
	for (int i = 0; i < pos; i++)
		p = p->next;
	value = p->value;
	return true;
}

bool Sequence::set(int pos, const ItemType& value)	// If 0 <= pos < size(), replace the item at position pos in the sequence and return true
{
	if (pos < 0 || pos >= size())					// Otherwise leave the sequence unchanged and return false
		return false;
	Node *p = head;
	for (int i = 0; i < pos; i++)
		p = p->next;
	p->value = value;
	return true;
}

int Sequence::find(const ItemType& value) const		// Find the smallest integer point at which its value == value and return p
{
	int pos = 0;
	Node *p = head;
	while (p != nullptr)
	{
		if (p->value == value)
			return pos;
		p = p->next;
		pos++;
	}
	return -1;	// If no such integer exists, return -1
}

void Sequence::swap(Sequence& other)	// Swap two sequences by switching their sizes and head pointers
{
	Node *p = other.head;
	other.head = this->head;
	this->head = p;

	int t_size = other.m_size;
	other.m_size = m_size;
	m_size = t_size;
}

void Sequence::dump() const	// Print out the test cases in order to make sure program is working correctly
{
	Node *p = head;
	cerr << "sequence: ";
	while (p != nullptr)
	{
		cerr << p->value << " ";
		p = p->next;
	}
	cerr << endl;
	cerr << "size: " << size() << endl << endl;
}

void Sequence::addToFront(ItemType value) // Add a new node to the front of LinkedList
{
	Node *p = new Node; //Make an empty node
	p->value = value; //put value in node
	if (head != nullptr) // If LinkedList is not empty, link the previous node of the old head to the new head
		head->prev = p;
	p->next = head; 
	p->prev = nullptr;
	head = p;
}

void Sequence::addItem(int pos, ItemType value) // Add a new node to the LinkedList and increment the size
{
	if (pos == 0) // Add item to the front of LinkedList
		addToFront(value);
	else	// Add item anywhere else
	{
		int count = 0;
		Node *p = head;
		while (p->next != nullptr)
		{
			if (pos - 1 == count)
				break;
			p = p->next;
			count++;
		}
		Node *latest = new Node;
		latest->value = value;
		latest->next = p->next;
		latest->prev = p;
		p->next = latest;
		if (latest->next != nullptr)	// If latest is not in the last place, connect it to the next node
			latest->next->prev = latest;
	}
	m_size++;
}

int subsequence(const Sequence& seq1, const Sequence& seq2) // If seq2 is a subsequence of seq1, return the earliest point
{
	if (seq1.size() >= seq2.size() && !seq2.empty()) // If seq2 is empty or bigger than seq1, return -1
	{
		bool subFound = false;
		ItemType firstValue, secondValue;
		int firstPos;
		for (int pos = 0; pos < seq1.size(); pos++)	// Go through seq1 to see if there exists a value which matches seq2 first value
		{
			seq2.get(0, secondValue);
			seq1.get(pos, firstValue);
			if (firstValue == secondValue)
			{
				firstPos = pos;
				for (int n = 0; n < seq2.size(); n++)	// Go through seq2 to see if the rest of seq2 elements are also in seq1
				{
					int m = firstPos;
					m += n;
					subFound = true;
					seq2.get(n, secondValue);
					seq1.get(m, firstValue);
					if (firstValue != secondValue) // If an element in seq2 does not exist in seq1 
					{							   // break the loop and change the boolean value to false
						subFound = false;
						break;
					}
				}
				if (subFound)
					return firstPos;	// If seq2 is a subsequence of seq1, return the earliest position in seq1
			}
		}
	}
	return -1;			// If seq2 is not a subsequence of seq1, return -1	
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result) // Insert the values in seq1 and seq2 into another sequence (result)
{
	Sequence temp;	// Create a temporary sequence
	ItemType test1, test2;
	bool test = false;
	bool secondTest = false;
	if (seq1.empty())	// If seq1 is empty, set temp equal to seq2
		temp = seq2;	
	else if (seq2.empty())	// If seq2 is empty, set temp equal to seq1
		temp = seq1;
	else if (seq1.size() == seq2.size())	// Check for aliasing
	{
		for (int i = 0; i < seq1.size(); i++)
		{
			test = true;
			seq1.get(i, test1);
			seq2.get(i, test2);
			if (test1 != test2)
			{
				test = false;
				break;
			}
		}
		if (test) // If seq1 == seq2 return, set temp to one of them
			temp = seq1;
	}

	else
		secondTest = true;
	
	if (secondTest)	// Insert the values in seq1 and seq2 into temp
	{
		int pos1 = 0, pos2 = 0;
		int pos_temp = 0;
		ItemType firstValue, secondValue;
		while (pos1 < seq1.size() || pos2 < seq2.size())
		{
			if (pos1 < seq1.size())
			{
				seq1.get(pos1, firstValue);
				temp.insert(pos_temp, firstValue);
				pos1++;
				pos_temp++;
			}
			if (pos2 < seq2.size())
			{
				seq2.get(pos2, secondValue);
				temp.insert(pos_temp, secondValue);
				pos2++;
				pos_temp++;
			}
		}
	}
	result = temp;	// Set result equal to temp
}