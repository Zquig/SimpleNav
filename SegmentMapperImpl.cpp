#include "SegmentMapperImpl.h"
#include <iostream>

using namespace std;

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
	for (size_t i = 0; i < m_associatedVectors.size(); i++)
	{
		delete m_associatedVectors[i];
	}
}

// Store mapping of GeoCoords to associated StreetSegments from MapLoader data
// O((N+A)*(log(N+A)))
bool SegmentMapperImpl::init(const MapLoader &ml)
{
	size_t totalSegments = ml.getNumSegments();

	if (0 == totalSegments)
		return false;

	cout << "Initializing street segments...\n";

	// For each SteetSegment in MapLoader's member variable:
	// vector<StreetSegment*> m_streetSegContainer
	// extract segment and store mapping of GeoCoord to that segment
	for (size_t i = 0; i < totalSegments; i++)
	{
		GeoCoord gcKey;
		StreetSegment curSegment;
		if (ml.getSegment(i, curSegment))
		{
			gcKey = curSegment.segment.start;
			initializeAssociation(gcKey, curSegment);

			gcKey = curSegment.segment.end;
			initializeAssociation(gcKey, curSegment);
		}

		vector<Address>* attractionsAtSegment = &curSegment.attractionsOnThisSegment;
		for (auto vecIter = attractionsAtSegment->begin(); vecIter != attractionsAtSegment->end(); vecIter++)
		{
			gcKey = vecIter->location;
			initializeAssociation(gcKey, curSegment);
		}
	}
	cout << "Initalized.\n";
	return true;
}

// The method must return true if one or more GeoSegments were found to be 
// associated with the input GeoCoord, 
// and it must fill in the segments parameter with all such associated segment(s).  
// If no segments were found to be associated with the specified GeoCoord, 
// then the method must return false and 
// the segments parameter may be empty or left unchanged from its input value. 
bool SegmentMapperImpl::lookupGeoCoord(const GeoCoord &geoCoord, vector<StreetSegment> &segments)
{
	vector<StreetSegment>* associatedSegments = m_geoCoordAssociations.find(geoCoord);
	
	if (nullptr == associatedSegments)
		return false; // segments left unchanged

	segments.clear();
	segments = *associatedSegments;
	return true;
}

void SegmentMapperImpl::initializeAssociation(const GeoCoord &geoCoord, const StreetSegment &seg)
{
	vector<StreetSegment>* associatedSegments = m_geoCoordAssociations.find(geoCoord);
	if (nullptr == associatedSegments)
	{
		// m_associatedVectors used to keep track of each allocated vector
		m_associatedVectors.push_back(new vector<StreetSegment>);
		associatedSegments = m_associatedVectors.back();
		associatedSegments->push_back(seg);
		m_geoCoordAssociations.associate(geoCoord, *associatedSegments);
	}
	else // m_geoCoordAssociations already contains the geoCoord key
	{
		// Insert association
		associatedSegments->push_back(seg);
	}
}