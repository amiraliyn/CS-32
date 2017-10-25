#include <iostream>
#include <cassert>
#include "Sequence.h"
using namespace std;

int main()
{
	ItemType hi;
	Sequence a;
	assert(a.empty() == true); // Tests empty function
	assert(a.size() == 0); // Tests size function
	for (int i = 0; i <= 3; i++)
		a.insert(i);
	a.insert(5); // Testing insert
	a.insert(5);
	a.insert(6);
	assert(a.size() == 7); // Tests size function
	assert(a.insert(5, 10) == true); // Should be able to insert
	assert(a.insert(5, 20) == true); // should insert at 5th position
	assert(a.remove(6) == 1); // Only one 6 so returns 1 removed
	assert(a.remove(5) == 2); // Removing 5's will return 2 items removed
	assert(a.get(0, hi) == true); // Checking get first element
	assert(a.get(10, hi) == false); // Checking you can't get if greater than size
	assert(a.get(3, hi) == true); // Checking get last element
	assert(a.set(2, hi) == true); // Can set value in middle
	assert(a.set(0, hi) == true); // Checking set first element
	assert(a.set(10, hi) == false); // Checking that you can't set if greater than size
	assert(a.set(3, hi) == true); // Checking set last element
	assert(a.find(3) == 0); // Checking find function
	assert(a.find(10) == 5); // Checking last element using find function

	Sequence b;
	b.insert(1);
	a.swap(b); // Checking swap function
	assert(a.get(4, hi) == false); // Checks that the arrays swapped
								   //b.dump(); // Dump to see if swapped
	Sequence c(b); // Checking copy contructor
				   //c.dump(); // Dump to see if correctly copy constructed
	a = c;
	//a.dump();
	Sequence seq1;
	seq1.insert(0, 1);
	seq1.insert(1, 3);
	seq1.insert(2, 4);
	seq1.insert(3, 5);
	seq1.insert(4, 4);
	seq1.insert(5, 5);
	seq1.insert(6, 2);
	seq1.insert(7, 2);
	seq1.insert(8, 1);
	seq1.dump();
	Sequence seq2;
	seq2.insert(0, 2);
	seq2.insert(1, 1);
	assert(subsequence(seq1, seq2) == 7); // Check subsequence function
										  //seq2.dump();
										  //a.dump();
	interleave(seq1, seq2, a);
	//a.dump(); // Dump to see if interleave function works properly

	std::cerr << "All tests passed!" << std::endl;
	return 0;
}
