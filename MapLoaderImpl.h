#pragma once

// STUDENTS ADD CODE HERE

#include "StreetSegment.h"
#include <vector>
#include <string>

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(const std::string &mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment &seg) const;

protected:
	std::vector<StreetSegment*> m_streetSegContainer; // ToDo: figure out a more efficient container
	size_t m_numSegments;
	const enum dataType {
		SEGMENT_NAME,
		GEOCOORD,
		NUM_ATTRACTIONS,
		ATTRACTION
	};

private:
	void storeData(const std::string &line, const dataType typeOfData);
	void extractGeoCoords(const std::string &lineFromFile, GeoCoord &start, GeoCoord &end);
	void extractAttraction(const std::string &lineFromFile, std::string &attraction, GeoCoord &coords);
	std::string convertToLower(const std::string &str);

private:
	MapLoaderImpl(const MapLoaderImpl &other);
	MapLoaderImpl &operator=(const MapLoaderImpl &other);
};