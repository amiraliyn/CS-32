#include "provided.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> m_streetSegmentData; // Make a vector which will hold all the information of the text file
	int m_numOfSegs;
};

MapLoaderImpl::MapLoaderImpl()
	:m_numOfSegs(0)
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream mapData(mapFile);  // Load the map
	if (!mapData)		        // Did opening the file fail?
	{
		cerr << "Error: Cannot open data.txt!" << endl;
		mapData.close();
		return false;			// If it failed, return false
	}

	int numOfPlaces = 0;	   
	string currLine = "";

	while (getline(mapData, currLine)) // Get the current line and store it in currLine; if there are no lines left, leave the loop
	{
		StreetSegment newStreetSegment;	// Make a new segment
		newStreetSegment.streetName = currLine; // Set the name of the street in the segment equal to currLine
		string coords[4];	// Make an array of strings of size 4 to store the starting and ending coordinates of the street
		getline(mapData, currLine, ',');
		coords[0] = currLine;
		getline(mapData, currLine, ' ');
		if(currLine == "")
			getline(mapData, currLine, ' ');
		coords[1] = currLine;
		getline(mapData, currLine, ',');
		coords[2] = currLine;
		getline(mapData, currLine, '\n');
		coords[3] = currLine;
		getline(mapData, currLine, '\n');
		numOfPlaces = stoi(currLine);
		for (int i = 0; i < numOfPlaces; i++) // If the stree has any attractions, go through each and save them in the vector
		{
			Attraction currentAttraction;
			getline(mapData, currLine, '|');
			currentAttraction.name = currLine;
			string attractionCoords[2];
			getline(mapData, currLine, ',');
			attractionCoords[0] = currLine;
			char space;
			mapData.get(space);
			getline(mapData, currLine);
			if(space == ' ')
				attractionCoords[1] = currLine;
			else
				attractionCoords[1] = space + currLine;
			GeoCoord attractionCoord(attractionCoords[0], attractionCoords[1]);
			currentAttraction.geocoordinates = attractionCoord;
			newStreetSegment.attractions.push_back(currentAttraction);
		}
		GeoCoord startCoord(coords[0], coords[1]);
		GeoCoord endCoord(coords[2], coords[3]);
		GeoSegment m_geo(startCoord, endCoord);
		newStreetSegment.segment = m_geo;
		m_streetSegmentData.push_back(newStreetSegment); // Push the completed street segment into the vector
		m_numOfSegs++;	// Increment the number of segments
	}
	mapData.close();
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_numOfSegs; // return the number of segments 
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < getNumSegments()) // as long as segNum is less than getNumSegments set seg equal to m_streetSegmentData[segNum]							  
	{                              // and return true
		seg = m_streetSegmentData[segNum];
		return true;
	}
	return false;				   // return false otherwise
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
	return m_impl->getSegment(segNum, seg);
}