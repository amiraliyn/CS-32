#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include "globals.h"
#include "History.h"
#include "Arena.h"
using namespace std;

History::History(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			displayGrid[r - 1][c - 1] = '.';
}

bool History::record(int r, int c) 
{
	if (r <= 0 || c <= 0 || r > m_rows || c > m_cols)
		return false;

	else
	{
		if (displayGrid[r-1][c-1] == '.')
			displayGrid[r-1][c-1] = 'A';
		else if (displayGrid[r-1][c-1] != '.' || displayGrid[r-1][c-1] != 'Z')
			displayGrid[r-1][c-1] ++;
		return true;
	}	
}

void History::display() const
{
	clearScreen();
	for (int r = 1; r <= m_rows; r++)
	{
		for (int c = 1; c <= m_cols; c++)
			cout << displayGrid[r - 1][c - 1];
		cout << endl;
	}
	cout << endl;
}