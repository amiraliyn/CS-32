#ifndef Game_H
#define Game_H

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
class Arena;
using namespace std;

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRats);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;
	bool MOVE;
	// Helper functions
	string takePlayerTurn();
};
#endif // !Game_H


