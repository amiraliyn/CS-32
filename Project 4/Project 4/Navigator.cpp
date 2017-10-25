#include "provided.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <stack>
#include <iostream>
using namespace std;

class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	MapLoader ml; 
	AttractionMapper am;
	SegmentMapper sm;
	StreetSegment searchForTheStreet(GeoSegment segment) const; // Use a helper function to find the street
	string getDirectionAngle(GeoSegment firstSeg, GeoSegment secondSeg) const; // Use this helper function to find the direction in 
																			   //which you have to move
	string getDirection(GeoSegment currentSeg) const;						   //Use this helper function to find whether you have
																			   // to make a right or left turn
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	if (ml.load(mapFile)) // If MapLoader loads the map, initialize AtrractionMapper and SegmentMapper and return true
	{
		am.init(ml);
		sm.init(ml);
		return true;
	}
	cerr << "The data was not loaded successfully!" << endl;
	return false;	// Otherwise return false
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	GeoCoord startGeoCoord, endGeoCoord;
	bool foundStart, foundEnd;

	foundStart = am.getGeoCoord(start, startGeoCoord);

	if (!foundStart) // If the starting attraction is not in the text file, return
	{
		cerr << "No geolocation found for " << start << endl;
		return NAV_BAD_SOURCE;
	}

	foundEnd = am.getGeoCoord(end, endGeoCoord);

	if (!foundEnd) // If the ending attraction is not in the text file, return
	{
		cerr << "No geolocation found for " << end << endl;
		return NAV_BAD_DESTINATION;
	}

	vector<StreetSegment> startingStreetSeg = sm.getSegments(startGeoCoord);
	StreetSegment firstStreet = startingStreetSeg[0];

	vector<StreetSegment> endingStreetSeg = sm.getSegments(endGeoCoord);
	StreetSegment lastStreet = endingStreetSeg[0];

	if (firstStreet.segment == lastStreet.segment) // If the starting and ending attractions are on the same street,
	{											   // find the path between them and return
		GeoSegment segment;
		segment.start = startGeoCoord;
		segment.end = endGeoCoord;
		NavSegment navSegment(getDirection(segment), firstStreet.streetName, distanceEarthMiles(segment.start, segment.end), segment);
		directions.clear();
		directions.insert(directions.begin(), navSegment);
		return NAV_SUCCESS;
	}

	priority_queue<Node*, vector<Node*>, CompareNode> geoCoordQueue;
	MyMap<GeoCoord, bool> navigatorMap;
	vector<Node*> usedNodes;
	Node node_path;
	
	Node *startingPoint = new Node; // Make a node that points to the starting position
	usedNodes.push_back(startingPoint); // Initialize its coordinates
	startingPoint->m_GeoCoord = startGeoCoord; // Initialize its GeoCoord
	startingPoint->traveledDistance = 0; // Set the traveled distance to 0
	startingPoint->remainingDistance = distanceEarthMiles(startingPoint->m_GeoCoord, endGeoCoord); // Find the remaining distance
	startingPoint->overallDistance = startingPoint->remainingDistance; // The overall distance will be the sum of the remaining distance
																	   // and the traveled distance
	startingPoint->m_parent = nullptr;   // Set its parent to null pointer
	geoCoordQueue.push(startingPoint);   // Push it onto the queue
									 
	bool done = false;
	
	while (!geoCoordQueue.empty() && done == false)
	{
		Node* newNode = geoCoordQueue.top(); // Make a new node which points to the top node of the queue
		geoCoordQueue.pop(); // Pop the top item
		if (navigatorMap.find(newNode->m_GeoCoord) == nullptr) // If the node is not in the map, add it to the map
		{ 
			navigatorMap.associate(newNode->m_GeoCoord, true);  // Put the node inside the map, and set the bool equal to true
			vector<StreetSegment> currentSegment(sm.getSegments(newNode->m_GeoCoord)); // Make a vector of all the segments that intersect
																					// with the GeoCoord of the new node
			for (int i = 0; i < currentSegment.size(); i++)
			{
				if (currentSegment[i].segment == lastStreet.segment) // If the current street is equal to the last street
				{
					done = true;	// Set the boolean equal to true
					node_path = *newNode;	// Set the back tracking path equal to the current Node
					break;	// Break out of the for loop
				}

				if (newNode->m_GeoCoord != currentSegment[i].segment.start) // If the coordinates of the new node is not equal to the 
																			// start of the current segment
				{
					Node* temp = new Node;	// Make a new temporary node
					usedNodes.push_back(temp); // Push it into the used nodes
					temp->m_GeoCoord = currentSegment[i].segment.start; // Set its GeoCoord to current segment's starting GeoCoord
					temp->m_parent = newNode; // Set its parent equal to the current node
					temp->traveledDistance = newNode->traveledDistance + distanceEarthMiles(newNode->m_GeoCoord, temp->m_GeoCoord);
						// Set the traveled distance equal to the current node's traveled distane + the distance between the temp and
						// current node
					temp->remainingDistance = distanceEarthMiles(temp->m_GeoCoord, endGeoCoord);
						// Set the remaining distance equal to the temp node's distance to the destination
					temp->overallDistance = temp->traveledDistance + temp->remainingDistance; // Add the remaining distance and the 
						//traveled distance
						

					if (navigatorMap.find(temp->m_GeoCoord) == nullptr)
						geoCoordQueue.push(temp); // Add the GeoCoord of the temp node to the map
				}

				if (newNode->m_GeoCoord != currentSegment[i].segment.end) // If the coordinates of the new node is not equal to the 
																		  // end of the current segment
				{
					Node* temp = new Node; // Make a new temporary node
					usedNodes.push_back(temp); // Push it into the used nodes
					temp->m_GeoCoord = currentSegment[i].segment.end; // Set its GeoCoord to current segment's starting GeoCoord
					temp->m_parent = newNode; // Set its parent equal to the current node
					temp->traveledDistance = newNode->traveledDistance + distanceEarthMiles(newNode->m_GeoCoord, temp->m_GeoCoord);
						// Set the traveled distance equal to the current node's traveled distane + the distance between the temp and
						// current node
					temp->remainingDistance = distanceEarthMiles(temp->m_GeoCoord, endGeoCoord); // Set the remaining distance equal to 
						//the temp node's distance to the destination
					temp->overallDistance = temp->traveledDistance + temp->remainingDistance; // Add the remaining distance and 
						//the traveled distance

					if (navigatorMap.find(temp->m_GeoCoord) == nullptr)
						geoCoordQueue.push(temp);	// Add the GeoCoord of the temp node to the map
				}
			}
		}
	}

	if (done == false) // If there was no path found, let the user no that there was no path between the two attractions
	{
		cerr << "No route was found between " << start << " and " << end << endl;
		return NAV_NO_ROUTE;
	}

	directions.clear(); // Empty the directions vector
	Node newPosition = node_path;

	GeoSegment destination;
	destination.start = newPosition.m_GeoCoord;
	destination.end = endGeoCoord;
	NavSegment endSeg(getDirection(destination), lastStreet.streetName, 
		distanceEarthMiles(destination.start, destination.end), destination);
	directions.insert(directions.begin(), endSeg); // Push the last segment into the vector

	while (newPosition.m_parent != nullptr)
	{
		GeoSegment segment;
		segment.start = newPosition.m_parent->m_GeoCoord; // Find the starting point of the segment
		segment.end = newPosition.m_GeoCoord;	// Find the ending point of the segment
		StreetSegment street = searchForTheStreet(segment);
		if (street.streetName != directions[0].m_streetName) // If the street changes, let the user know by letting them know to either
		{													 // make a right or a left	
			NavSegment navSegment(getDirectionAngle(segment, directions[0].m_geoSegment), directions[0].m_streetName);
			directions.insert(directions.begin(), navSegment); // Push the new NavSegment into the beginning of the vector
		}

		NavSegment navSegment(getDirection(segment), street.streetName, distanceEarthMiles(segment.start, segment.end), segment);
			// If you are still on the same street call NavSegment constructor
		directions.insert(directions.begin(), navSegment); // Push the new NavSegment into the beginning of the vector
		newPosition = *newPosition.m_parent;	// Move from current node to its parent
	}

	for (int i = 0; i < usedNodes.size(); i++) // Go through the vector and delete all the pointers
	{
		Node* temp;
		temp = usedNodes[i];
		if (temp != nullptr)
			delete temp;
	}
	return NAV_SUCCESS; // Let the user know that the path was found
}

StreetSegment NavigatorImpl::searchForTheStreet(GeoSegment segment) const
{
	vector<StreetSegment> beginning(sm.getSegments(segment.start));
	vector<StreetSegment> ending(sm.getSegments(segment.end));

	for (int i = 0; i < beginning.size(); i++)
	{
		for (int j = 0; j < ending.size(); j++)
		{
			if (beginning[i].segment == ending[j].segment) 
				return ending[j];
		}
	}
}

string NavigatorImpl::getDirection(GeoSegment currentSeg) const // Use this helper function to find the direction in which you have to move
{
	double angle = angleOfLine(currentSeg);
	if (0 <= angle && angle <= 22.5)
		return "east";
	if (22.5 < angle && angle <= 67.5)
		return "northeast";
	if (67.5 < angle && angle <= 112.5)
		return "north";
	if (112.5 < angle && angle <= 157.5)
		return "northwest";
	if (157.5 < angle && angle <= 202.5)
		return "west";
	if (202.5 < angle && angle <= 247.5)
		return "southwest";
	if (247.5 < angle && angle <= 292.5)
		return "south";
	if (292.5 < angle && angle <= 337.5)
		return "southeast";
	if (337.5 < angle && angle < 360)
		return "east";
}

string NavigatorImpl::getDirectionAngle(GeoSegment firstSeg, GeoSegment secondSeg) const //Use this helper function to find whether you have
{																						 // to make a right or left turn
	double angle = angleBetween2Lines(firstSeg, secondSeg);
	if (angle >= 180)
		return "right";
	else
		return "left";
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}
