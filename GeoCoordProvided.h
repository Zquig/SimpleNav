#pragma once

// STUDENTS: YOU MUST NOT MODIFY ANY CODE IN THIS FILE
// FYI: get lat,long coordinates for addresses at: http://www.latlong.net/

// Contains a GeoCoord struct that you can use to hold a particular latitude/longitude, 
// and a GeoSegment struct that defines a segment (comprised of starting and ending GeoCoords).

#include<string>

struct GeoCoord
{
	GeoCoord(const std::string &lat="0", const std::string &lon="0")
	{
		latitude = atof(lat.c_str());
		longitude = atof(lon.c_str());

		sLatitude = lat;
		sLongitude = lon;
	}

	double latitude;
	double longitude;
	std::string sLatitude;
	std::string sLongitude;
};

struct GeoSegment
{
	GeoSegment(const GeoCoord &s, const GeoCoord &e)
		: start(s), end(e)
	{
	}

	GeoSegment()
	{

	}

	GeoCoord start;
	GeoCoord end;
};