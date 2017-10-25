#include "provided.h"
#include "MyMap.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap<GeoCoord, vector<StreetSegment>> m_segmentMapper; // Make a map which sotres the GeoCoords of each segment and returns all the
														    // street segments that intersect at a given GeoCoord
	void initHelper(const GeoCoord& geo, const StreetSegment& street); // Use this helper function to initialize the segment mapper init
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	size_t numOfSegs = ml.getNumSegments();	// Find the number of segments in the given text file
	for (size_t i = 0; i < numOfSegs; i++)
	{
		StreetSegment tempSegment;
		if (ml.getSegment(i, tempSegment)) // If there exists a segment, store it inside tempSegment and go inside the if statement
		{
			initHelper(tempSegment.segment.start, tempSegment); // Call the helper function initHelper and pass in a GeoCoord and street seg
			initHelper(tempSegment.segment.end, tempSegment);
			if (tempSegment.attractions.size() != 0) // If the segment has any attractions go inside the if statement
			{
				for (int j = 0; j < tempSegment.attractions.size(); j++) // Go through all the attractions
				{
					initHelper(tempSegment.attractions[j].geocoordinates, tempSegment);
				}
			}
		}
	}
}

void SegmentMapperImpl::initHelper(const GeoCoord& gc, const StreetSegment& street)
{
	if (m_segmentMapper.find(gc) != nullptr) // If the given GeoCoord (gc) is in the map, add the new street segment to the vector 
	{										 // that holds its other street segments
		m_segmentMapper.find(gc)->push_back(street);
	}
	else // If the given GeoCoord (gc) is not in the map yet, make a new vector, push the new street segment into the vector and add the
	{    // GeoCoord and the vector into the map
		vector<StreetSegment> newStreetSegment;
		newStreetSegment.push_back(street);
		m_segmentMapper.associate(gc, newStreetSegment);
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;
	const vector<StreetSegment>* segmentPtr = m_segmentMapper.find(gc); // Search for the given GeoCoord inside the map										
	if (segmentPtr != nullptr)	// if the GeoCoord is inside the map set segments vector equal to the streetsegment vector 
	{
		segments = *segmentPtr;
	}
	return segments;	// return the segments vector
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
