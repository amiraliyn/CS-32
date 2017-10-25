#ifndef Rat_H
#define Rat_H

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

class Arena;  // This is needed to let the compiler know that Arena is a
			  // type name, since it's mentioned in the Rat declaration.

class Rat
{
public:
	// Constructor
	Rat(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	void move();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	int    m_health;
	int    m_idleTurnsRemaining;
};

#endif // !Rat_H


