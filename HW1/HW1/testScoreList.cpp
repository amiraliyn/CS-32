#include <iostream>
#include "scorelist.h"
using namespace std;

int main()
{
	ScoreList sl;
	
	sl.add(15);
	sl.add(25);
	sl.add(95);
	sl.add(53);
	sl.remove(53);
	sl.add(-10);
	cout << sl.maximum() << endl;
	cout << sl.minimum() << endl;
	cout << sl.size() << endl;
	cout << "passed all tests" << endl;
	return 0;
}
