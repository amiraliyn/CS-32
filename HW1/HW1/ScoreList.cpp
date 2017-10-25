#include <iostream>
#include "ScoreList.h"
using namespace std;

ScoreList::ScoreList(){}
// Create an empty score list.

bool ScoreList::add(unsigned long score)
{
if (score <= 100)
{
if (size() == DEFAULT_MAX_ITEMS)
return false;
else
{
scoreL.insert(score);
return true;
}
}
else
return false;
}
// If the score is valid (a value from 0 to 100) and the score list
// has room for it, add it to the score list and return true.
// Otherwise, leave the score list unchanged and return false.

bool ScoreList::remove(unsigned long score)
{
if (scoreL.find(score) != -1)
{
scoreL.erase(scoreL.find(score));
return true;
}
return false;
}
// Remove one instance of the specified score from the score list.
// Return true if a score was removed; otherwise false.

int ScoreList::size() const
{
return scoreL.size();
}
// Return the number of scores in the list.

unsigned long ScoreList::minimum() const
{
	if (size() == 0)
		return NO_SCORE;
	for (unsigned long i = 0; i <= 100; i++)
	{
		if (scoreL.find(i) != -1)
			return i;
	}
}
// Return the lowest score in the score list.  If the list is
// empty, return NO_SCORE.

unsigned long ScoreList::maximum() const
{
if (size() == 0)
return NO_SCORE;
for (unsigned long i = 100; i >= 0; i--)
{
if (scoreL.find(i) != -1) {
return i;
}
}
}
// Return the highest score in the score list.  If the list is
// empty, return NO_SCORE.