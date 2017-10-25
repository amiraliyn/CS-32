#include "provided.h"
#include "MyMap.h"
#include <string>
#include <iostream>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> m_myMap; // Make a map using the MyMap header file to store the GeoCoords of each attraction
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	size_t numOfSegs = ml.getNumSegments(); // Find the number of segments in the given text file
	for (size_t i = 0; i < numOfSegs; i++)
	{
		StreetSegment tempSegment;
		if (ml.getSegment(i, tempSegment)) // If there exists a segment, store it inside tempSegment and go inside the if statement
		{
			if (tempSegment.attractions.size() != 0) // If the segment has any attractions go inside the if statement
			{
				for (int j = 0; j < tempSegment.attractions.size(); j++) // Go through all the attractions
				{
					for (int k = 0; k < tempSegment.attractions[j].name.length(); k++) // change the attraction's name to lower case
					{
						tempSegment.attractions[j].name[k] = tolower(tempSegment.attractions[j].name[k]);
					}
					m_myMap.associate(tempSegment.attractions[j].name, tempSegment.attractions[j].geocoordinates); // add the attraction's
				}																		// name and GeoCoord to the map
			}
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	for (int i = 0; i < attraction.length(); i++) // change the given string to all lower case
	{
		attraction[i] = tolower(attraction[i]);
	}
	const GeoCoord* m_attractionGeoCoord = m_myMap.find(attraction); // Search for the given attraction's name inside the map
	if (m_attractionGeoCoord != nullptr)  // if the name is inside the map set gc equal to the coordinates of the attraction
	{									  // and return true
		gc = *m_attractionGeoCoord;
		return true;
	}
	return false;						  // Otherwise return false
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
