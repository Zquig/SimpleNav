#pragma once

// STUDENTS: YOU MUST NOT MODIFY ANY CODE IN THIS FILE

#include <string>
#include "StreetSegment.h"
#include "MapLoaderImpl.h"


class MapLoader
{
public:
	MapLoader()
	{
		m_ml = new MapLoaderImpl;
	}
	~MapLoader()
	{
		delete m_ml;
	}

	bool load(const std::string &mapFile)
	{
		if (m_ml == nullptr)
			return false;

		return m_ml->load(mapFile);
	}

	size_t getNumSegments() const
	{
		if (m_ml == nullptr)
			return 0;

		return m_ml->getNumSegments();
	}

	bool getSegment(size_t segNum, StreetSegment &seg) const
	{
		if (m_ml == nullptr)
			return false;
		return m_ml->getSegment(segNum, seg);
	}
private:
	MapLoaderImpl *m_ml;
};