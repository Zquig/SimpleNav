#pragma once

#include "MapLoader.h"
#include "MyMap.h"
#include <vector>

// STUDENTS ADD CODE HERE

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	bool init(const MapLoader &ml);
	bool lookupGeoCoord(const GeoCoord &geoCoord, std::vector<StreetSegment> &segments);

private:
	MyMap<GeoCoord, std::vector<StreetSegment>> m_geoCoordAssociations;
	std::vector<std::vector<StreetSegment>*> m_associatedVectors;

private:
	void initializeAssociation(const GeoCoord &geoCood, const StreetSegment &seg);

private:
	friend class SegmentMapperImplTests;
	SegmentMapperImpl(const SegmentMapperImpl &other);
	SegmentMapperImpl &operator=(const SegmentMapperImpl &other);
};