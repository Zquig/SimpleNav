#include "AttractionMapperImpl.h"
#include <iostream>

// Implements AttractionMapper class
// Takes loaded MapLoader data and stores it in a data structure
// to allow looking up associated street segments with a specified
// geocoordinate

using namespace std;

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

// If there are N total street segments in the input mapping data, 
// and A total attractions dispersed throughout the streets, 
// then your init() method must run in O(N + A*log(A)) time, 
// and your getGeoCoordForAttraction() must run in O(log(A)) time.
// You may assume that the map data being loaded is randomly ordered.
bool AttractionMapperImpl::init(const MapLoader &ml)
{
	// Q: Is O(N + A*log(A)) really possible? I can think of only O(N*A+A(log(A))
	// d/t for N StreetSegments in the container, we extract each attraction,
	// then sort the attractions
	// Are we assuming A << N?
	// A: YES

	if (0 == ml.getNumSegments())
		return false;

	m_attractions.clear();

	cout << "Initializing attractions...\n";

	size_t numStreetSegments = ml.getNumSegments();
	StreetSegment ssPtr;

	// For each street segment, extract attractions and place into m_attractions
	for (size_t i = 0; i < numStreetSegments; i++)
	{
		if (ml.getSegment(i, ssPtr))
		{
			size_t totalAttr = ssPtr.attractionsOnThisSegment.size();
			if (0 < totalAttr)
			{
				vector<Address>::iterator vecIter = ssPtr.attractionsOnThisSegment.begin();
				for (size_t numAttr = 0; numAttr < totalAttr; numAttr++)
				{
					string attrName = (vecIter + numAttr)->attraction;
					GeoCoord coords = (vecIter + numAttr)->location;
					m_attractions.associate(attrName, coords);
				}	
				// DONE: sort m_attractions into efficient data structure to be able to easily search
				// attractions by identifier and return the appropriate GeoCoord
			}
		}
	}
	cout << "Initialized.\n";
	return true;
}

bool AttractionMapperImpl::getGeoCoordForAttraction(const std::string &attraction, GeoCoord &gc)
{
	// Case insensitive
	string normalizedAttrArgu;
	for (size_t i = 0; i < attraction.length(); i++)
	{
		normalizedAttrArgu += tolower(attraction[i]);
	}
	//cerr << "getGeoCoordForAttraction(" << normalizedAttrArgu << ")\n";

	GeoCoord* gcPtr = m_attractions.find(normalizedAttrArgu);
	if (nullptr != gcPtr)
	{
		gc = *gcPtr;
		return true;
	}
	return false;
}