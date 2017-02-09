#pragma once

// DEFINITION:
// Define inline comparison operators
#include "GeoCoordProvided.h"

//____GeoCoord______
// Copy constructors
// ERROR : C2600 - Must be declared in class
//inline GeoCoord& GeoCoord::operator=(const GeoCoord &other)
//{
//	sLatitude = other.sLatitude;
//	sLongitude = other.sLongitude;
//	latitude = other.latitude;
//	longitude = other.longitude;
//}
//
//inline GeoCoord::GeoCoord(const GeoCoord &other)
//{
//	sLatitude = other.sLatitude;
//	sLongitude = other.sLongitude;
//	latitude = other.latitude;
//	longitude = other.longitude;
//}

// Comparison operators
inline bool operator==(const GeoCoord &first, const GeoCoord &second)
{
	if (first.latitude == second.latitude && first.longitude == second.longitude)
		return true;
	return false;
}

inline bool operator!=(const GeoCoord& first, const GeoCoord& second)
{
	if (first.latitude == second.latitude && first.longitude == second.longitude)
		return false;
	return true;
}

inline bool operator<(const GeoCoord &first, const GeoCoord &second)
{
	if (first.latitude == second.latitude)
		return (first.longitude < second.longitude);

	return (first.latitude < second.latitude);
}

inline bool operator>(const GeoCoord &first, const GeoCoord &second)
{
	if (first.latitude == second.latitude)
		return (first.longitude > second.longitude);

	return (first.latitude > second.longitude);
}

//______GeoSegment______
// Copy constructors
// ERROR : C2600 - Must be declared in class
//inline GeoSegment& GeoSegment::operator=(const GeoSegment&other)
//{
//	start = other.start;
//	end = other.end;
//}
//
//inline GeoSegment::GeoSegment(const GeoSegment&other)
//{
//	start = other.start;
//	end = other.end;
//}

// Comparison operators
inline bool operator==(const GeoSegment &first, const GeoSegment &second)
{
	if (first.start == second.start && first.end == second.end)
		return true;
	return false;
}

inline bool operator!=(const GeoSegment &first, const GeoSegment &second)
{
	if (first.start == second.start && first.end == second.end)
		return false;
	return true;
}

inline bool operator<(const GeoSegment &first, const GeoSegment &second)
{
	if (first.start == second.start)
	{
		if (first.end < second.end)
			return true;
		else return false;
	}

	if (first.start < second.start)
		return true;
	return false;
}

inline bool operator>(const GeoSegment &first, const GeoSegment &second)
{
	if (first.start == second.start)
	{
		if (first.end > second.end)
			return true;
		else return false;
	}

	if (first.start > second.start)
		return true;
	return false;
}