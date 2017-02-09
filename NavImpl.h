#pragma once

// STUDENTS ADD CODE HERE
#include <string>
#include <vector>
#include "NavSegment.h"
#include "NavResult.h"
#include "AttractionMapper.h"
#include "SegmentMapper.h"
#include "MapLoader.h"
#include <memory> // std::unique_ptr

class NavImpl
{
public:
	NavImpl();
	~NavImpl();
	bool loadMapData(const std::string &mapFile);
	NAV_RESULT navigate(const std::string &startLocation
		, const std::string &endLocation
		, std::vector<NavSegment> &directions);

private:
	friend class NavImplTests;
	NavImpl(const NavImpl &other);
	NavImpl &operator=(const NavImpl &other);
private:
	//std::string m_destinationName;
	//StreetSegment* m_destinationSeg;
	std::unique_ptr<AttractionMapper> m_attractionMapper;
	std::unique_ptr<SegmentMapper> m_segmentMapper;
	
	//AttractionMapper* m_attractionMapper;
	//SegmentMapper* m_segmentMapper;

private:
	class GeoLocation : public GeoCoord
	{
	public:
		GeoLocation();
		GeoLocation(const GeoCoord& gc);
		~GeoLocation();
		void updateVector(const std::shared_ptr<GeoLocation>& previous, const GeoCoord& destination, StreetSegment* associatedSeg);
		std::weak_ptr<GeoLocation> getPreviousWaypoint() const;
		GeoCoord getCurrentCoords() const;
		std::shared_ptr<StreetSegment> getAssociatedSegment() const;
		double getDistFromStart() const;
		double getWeight() const;

	private:
		std::unique_ptr<GeoCoord> m_endCoords;
		std::weak_ptr<GeoLocation> m_previous; // holds previous waypoint in path
		std::shared_ptr<StreetSegment> m_streetSegPtr;
		double m_weight; // distance from starting location + estimated distance to destination
		double m_distFromStart;
	private:
		void setDistFromStart();
		void relaxWeight();
	};

private:
	std::queue<std::shared_ptr<GeoLocation>> m_potentialPaths;
	//std::vector<std::shared_ptr<GeoLocation>> m_potentialPaths;
	std::shared_ptr<GeoLocation> m_start;
	std::shared_ptr<GeoLocation> m_destination;
	//GeoLocation* m_start;
	//GeoLocation* m_destination;

private:
	//std::vector<GeoLocation*> determineShortestPath(std::vector<GeoLocation*> currentPath, std::queue<std::unique_ptr<GeoLocation>>& potentialPaths);
	std::vector<std::shared_ptr<GeoLocation>> determineShortestPath(std::vector<std::shared_ptr<GeoLocation>>& currentPath);
	bool locationVisited(const GeoCoord& gL, const std::vector<std::shared_ptr<GeoLocation>>& visitedLocations) const;
	void setPossibleRoute(std::shared_ptr<GeoLocation>& location);
	std::string angleToDirection(double angleInDegrees);
	std::string angleToTurnDir(double angleOfTurn);

// ATTEMPT : implementation with MyMap of previous waypoints <- incomplete
	
//private:
//	class GeoLocation : public GeoCoord
//	{
//	public:
//		GeoLocation();
//		GeoLocation(const GeoCoord& gc);
//		~GeoLocation();
//		bool relaxWeight(GeoLocation* previous, const GeoCoord& destination);
//		void findAdjacentLocations(SegmentMapper& segM);
//		std::vector<unique_ptr<GeoLocation>> getAdjenctLocations() const;
//		GeoCoord getGeoCoord() const;
//		double getWeight() const;
//		double getDistanceTraveled() const;
//
//	private:
//		std::vector<unique_ptr<GeoLocation>> m_adjacencyList;
//		double m_weight; // units = km
//		double m_distanceTraveled; // units = km
//	};
//
//private:
//	std::queue<GeoLocation*> m_potentialPaths;
//	MyMap<GeoCoord, GeoCoord*> m_previousWaypointMap;
//	unique_ptr<GeoCoord> m_start;
//	unique_ptr<GeoCoord> m_destination;
//
//private:
//	GeoCoord* determineShortestPath(GeoLocation& curLocation, MyMap<GeoCoord, GeoCoord*>& visited);
};