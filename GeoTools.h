#pragma once

// STUDENTS: YOU MUST NOT MODIFY ANY CODE IN THIS FILE

// Contains a function to compute the angle in degrees between two GeoSegments (i.e., street segments), 
// and a function to compute the distance in miles between two GeoCoords (i.e., lat-long coordinates).

#include "GeoCoord.h"

// Haversine formula: http://stackoverflow.com/questions/10198985/calculating-the-distance-between-2-latitudes-and-longitudes-that-are-saved-in-a

#include <cmath> 
#define pi 3.14159265358979323846
#define earthRadiusKm 6371.0

// This function converts decimal degrees to radians
inline double deg2rad(double deg) {
	return (deg * pi / 180);
}

//  This function converts radians to decimal degrees
inline double rad2deg(double rad) {
	return (rad * 180 / pi);
}

/**
* Returns the distance between two points on the Earth.
* Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
* @param lat1d Latitude of the first point in degrees
* @param lon1d Longitude of the first point in degrees
* @param lat2d Latitude of the second point in degrees
* @param lon2d Longitude of the second point in degrees
* @return The distance between the two points in kilometers
*/
inline double distanceEarthKM(const GeoCoord &g1, const GeoCoord &g2) {
	double lat1r, lon1r, lat2r, lon2r, u, v;
	lat1r = deg2rad(g1.latitude);
	lon1r = deg2rad(g1.longitude);
	lat2r = deg2rad(g2.latitude);
	lon2r = deg2rad(g2.longitude);
	u = sin((lat2r - lat1r) / 2);
	v = sin((lon2r - lon1r) / 2);
	return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

inline double distanceEarthMiles(const GeoCoord &g1, const GeoCoord &g2) {
	const double milesPerKm = 0.621371;
	return distanceEarthKM(g1, g2) * milesPerKm;
}


inline double angleBetween2Lines(const GeoSegment &line1, const GeoSegment &line2)
{
	/*
	double angle1 = atan2(line1.start.latitude - line1.end.latitude, line1.start.longitude - line1.end.longitude);
	double angle2 = atan2(line2.start.latitude - line2.end.latitude, line2.start.longitude - line2.end.longitude);
	*/
	double angle1 = atan2(line1.end.latitude - line1.start.latitude, line1.end.longitude - line1.start.longitude);
	double angle2 = atan2(line2.end.latitude - line2.start.latitude, line2.end.longitude - line2.start.longitude);

	double result = (angle2 - angle1) * 180 / 3.14;
	if (result < 0)
		result += 360;

	return result;
}


inline double angleOfLine(const GeoSegment &line1)
{
	double angle = atan2(line1.end.latitude - line1.start.latitude, line1.end.longitude - line1.start.longitude);
	double result = angle * 180 / 3.14;
	if (result < 0)
		result += 360;	

	return result;
}