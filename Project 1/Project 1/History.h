#ifndef History_H
#define History_H

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include "globals.h"
using namespace std;

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_rows;
	int m_cols;
	char displayGrid[MAXROWS][MAXCOLS];
};

#endif // !History_H