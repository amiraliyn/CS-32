#include "support.h"

bool operator<(const GeoCoord& firstGeoCoord, const GeoCoord& secondGeoCoord) // Overload < in order to compare GeoCoords
{
	return ((firstGeoCoord.latitudeText < secondGeoCoord.latitudeText) || ((firstGeoCoord.latitudeText == secondGeoCoord.latitudeText)
		&& (firstGeoCoord.longitudeText < secondGeoCoord.longitudeText)));
}

bool operator>(const GeoCoord& firstGeoCoord, const GeoCoord& secondGeoCoord) // Overload < in order to compare GeoCoords
{
	return ((firstGeoCoord.latitudeText > secondGeoCoord.latitudeText) || ((firstGeoCoord.latitudeText == secondGeoCoord.latitudeText) 
		&& (firstGeoCoord.longitudeText > secondGeoCoord.longitudeText)));
}

bool operator==(const GeoCoord& firstGeoCoord, const GeoCoord& secondGeoCoord) // Overload == in order to compare GeoCoords
{
	return ((firstGeoCoord.latitudeText == secondGeoCoord.latitudeText) && (firstGeoCoord.longitudeText == secondGeoCoord.longitudeText));
}

bool operator!=(const GeoCoord& firstGeoCoord, const GeoCoord& secondGeoCoord) // Overload != in order to compare GeoCoords
{
	return ((firstGeoCoord.latitudeText != secondGeoCoord.latitudeText) || (firstGeoCoord.longitudeText != secondGeoCoord.longitudeText));
}

bool operator<(const Node& firstNode, const Node& secondNode) // Overload < in order to compare Nodes (trick the operator to think < is >)
{
	return firstNode.overallDistance > secondNode.overallDistance;
}

bool operator>(const Node& firstNode, const Node& secondNode) // Overload < in order to compare Nodes (trick the operator to think < is >)
{
	return firstNode.overallDistance > secondNode.overallDistance;
}

bool operator==(const GeoSegment& firstGeoSegment, const GeoSegment& secondGeoSegment) // Overload == in order to compare GeoSegments
{
	return ((firstGeoSegment.start == secondGeoSegment.start) && (firstGeoSegment.end == secondGeoSegment.end)) || 
		((firstGeoSegment.start == secondGeoSegment.end) && (firstGeoSegment.end == secondGeoSegment.start));
}

bool operator!=(const GeoSegment& firstGeoSegment, const GeoSegment& secondGeoSegment) // Overload != in order to compare GeoSegments
{
	if (firstGeoSegment == secondGeoSegment)
		return false;
	return true;
}