// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	if (sr == er && sc == ec)	// Check to see if you are at the end point
		return true;

	maze[sr][sc] = '#';

	if (maze[sr - 1][sc] == '.')	// Check to see if you can move North
	{
		if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
			return true;
	}
	if (maze[sr][sc + 1] == '.')	// Check to see if you can move East
	{
		if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
			return true;
	}
	if (maze[sr + 1][sc] == '.')	// Check to see if you can move South
	{
		if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
			return true;
	}
	if (maze[sr][sc - 1] == '.')	// Check to see if you can move West
	{
		if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
			return true;
	}
	return false;
}