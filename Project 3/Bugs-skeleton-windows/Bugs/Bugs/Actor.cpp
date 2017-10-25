#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <algorithm>
#include <cmath>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR CLASS ////////////////////////////////////////////////////////////////////////////////

Actor::Actor(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld *student)
	:GraphObject(imageID, startX, startY, dir, depth), moved(false), sw(student), m_alive(true)
{}

void Actor::setMoved(bool change)
{
	moved = change;
}

bool Actor::getMoved() const
{
	return moved;
}

bool Actor::blocksMovement() const
{
	return false;
}

// Is this actor dead?
bool Actor::isDead() const
{
	return false;
}

// Cause this actor to be be bitten, suffering an amount of damage.
void Actor::getBitten(int amt)
{
	return;
}

// Cause this actor to be be poisoned.
void Actor::getPoisoned()
{
	return;
}

// Cause this actor to be be stunned.
void Actor::getStunned()
{
	return;
}

// Can this actor be picked up to be eaten?
bool Actor::isEdible() const
{
	return false;
}

// Is this actor detected by an ant as a pheromone?
bool Actor::isPheromone(int colony) const
{
	return false;
}

// Is this actor an enemy of an ant of the indicated colony?
bool Actor::isEnemy(int colony) const
{
	return false;
}

// Is this actor detected as dangerous by an ant of the indicated colony?
bool Actor::isDangerous(int colony) const
{
	return false;
}

// Is this actor the anthill of the indicated colony?
bool Actor::isAntHill(int colony) const
{
	return false;
}

// Get this actor's world.
StudentWorld* Actor::getWorld() const
{
	return sw;
}

// PEBBLE CLASS ////////////////////////////////////////////////////////////////////////////////

Pebble::Pebble(int startX, int startY, StudentWorld *student)
	:Actor(IID_ROCK, startX, startY, right, 1, student)
{}

void Pebble::doSomething()
{
	return;
}

bool Pebble::blocksMovement() const
{
	return true;
}

// DANGER CLASS ////////////////////////////////////////////////////////////////////////////////

Danger::Danger(int imageID, int startX, int startY, StudentWorld *student)
	:Actor(imageID, startX, startY, right, 2, student)
{}

bool Danger::isDangerous(int colony) const
{
	return true;
}

// WATER CLASS ////////////////////////////////////////////////////////////////////////////////

Water::Water(int startX, int startY, StudentWorld *student)
	:Danger(IID_WATER_POOL, startX, startY, student)
{}

void Water::doSomething()
{
	StudentWorld* sw = getWorld();
	int x = getX();
	int y = getY();
	sw->stunAllStunnableAt(x, y);
}

// POISON CLASS ////////////////////////////////////////////////////////////////////////////////

Poison::Poison(int startX, int startY, StudentWorld *student)
	: Danger(IID_POISON, startX, startY, student)
{}

void Poison::doSomething()
{
	StudentWorld* sw = getWorld();
	int x = getX();
	int y = getY();
	sw->poisonAllPoisonableAt(x, y);
}

// ENERGYHOLDER CLASS ////////////////////////////////////////////////////////////////////////////////

EnergyHolder::EnergyHolder(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld *student, int hitPoints)
	:Actor(imageID, startX, startY, dir, depth, student), m_hitPoints(hitPoints)
{}

bool EnergyHolder::isDead() const
{
	if (m_hitPoints <= 0)
		return true;
	return false;
}

int EnergyHolder::getHitPoints() const
{
	return m_hitPoints;
}
 
void EnergyHolder::updateHitPoints(int hitPoints)
{
	m_hitPoints += hitPoints;
}

void EnergyHolder::addFood(int amt)
{
	int x = getX();
	int y = getY();
	StudentWorld* sw = getWorld();
	Actor* actor = sw->getEdibleAt(x, y);
	if (actor == nullptr)
		sw->addActor(new Food(x, y, sw, amt));
	else
		static_cast<Food*>(actor)->updateHitPoints(amt);
}

int EnergyHolder::pickupFood(int amt)
{
	Actor* actor = getWorld()->getEdibleAt(getX(), getY());
	if (actor != nullptr)
	{
		int amount = static_cast<Food*>(actor)->getHitPoints();
		int minFood = min(amt, amount);
		static_cast<Food*>(actor)->updateHitPoints(-minFood);
		return minFood;
	}
	return 0;
}

int EnergyHolder::pickupAndEatFood(int amt)
{
	int amount = pickupFood(amt);
	updateHitPoints(amount);
	return amount;
}

bool EnergyHolder::becomesFoodUponDeath() const
{
	return false;
}

// ANTHILL CLASS ////////////////////////////////////////////////////////////////////////////////

AntHill::AntHill(int startX, int startY, StudentWorld *student, int colonyNum, Compiler* program)
	:EnergyHolder(IID_ANT_HILL, startX, startY, right, 2, student, 8999), m_colony(colonyNum), m_program(program)
{}

void AntHill::doSomething()
{
	updateHitPoints(-1);
	int points = getHitPoints();
	if (points <= 0)
		return;
	int x = getX();
	int y = getY();
	StudentWorld* sw = getWorld();
	Actor* actor = sw->getEdibleAt(x, y);
	if (actor != nullptr && points < 10000)
	{
		pickupAndEatFood(min(10000, static_cast<Food*>(actor)->getHitPoints()));
	}
	if (points >= 2000 && m_program != nullptr)
	{
		updateHitPoints(-1500);
		sw->addActor(new Ant(IID_ANT_TYPE0 + m_colony, x, y, m_colony, sw, m_program));
		sw->increaseScore(m_colony);
	}
}

bool AntHill::isAntHill(int colony) const
{
	if (colony == m_colony)
		return true;
	return false;
}

// FOOD CLASS ////////////////////////////////////////////////////////////////////////////////

Food::Food(int startX, int startY, StudentWorld *student, int hitPoints)
	: EnergyHolder(IID_FOOD, startX, startY, right, 2, student, hitPoints)
{}

void Food::doSomething()
{
	return;
}

bool Food::isEdible() const
{
	return true;
}

// PHEROMONE CLASS ////////////////////////////////////////////////////////////////////////////////

Pheromone::Pheromone(int startX, int startY, StudentWorld *student, int colony)
	:EnergyHolder(IID_PHEROMONE_TYPE0 + colony, startX, startY, right, 2, student, 256),m_colony(colony)
{}

void Pheromone::doSomething()
{
	int hitPoints = getHitPoints();
	if (hitPoints <= 0)
		return;
	updateHitPoints(-1);
}

bool Pheromone::isPheromone(int colony) const
{
	if (m_colony == colony)
		return true;
	return false;
}

void Pheromone::increaseStrength()
{
	int hitPoints = getHitPoints();
	if (hitPoints >= 768)
		return;
	int hitPointsLeft = 768 - hitPoints;
	int addedStrength = min(hitPointsLeft, 256);
	updateHitPoints(addedStrength);
}

// INSECT CLASS ////////////////////////////////////////////////////////////////////////////////

Insect::Insect(int imageID, int startX, int startY, StudentWorld *student, int hitPoints)
	: EnergyHolder(imageID, startX, startY, Direction(randInt(1, 4)), 1, student, hitPoints), 
	m_distance(randInt(2, 10)), m_sleepTicks(0), stunX(0), stunY(0), samePool(false)
{}

void Insect::getBitten(int amt)
{
	updateHitPoints(-amt);
}

void Insect::getPoisoned()
{
	updateHitPoints(-150);
}

void Insect::getStunned()
{
	if (stunX == getX() && stunY == getY() && samePool == true)
	{
		return;
	}
	stunX = getX();
	stunY = getY();
	samePool = true;
	increaseSleepTicks(2);
}

int Insect::getSleepTicks() const
{
	return m_sleepTicks;
}

bool Insect::isEnemy(int colony)
{
	return true;
}

bool Insect::becomesFoodUponDeath() const
{
	return true;
}

bool Insect::moveForwardIfPossible()
{
	if (m_sleepTicks > 0)
	{
		m_sleepTicks--;
		return false;
	}

	if (m_distance == 0)
	{
		setDirection(Direction(randInt(1, 4)));
		m_distance = randInt(2, 10);
	}

	StudentWorld* sw = getWorld();
	int x = getX();
	int y = getY();

	int m_direction = getDirection();
	switch (m_direction)
	{
	case(up):
		y++;
		break;
	case(right):
		x++;
		break;
	case(down):
		y--;
		break;
	case(left):
		x--;
		break;
	}

	if (!sw->canMoveTo(x, y))
	{
		m_distance = 0;
		return false;
	}
	moveTo(x, y);
	samePool = false;
	m_distance--;
	return true;
	
}

void Insect::setPool(bool change)
{
	samePool = change;
}

void Insect::increaseSleepTicks(int amt)
{
	m_sleepTicks += amt;
}

// ANT CLASS ////////////////////////////////////////////////////////////////////////////////

Ant::Ant(int imageID, int startX, int startY, int colony, StudentWorld* student, Compiler* program)
	:Insect(IID_ANT_TYPE0 + colony, startX, startY, student, 1500), c(program), m_colony(colony), ic(0), 
	beenBitten (false), beenBlocked (false), randNum(0), foodHolding(0)
{}


void Ant::doSomething()
{
	if (c != nullptr)
	{
		updateHitPoints(-1);
		/*if (getHitPoints() <= 0)
			getWorld()->increaseScore(m_colony + 4);*/
		if (isDead())
			return;
		if (getSleepTicks() > 0)
		{
			increaseSleepTicks(-1);
			return;
		}

		if (!Interpret())
		{
			updateHitPoints(-getHitPoints());
			return;
		}
	}
	return;
}

bool Ant::moveForwardIfPossible()
{
	StudentWorld* sw = getWorld();
	int x = getX();
	int y = getY();

	int m_direction = getDirection();
	switch (m_direction)
	{
	case(up):
		y++;
		break;
	case(right):
		x++;
		break;
	case(down):
		y--;
		break;
	case(left):
		x--;
		break;
	default:
		break;
	}

	if (!sw->canMoveTo(x, y))
	{
		beenBlocked = true;
		return false;
	}
	moveTo(x, y);
	beenBlocked = false;
	setPool(false);
	return true;
}

void Ant::getBitten(int amt)
{
	beenBitten = true;
	updateHitPoints(-amt);
}

bool Ant::Interpret()
{
	int m_count = 10;
	for (m_count = 10; m_count > 0; m_count--)
	{
		Compiler::Command cmd;
		if (!c->getCommand(ic, cmd))
			return false; // error - no such instruction!
		switch (cmd.opcode)
		{
		case Compiler::invalid:
			return false;

		case Compiler::emitPheromone:
		{
			ic++;
			Actor* actor = getWorld()->getPheromoneAt(getX(), getY(), m_colony);
			if (actor == nullptr)
				getWorld()->addActor(new Pheromone(getX(), getY(), getWorld(), m_colony));
			else
				static_cast<Pheromone*>(actor)->increaseStrength();
			return true;
		}

		case Compiler::faceRandomDirection:
			setDirection(static_cast<GraphObject::Direction>(randInt(1, 4)));
			ic++;
			return true;

		case Compiler::rotateClockwise:
		{
			int m_direction = getDirection();
			switch (m_direction)
			{
			case(up):
				setDirection(right);
				break;
			case(right):
				setDirection(down);
				break;
			case(down):
				setDirection(left);
				break;
			case(left):
				setDirection(up);
				break;
			}
			ic++;
			return true;
		}

		case Compiler::rotateCounterClockwise:
		{
			int m_direction = getDirection();
			switch (m_direction)
			{
			case(up):
				setDirection(left);
				break;
			case(right):
				setDirection(up);
				break;
			case(down):
				setDirection(right);
				break;
			case(left):
				setDirection(down);
				break;
			}
			ic++;
			return true;
		}

		case Compiler::bite:
			getWorld()->biteEnemyAt(getX(),getY(),this, m_colony, 15);
			ic++;
			return true;

		case Compiler::pickupFood:
		{
			ic++;
			if (foodHolding + pickupFood(400) < 1800)
				foodHolding += pickupFood(400);
			else
			{
				pickupFood(1800 - foodHolding);
				foodHolding = 1800;
			}
			return true;
		}

		case Compiler::eatFood:
			if (foodHolding < 100)
			{
				updateHitPoints(foodHolding);
				foodHolding = 0;
			}
			else
			{
				foodHolding -= 100;
				updateHitPoints(100);
			}
			ic++;
			return true;

		case Compiler::dropFood:
		{
			Actor* actor = getWorld()->getEdibleAt(getX(), getY());
			if (actor != nullptr)
				static_cast<Food*>(actor)->updateHitPoints(foodHolding);
			foodHolding = 0;
			ic++;
			return true;
		}

		case Compiler::moveForward:
			if (moveForwardIfPossible())
			{ }
			++ic; // advance to next instruction
			return true;
		case Compiler::generateRandomNumber:
			randNum = randInt(0, stoi(cmd.operand1));
			++ic; // advance to next instruction
			break;
		case Compiler::if_command:
			// if the condition of the if command is
			// is true, then go to the target position
			// in the vector; otherwise fall through to
			// the next position
			if (conditionTriggered(stoi(cmd.operand1)))
				ic = stoi(cmd.operand2);
			else
				++ic; // just advance to the next line
			break;
		case Compiler::goto_command:
			// just set ic the specified position
			// in operand1
			ic = stoi(cmd.operand1);
			break;
		}
	}
	return true;
}

bool Ant::conditionTriggered(int cmd)
{
	switch (cmd)
	{
	case(Compiler::i_smell_danger_in_front_of_me):
		if (getWorld()->isDangerAt(getX(), getY(), m_colony) || getWorld()->isEnemyAt(getX(), getY(), m_colony))
			return true;
		return false;
	case(Compiler::i_smell_pheromone_in_front_of_me):
	{
		Actor* actor = getWorld()->getPheromoneAt(getX(), getY(), m_colony);
		if (actor != nullptr)
			return true;
		return false;
	}
		
	case(Compiler::i_was_bit):
		if (beenBitten)
			return true;
		return false;
	case(Compiler::i_am_carrying_food):
		if (foodHolding > 0)
			return true;
		return false;
	case(Compiler::i_am_hungry):
		if (getHitPoints() < 25)
			return true;
		return false;
	case(Compiler::i_am_standing_on_my_anthill):
		if (isAntHill(m_colony))
			return true;
		return false;
	case(Compiler::i_am_standing_on_food):
		if (getWorld()->getEdibleAt(getX(), getY()))
			return true;
		return false;
	case(Compiler::i_am_standing_with_an_enemy):
		if (getWorld()->isEnemyAt(getX(), getY(), m_colony))
			return true;
		return false;
	case(Compiler::i_was_blocked_from_moving):
		if (beenBlocked)
			return true;
		return false;
	case(Compiler::last_random_number_was_zero):
		if (randNum == 0)
			return true;
		return false;
	}
}

bool Ant::isEnemy(int colony) const
{
	if (colony == m_colony)
		return false;
	return true;
}

// GRASSHOPPER CLASS ////////////////////////////////////////////////////////////////////////////////

Grasshopper::Grasshopper(int imageID, int startX, int startY, StudentWorld *student, int hitPoints)
	: Insect(imageID, startX, startY, student, hitPoints)
{}

// BABYGRASSHOPPER CLASS ////////////////////////////////////////////////////////////////////////////////

BabyGrasshopper::BabyGrasshopper(int startX, int startY, StudentWorld* student)
	:Grasshopper(IID_BABY_GRASSHOPPER, startX, startY, student, 500)
{}

void BabyGrasshopper::doSomething()
{
	updateHitPoints(-1);
	int points = getHitPoints();
	if (points <= 0)
	{
		addFood(100);
		return;
	}
	StudentWorld* sw = getWorld();
	if (points >= 1600)
	{
		sw->addActor(new AdultGrasshopper(getX(), getY(), sw));
		updateHitPoints(-points);
		return;
	}

	int amount = pickupAndEatFood(200);
	if (amount > 0)
	{
		int randomRest = randInt(0, 1);
		switch (randomRest)
		{
		case(0):
			increaseSleepTicks(2);
			return;
		case(1):
			break;
		}
	}
	
	if (moveForwardIfPossible())
		increaseSleepTicks(2);
}

// ADULTGRASSHOPPER CLASS ////////////////////////////////////////////////////////////////////////////////

AdultGrasshopper::AdultGrasshopper(int startX, int startY, StudentWorld* student)
	: Grasshopper(IID_ADULT_GRASSHOPPER, startX, startY, student, 1600)
{}

void AdultGrasshopper::doSomething()
{
	updateHitPoints(-1);
	int points = getHitPoints();
	if (points <= 0)
	{
		addFood(100);
		return;
	}
	StudentWorld* sw = getWorld();
	int randomBite = randInt(0, 2);
	if (randomBite == 0)
		sw->biteEnemyAt(getX(), getY(), this, 5, 50);
	int amount = pickupAndEatFood(200);
	if (amount > 0)
	{
		int randomRest = randInt(0, 1);
		switch (randomRest)
		{
		case(0):
			increaseSleepTicks(2);
			return;
		case(1):
			break;
		}
	}
	int randomNum = randInt(0, 9);
	if (randomNum > 0)
	{
		if (moveForwardIfPossible())
			increaseSleepTicks(2);
		return;
	}
	int randomAngle = randInt(0, 359);
	int m_distance = randInt(2, 10);
	double radian = (randomAngle * 3.14) / 180;
	int m_y = m_distance * sin(radian);
	int m_x = m_distance * cos(radian);
	int x = getX();
	int y = getY();
	if (!sw->canMoveTo(x + m_x, y + m_y) || x + m_x >= VIEW_WIDTH || x + m_x < 0 || y + m_y < 0 || y + m_y >= VIEW_HEIGHT)
	{
		m_distance = 0;
		return;
	}
	moveTo(x + m_x, y + m_y);
	increaseSleepTicks(2);
	setPool(false);
	m_distance--;

}

void AdultGrasshopper::getBitten(int amt)
{
	Insect::getBitten(amt);
	StudentWorld* sw = getWorld();
	int randomNum = randInt(0, 1);
	switch (randomNum)
	{
	case(0):
		sw->biteEnemyAt(getX(), getY(), this, 5, 50);
		break;
	case(1):
		break;
	default:
		break;
	}
}