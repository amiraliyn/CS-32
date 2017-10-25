#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <list>
#include <string>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Compiler;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	virtual ~StudentWorld() {}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	// Can an insect move to x,y?
	bool canMoveTo(int x, int y) const;

	// Add an actor to the world
	void addActor(Actor* a);

	// If an item that can be picked up to be eaten is at x,y, return a
	// pointer to it; otherwise, return a null pointer.  (Edible items are
	// only ever going be food.)
	Actor* getEdibleAt(int x, int y) const;

	// If a pheromone of the indicated colony is at x,y, return a pointer
	// to it; otherwise, return a null pointer.
	Actor* getPheromoneAt(int x, int y, int colony) const;

	// Is an enemy of an ant of the indicated colony at x,y?
	bool isEnemyAt(int x, int y, int colony) const;

	// Is something dangerous to an ant of the indicated colony at x,y?
	bool isDangerAt(int x, int y, int colony) const;

	// Is the anthill of the indicated colony at x,y?
	bool isAntHillAt(int x, int y, int colony) const;

	// Bite an enemy of an ant of the indicated colony at me's location
	// (other than me; insects don't bite themselves).  Return true if an
	// enemy was bitten.
	bool biteEnemyAt(int x, int y, Actor* me, int colony, int biteDamage);

	//// Poison all poisonable actors at x,y.
	void poisonAllPoisonableAt(int x, int y);

	// Stun all stunnable actors at x,y.
	void stunAllStunnableAt(int x, int y);

	// Record another ant birth for the indicated colony.
	void increaseScore(int colony);

	int theWinnerColony();

	void displayText();

private:
	std::list<Actor*> m_actor[VIEW_WIDTH][VIEW_HEIGHT];
	int tick;
	int count0, count1, count2, count3;
	std::vector<std::string> compilerProgram;
	Compiler* compiler[4];
	int compilerSize;
};

#endif // STUDENTWORLD_H_
