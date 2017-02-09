#pragma once

#include "MapLoader.h"
#include "MyMap.h"
#include <vector>

// STUDENTS ADD CODE HERE

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	bool init(const MapLoader &ml);
	bool getGeoCoordForAttraction(const std::string &attraction, GeoCoord &gc);
private:
	MyMap<std::string, GeoCoord> m_attractions;
private:
	friend class AttractionMapperImplTests;
	AttractionMapperImpl(const AttractionMapperImpl &other);
	AttractionMapperImpl &operator=(const AttractionMapperImpl &other);
};