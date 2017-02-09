#pragma once

// STUDENTS: YOU MUST NOT MODIFY ANY CODE IN THIS FILE

#include <string>
#include <vector>

#include "NavSegment.h"
#include "NavImpl.h"
#include "NavResult.h"

class Nav
{
public:

	Nav()
	{
		m_ni = new NavImpl;
	}
	~Nav()
	{
		delete m_ni;
	}

	bool loadMapData(const std::string &mapFile)
	{
		if (m_ni == nullptr)
			return false;

		return m_ni->loadMapData(mapFile);
	}

	NAV_RESULT navigate(const std::string &startLocation, const std::string &endLocation, std::vector<NavSegment> &directions)
	{
		if (m_ni == nullptr)
			return invalid_impl;

		return m_ni->navigate(startLocation, endLocation, directions);
	}

private:
	NavImpl				*m_ni;
};