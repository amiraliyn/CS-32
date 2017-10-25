#ifndef Support_h
#define Support_h

#include "provided.h"
#include <math.h>
#include <string>

struct Node		// struct a node in order to use A* in Navigator
{
	Node* m_parent;	
	GeoCoord m_GeoCoord;
	double remainingDistance, overallDistance, traveledDistance;
};

struct CompareNode : public std::binary_function<Node*, Node*, bool> // Optimized route 
{										//http://www.technical-recipes.com/2011/priority-queues-and-min-priority-queues-in-c/
	bool operator()(const Node* lhs, const Node* rhs) const
	{
		return lhs->overallDistance > rhs->overallDistance;
	}
};

bool operator<(const GeoCoord& firstGeoCoord, const GeoCoord& secondGeoCoord); // Overload < in order to compare GeoCoords

bool operator>(const GeoCoord& firstGeoCoord, const GeoCoord& secondGeoCoord); // Overload < in order to compare GeoCoords

bool operator==(const GeoCoord& firstGeoCoord, const GeoCoord& secondGeoCoord); // Overload == in order to compare GeoCoords

bool operator!=(const GeoCoord& firstGeoCoord, const GeoCoord& secondGeoCoord); // Overload != in order to compare GeoCoords

bool operator==(const GeoSegment& firstGeoSegment, const GeoSegment& secondGeoSegment); // Overload == in order to compare GeoSegments

bool operator!=(const GeoSegment& firstGeoSegment, const GeoSegment& secondGeoSegment); // Overload != in order to compare GeoSegments

#endif // !Support_h