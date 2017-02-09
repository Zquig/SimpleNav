#pragma once

// STUDENTS: YOU MUST NOT MODIFY ANY CODE IN THIS FILE

// Contains a definition of the StreetSegment struct, 
// which can be used to hold details on a street segment loaded by your MapLoaderImpl class, 
// including the name of the street segment, its starting/ending geo-coordinates, and a vector of attractions found on that segment.

#include <vector>
#include <string>
#include "GeoCoord.h"

struct Address
{
	std::string attraction;
	GeoCoord	location;
};

struct StreetSegment
{
	// students to implement this function
	bool operator==(const StreetSegment &other)
	{
		return segment.start == other.segment.start && segment.end == other.segment.end;
	}

	bool operator<(const StreetSegment &other)
	{
		if (segment.start < other.segment.start)
			return true;
		
		if (segment.start > other.segment.start)
			return false;

		return segment.end < other.segment.end;
	}

	std::string				streetName;
	GeoSegment				segment;
	std::vector<Address>	attractionsOnThisSegment;
};