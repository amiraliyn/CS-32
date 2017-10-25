#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;
class Compiler;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// ACTOR CLASS ////////////////////////////////////////////////////////////////////////////////

class Actor :public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld *student);

	virtual ~Actor() {}

	virtual void doSomething() = 0;

	void setMoved(bool change);

	bool getMoved() const;

	virtual bool blocksMovement() const;

	// Is this actor dead?
	virtual bool isDead() const;

	// Cause this actor to be be bitten, suffering an amount of damage.
	virtual void getBitten(int amt);

	// Cause this actor to be be poisoned.
	virtual void getPoisoned();

	// Cause this actor to be be stunned.
	virtual void getStunned();

	// Can this actor be picked up to be eaten?
	virtual bool isEdible() const;

	// Is this actor detected by an ant as a pheromone?
	virtual bool isPheromone(int colony) const;

	// Is this actor an enemy of an ant of the indicated colony?
	virtual bool isEnemy(int colony) const;

	// Is this actor detected as dangerous by an ant of the indicated colony?
	virtual bool isDangerous(int colony) const;

	// Is this actor the anthill of the indicated colony?
	virtual bool isAntHill(int colony) const;

	// Get this actor's world.
	StudentWorld* getWorld() const;

private:
	bool moved;
	bool m_alive;
	StudentWorld* sw;
};

// PEBBLE CLASS ////////////////////////////////////////////////////////////////////////////////

class Pebble :public Actor
{
public:
	Pebble(int startX, int startY, StudentWorld *student);

	virtual ~Pebble() {}

	virtual bool blocksMovement() const;

	virtual void doSomething();

private:
};

// DANGER CLASS ////////////////////////////////////////////////////////////////////////////////

class Danger :public Actor
{
public:
	Danger(int imageID, int startX, int startY, StudentWorld *student);

	virtual ~Danger() {}

	virtual bool isDangerous(int colony) const;

private:
};

// WATER CLASS ////////////////////////////////////////////////////////////////////////////////

class Water :public Danger
{
public:
	Water(int startX, int startY, StudentWorld *student);

	virtual ~Water() {}

	virtual void doSomething();

private:
};

// POISON CLASS ////////////////////////////////////////////////////////////////////////////////

class Poison :public Danger
{
public:
	Poison(int startX, int startY, StudentWorld *student);

	virtual ~Poison() {}

	virtual void doSomething();

private:
};

// ENERGYHOLDER CLASS ////////////////////////////////////////////////////////////////////////////////

class EnergyHolder : public Actor
{
public:

	EnergyHolder(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld *student, int hitPoints);

	virtual ~EnergyHolder() {}

	virtual bool isDead() const;

	int getHitPoints() const;

	void updateHitPoints(int hitPoints);

	// Add an amount of food to this actor's location.
	void addFood(int amt);

	// Have this actor pick up an amount of food.
	int pickupFood(int amt);

	// Have this actor pick up an amount of food and eat it.
	int pickupAndEatFood(int amt);

	// Does this actor become food when it dies?
	virtual bool becomesFoodUponDeath() const;

private:
	int m_hitPoints;
};

// ANTHILL CLASS ////////////////////////////////////////////////////////////////////////////////

class AntHill :public EnergyHolder
{
public:
	AntHill(int startX, int startY, StudentWorld *student, int colonyNum, Compiler* program);

	virtual ~AntHill() {}

	virtual void doSomething();

	virtual bool isAntHill(int colony) const;

private:
	int m_colony;
	Compiler* m_program;
};

// FOOD CLASS ////////////////////////////////////////////////////////////////////////////////

class Food : public EnergyHolder
{
public:
	Food(int startX, int startY, StudentWorld *student, int hitPoints);

	virtual ~Food() {}

	virtual void doSomething();

	virtual bool isEdible() const;

private:
};

// PHEROMONE CLASS ////////////////////////////////////////////////////////////////////////////////

class Pheromone : public EnergyHolder
{
public:
	Pheromone(int startX, int startY, StudentWorld *student, int colony);

	virtual ~Pheromone() {}

	virtual void doSomething();

	virtual bool isPheromone(int colony) const;

	void increaseStrength();

private:
	int m_colony;
};

// INSECT CLASS ////////////////////////////////////////////////////////////////////////////////

class Insect :public EnergyHolder
{
public:
	Insect(int imageID, int startX, int startY, StudentWorld *student, int hitPoints);

	virtual ~Insect() {}

	virtual void getBitten(int amt);

	virtual void getPoisoned();

	virtual void getStunned();

	virtual bool isEnemy(int colony);

	virtual bool becomesFoodUponDeath() const;

	virtual void setPool(bool change);

	int getSleepTicks() const;


	// Move this insect one step forward if possible, and return true;
	// otherwise, return false without moving.
	virtual bool moveForwardIfPossible();

	// Increase the number of ticks this insect will sleep by the indicated amount.
	void increaseSleepTicks(int amt);

private:
	int m_sleepTicks;
	int m_distance;
	int stunX;
	int stunY;
	bool samePool;
};

// ANT CLASS ////////////////////////////////////////////////////////////////////////////////

class Ant : public Insect
{
public:
	Ant(int imageID, int startX, int startY, int colony, StudentWorld* student, Compiler* program);

	virtual ~Ant() {}

	virtual void doSomething();
	virtual void getBitten(int amt);
	virtual bool isEnemy(int colony) const;
	virtual bool moveForwardIfPossible();
	bool Interpret();
	bool conditionTriggered(int cmd);

	

private:
	Compiler* c;
	int m_colony;
	int ic;
	int foodHolding;
	bool beenBitten, beenBlocked;
	int randNum;
};

// GRASSHOPPER CLASS ////////////////////////////////////////////////////////////////////////////////

class Grasshopper :public Insect
{
public:
	Grasshopper(int imageID, int startX, int startY, StudentWorld *student, int hitPoints);

	virtual ~Grasshopper() {}

private:
};

// BABYGRASSHOPPER CLASS ////////////////////////////////////////////////////////////////////////////////

class BabyGrasshopper : public Grasshopper
{
public:
	BabyGrasshopper(int startX, int startY, StudentWorld* student);

	virtual ~BabyGrasshopper() {}

	virtual void doSomething();
};

// ADULTGRASSHOPPER CLASS ////////////////////////////////////////////////////////////////////////////////

class AdultGrasshopper : public Grasshopper
{
public:
	AdultGrasshopper(int startX, int startY, StudentWorld* student);

	virtual ~AdultGrasshopper() {}

	virtual void getBitten(int amt);

	virtual void doSomething();
};

#endif // ACTOR_H_