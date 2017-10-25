#include "StudentWorld.h"
#include "Field.h"
#include "Actor.h"
#include "Compiler.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir), count0(0), count1(0), count2(0), count3(0), compilerSize(0)
{
}

int StudentWorld::init()
{
	tick = 0;
	Field f;
	string fieldFile = getFieldFilename();
	string error;

	if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	{
		setError(fieldFile + " " + error);
		return false; // something bad happened!
	}

	compilerProgram = getFilenamesOfAntPrograms();
	compilerSize = compilerProgram.size();

	for (int i = 0; i < 4; i++)
	{
		compiler[i] = nullptr;
	}

	for (int i = 0; i < compilerSize ; i++)
	{
		compiler[i] = new Compiler();
		if (!compiler[i]->compile(compilerProgram[i], error))
		{
			setError(compilerProgram[i] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
	}
	
	// otherwise the load was successful and you can access the
	// contents of the field – here’s an example

	for (int x = 0; x < VIEW_WIDTH; x++)
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			Field::FieldItem item = f.getContentsOf(x, y); // note it’s x,y and not y,x!!!
			switch (item)
			{
			case Field::FieldItem::rock:
				m_actor[y][x].push_back(new Pebble(x, y, this));
				//count++;
				break;
			case Field::FieldItem::grasshopper:
				m_actor[y][x].push_back(new BabyGrasshopper(x, y, this));
				//count++;
				break;
			case Field::FieldItem::food:
				m_actor[y][x].push_back(new Food(x, y, this, 6000));
				//count++;
				break;
			case Field::FieldItem::anthill0:
			{
				if (compiler[0] == nullptr)
					break;
				m_actor[y][x].push_back(new AntHill(x, y, this, 0, compiler[0]));
				//count++;
				break;
			}
			case Field::FieldItem::anthill1:
			{
				if (compiler[1] == nullptr)
					break;
				m_actor[y][x].push_back(new AntHill(x, y, this, 1, compiler[1]));
				//count++;
				break;
			}
			case Field::FieldItem::anthill2:
			{
				if (compiler[2] == nullptr)
					break;
				m_actor[y][x].push_back(new AntHill(x, y, this, 2, compiler[2]));
				//count++;
				break;
			}
			case Field::FieldItem::anthill3:
			{
				if (compiler[3] == nullptr)
					break;
				m_actor[y][x].push_back(new AntHill(x, y, this, 3, compiler[3]));
				//count++;
				break;
			}
			case Field::FieldItem::water:
				m_actor[y][x].push_back(new Water(x, y, this));
				//count++;
				break;
			case Field::FieldItem::poison:
				m_actor[y][x].push_back(new Poison(x, y, this));
				//count++;
				break;
			default:
				break;
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	tick++; // update the current tick # in the simulation
			// the term "actors" refers to all ants, anthills, poison, pebbles,
			// baby and adult grasshoppers, food, pools of water, etc.
			// give each actor a chance to do something

	if (tick <= 2000)
	{
		for (int x = 0; x < VIEW_WIDTH; x++)
		{
			for (int y = 0; y < VIEW_HEIGHT; y++)
			{
				for (list<Actor*>::iterator it = m_actor[y][x].begin(); it != m_actor[y][x].end(); it++)
				{
					(*it)->setMoved(false);
				}
			}
		}
		for (int x = 0; x < VIEW_WIDTH; x++)
		{
			for (int y = 0; y < VIEW_HEIGHT; y++)
			{
				//for (list<Actor*>::iterator it = m_actor[y][x].begin(); it != m_actor[y][x].end(); it++) // ERROR! List erase iterator
				list<Actor*>::iterator it = m_actor[y][x].begin();										   // outside range
				while (it != m_actor[y][x].end())
				{
					if ((*it)->isDead())
					{
						delete *it;
						it = m_actor[y][x].erase(it);
					}
					else
					{ 
						if ((*it)->getMoved())
							it++;
						else
						{
							// get the actor’s current location
							int oldx = (*it)->getX();
							int oldy = (*it)->getY();
							Actor* tmp = *it;
							(*it)->doSomething();
							(*it)->setMoved(true);
							int newx = (*it)->getX();
							int newy = (*it)->getY();
							if (oldx != newx || oldy != newy)
							{
								m_actor[newy][newx].push_back(tmp);
								it = m_actor[oldy][oldx].erase(it);
							}
							else
								it++;
						}
					}
					
				}
			}
		
		}

		displayText(); // update the ticks/ant stats text at screen top
					   //if the simulation’s over (ticks == 2000) then see if we have a winner

		return GWSTATUS_CONTINUE_GAME;
	}

	if (count0 == 5 && count1 == 5 && count2 == 5 && count3 == 5)
		return GWSTATUS_NO_WINNER;
	else
	{
		int theWinner = theWinnerColony();
		if(theWinner == count0)
			setWinner(compilerProgram[0]);
		if (theWinner == count1)
			setWinner(compilerProgram[1]);
		if (theWinner == count2)
			setWinner(compilerProgram[2]);
		if (theWinner == count3)
			setWinner(compilerProgram[3]);
		return GWSTATUS_PLAYER_WON;
	}
}

int StudentWorld::theWinnerColony()
{
	int winner1 = max(count0, count1);
	int winner2 = max(count2, count3);
	return max(winner1, winner2);
}

void StudentWorld::cleanUp()
{
	for (int x = 0; x < VIEW_WIDTH; x++)
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			for (list <Actor*>::iterator it = m_actor[y][x].begin(); it != m_actor[y][x].end();)
			{
				delete *it;
				it = m_actor[y][x].erase(it);
			}
		}
	}

	for (int i = 0; i < compilerSize; i++)
	{
		delete compiler[i];
	}
}


void StudentWorld::displayText()
{
	ostringstream display;
	if (compilerSize == 1)
		display << "Ticks: " << 2000 - tick << " - " << compilerProgram[0] << ": " << count0;
	else if (compilerSize == 2)
	{
		display << "Ticks: " << 2000 - tick << " - " << compilerProgram[0] << ": " << count0 << " - " << compilerProgram[1]
			<< ": " << count1;
	}
	else if (compilerSize == 3)
	{
		display << "Ticks: " << 2000 - tick << " - " << compilerProgram[0] << ": " << count0 << " - " << compilerProgram[1]
			<< ": " << count1 << " - " << compilerProgram[2] << ": " << count2;
	}
	else if (compilerSize == 4)
	{
		display << "Ticks: " << 2000 - tick << " - " << compilerProgram[0] << ": " << count0 << " - " << compilerProgram[1]
			<< ": " << count1 << " - " << compilerProgram[2] << ": " << count2 << " - " << compilerProgram[3] << ": " << count3;
	}
	string gameStart = display.str();
	setGameStatText(gameStart);
}

void StudentWorld::addActor(Actor* a)
{
	int x = a->getX();
	int y = a->getY();
	m_actor[y][x].push_back(a);
}

bool StudentWorld::canMoveTo(int x, int y) const
{
	if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT)
		return false;
	for (list<Actor*>::const_iterator it = m_actor[y][x].begin(); it != m_actor[y][x].end(); it++)
	{
		if ((*it)->blocksMovement())
			return false;
	}
	return true;
}

Actor* StudentWorld::getEdibleAt(int x, int y) const
{
	for (list<Actor*>::const_iterator it = m_actor[y][x].begin(); it != m_actor[y][x].end(); it++)
	{
		if ((*it)->isEdible())
			return *it;
	}
	return nullptr;
}

Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
	for (list<Actor*>::const_iterator it = m_actor[y][x].begin(); it != m_actor[y][x].end(); it++)
	{
		if ((*it)->isPheromone(colony))
			return *it;
	}
	return nullptr;
}

bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
	for (list<Actor*>::const_iterator it = m_actor[y][x].begin(); it != m_actor[y][x].end(); it++)
	{
		if ((*it)->isEnemy(colony))
			return true;
	}
	return false;
}

bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
	for (list<Actor*>::const_iterator it = m_actor[y][x].begin(); it != m_actor[y][x].end(); it++)
	{
		if ((*it)->isDangerous(colony))
			return true;
	}
	return false;
}

bool StudentWorld::isAntHillAt(int x, int y, int colony) const
{
	for (list<Actor*>::const_iterator it = m_actor[y][x].begin(); it != m_actor[y][x].end(); it++)
	{
		if ((*it)->isAntHill(colony))
			return true;
	}
	return false;
}

bool StudentWorld::biteEnemyAt(int x, int y, Actor* me, int colony, int biteDamage)
{
	vector<Actor*> enemies;
	for (list<Actor*>::iterator it = m_actor[y][x].begin(); it != m_actor[y][x].end();
		it++)
	{
		if ((*it) != me && (*it)->isEnemy(colony))
			enemies.push_back(*it);
	}
	if (!enemies.empty())
	{
		int randomNum = randInt(0, enemies.size() - 1);
		enemies[randomNum]->getBitten(biteDamage);
		return true;
	}
	return false;
}

void StudentWorld::poisonAllPoisonableAt(int x, int y)
{
	list<Actor*>::iterator it = m_actor[y][x].begin();
	if ((*it) == nullptr)
		return;
	for (; it != m_actor[y][x].end(); it++)
		(*it)->getPoisoned();
}

void StudentWorld::stunAllStunnableAt(int x, int y)
{
	list<Actor*>::iterator it = m_actor[y][x].begin();
	if ((*it) == nullptr)
		return;
	for (; it != m_actor[y][x].end(); it++)
		(*it)->getStunned();
}

void StudentWorld::increaseScore(int colony)
{
	switch (colony)
	{
	case(0):
		count0++;
		break;
	case(1):
		count1++;
		break;
	case(2):
		count2++;
		break;
	case(3):
		count3++;
		break;
	}
}