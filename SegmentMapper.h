#pragma once

// STUDENTS: YOU MUST NOT MODIFY ANY CODE IN THIS FILE

#include <string>
#include "StreetSegment.h"
#include "MapLoader.h"
#include "SegmentMapperImpl.h"

class SegmentMapper
{
public:
	SegmentMapper()
	{
		m_impl = new SegmentMapperImpl;
	}

	~SegmentMapper()
	{
		delete m_impl; 
	}

	bool init(const MapLoader &ml)
	{
		if (m_impl == nullptr)
			return false;

		m_impl->init(ml);
		return true;
	}

	bool lookupGeoCoord(const GeoCoord &geoCoord, std::vector<StreetSegment> &segments)
	{
		if (m_impl == nullptr)
			return false;

		return m_impl->lookupGeoCoord(geoCoord,segments);
	}

private:
	SegmentMapperImpl *m_impl;
};