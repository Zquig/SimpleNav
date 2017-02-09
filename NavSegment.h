#pragma once

// STUDENTS: YOU MUST NOT MODIFY ANY CODE IN THIS FILE

// Contains a definition for a NavSegment class. 
// Your NavImpl’s navigate() method will return its routing directions as an ordered vector of these NavSegments. 
// Each NavSegment holds data on either 
// (a) one segment of the route (e.g., a street name, and the segment’s starting and ending geo-coordinates), 
// or (b) a turn instruction, detailing a turn that must be made between two segments in the route.

#include <string>

#include "GeoCoord.h"

class NavSegment
{
public:

	enum NAV_COMMAND { invalid, turn, proceed };

	NavSegment()
	{
		m_command = invalid;
	}
	
	void initTurn(const std::string &turnDirection, const std::string &streetName)
	{
		m_command = turn;
		m_streetName = streetName;
		m_direction = turnDirection;
		m_distance = 0;
	}

	void initProceed(const std::string &direction, const std::string &streetName, double distance, const GeoSegment &gs)
	{
		m_command = proceed;
		m_direction = direction;
		m_streetName = streetName;
		m_distance = distance;
		m_gs = gs;
	}

	NAV_COMMAND getCommandType() const
	{
		return m_command;
	}
	
	std::string getDirection() const
	{
		return m_direction;
	}

	std::string getStreet() const
	{
		return m_streetName;
	}

	double getDistance() const
	{
		return m_distance;
	}

	void setDistance(double dist)
	{
		m_distance = dist;
	}

	GeoSegment getSegment() const
	{
		return m_gs;
	}
	

private:
	NAV_COMMAND		m_command;	// turn left, turn right, proceed
	std::string		m_streetName;	// Westwood Blvd
	std::string		m_direction;	// "left" for turn or "northeast" for proceed
	double			m_distance;		// 3.2 //KM
	GeoSegment		m_gs;
};
