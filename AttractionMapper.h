#pragma once

// STUDENTS: YOU MUST NOT MODIFY ANY CODE IN THIS FILE

#include <string>

#include "MapLoader.h"
#include "AttractionMapperImpl.h"
#include "GeoCoord.h"

#include <vector>

class AttractionMapper
{
public:
	AttractionMapper()
	{
		m_am = new AttractionMapperImpl;
	}

	~AttractionMapper()
	{
		delete m_am;
	}

	bool init(const MapLoader &ml)
	{
		if (m_am == nullptr)
			return false;

		return m_am->init(ml);
	}

	bool getGeoCoordForAttraction(const std::string &attraction, GeoCoord &gc)
	{
		if (m_am == nullptr)
			return false;

		return m_am->getGeoCoordForAttraction(attraction, gc);
	}

private:
	AttractionMapperImpl *m_am;
};