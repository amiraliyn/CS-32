#include <iostream>
#include <stack>
using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	stack<Coord> coordStack;     // Declare a stack of Coords
	
	Coord current(sr, sc);			 // Starting coordinate
	int currentRow = sr;
	int currentCol = sc;

	coordStack.push(current);
	maze[sr][sc] = '#';

	while (!coordStack.empty())
	{
		current = coordStack.top();
		currentRow = current.r();
		currentCol = current.c();
		coordStack.pop();

		if (currentRow == er && currentCol == ec)	// Check to see if you are at the end point
			return true;
		if (maze[currentRow - 1][currentCol] == '.')	// Check to see if you can move North
		{
			coordStack.push(Coord(currentRow - 1, currentCol));
			maze[currentRow - 1][currentCol] = '#';
		}
		if (maze[currentRow][currentCol + 1] == '.')	// Check to see if you can move East
		{
			coordStack.push(Coord(currentRow, currentCol + 1));
			maze[currentRow][currentCol + 1] = '#';
		}
		if (maze[currentRow + 1][currentCol] == '.')	// Check to see if you can move South
		{
			coordStack.push(Coord(currentRow + 1, currentCol));
			maze[currentRow + 1][currentCol] = '#';
		}
		if (maze[currentRow][currentCol - 1] == '.')	// Check to see if you can move West
		{
			coordStack.push(Coord(currentRow, currentCol - 1));
			maze[currentRow][currentCol - 1] = '#';
		}
	}
	return false;	// If the maze is not solvable, return false
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};


	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}