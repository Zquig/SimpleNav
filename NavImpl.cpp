// CREATED: Saturday, January 14, 2017

// FUNCTION:
// Compute the most efficient route from a source attraction to
// a destination attraction, if one exists.
// Utilizes AttractionMapper, SegmentMapper, and MapLoader

#include "NavImpl.h"
#include "GeoTools.h"
#include <iostream>
#include <stack>

using namespace std;

// CONSTRUCTOR:
NavImpl::NavImpl()
	//: m_destinationSeg(nullptr)
{
	m_attractionMapper = make_unique<AttractionMapper>();
	m_segmentMapper = make_unique<SegmentMapper>();

	//m_attractionMapper = new AttractionMapper;
	//m_segmentMapper = new SegmentMapper;
}

// DESTRUCTOR:
NavImpl::~NavImpl()
{
	//delete m_attractionMapper; // Handled by unique_ptr deleter
	//delete m_segmentMapper; // Handled by unique_ptr deleter
}

// FUNCTION:
// load all data
// initialize all internal data structures, AttractionMapper, SegmentMapper
// RETURNS:
// true: if successful
// false: if failed
bool NavImpl::loadMapData(const string &mapFile)
{
	MapLoader mapData;
	if (!mapData.load(mapFile))
	{
		cerr << "Unable to load " << mapFile << "\n\n";
		return false;
	}
	if (!m_attractionMapper->init(mapData))
	{
		cerr << "Unable to initialize AttractionMapper\n\n";
		return false;
	}
	if (!m_segmentMapper->init(mapData))
	{
		cerr << "Unable to initialize SegmentMapper\n\n";
		return false;
	}
	return true;
}

// FUNCTION:
// takes start and end locations and finds returns vector containing
// step-by-step directions using shortest path
// RETURNS:
// success : A path was found from the source to the destination.
// invalid_source : The source attraction or street address that was passed in was not found in our data file, and therefore the system can’t route from it.
// invalid_destination : The destination attraction or street address that was passed in was not found in our data file, and therefore the system can’t route to it.
// no_route : No route was found linking the source to the destination address.
NAV_RESULT NavImpl::navigate(const string &startLocation
	, const string &endLocation
	, vector<NavSegment> &directions)
{
	m_start.reset();
	m_destination.reset();
	while (!m_potentialPaths.empty())
	{
		m_potentialPaths.pop();
	}
	//m_potentialPaths.clear();

	GeoCoord startGC, endGC;

	/*for (size_t i = 0; i < endLocation.length(); i++)
	{
		m_destinationName += tolower(endLocation[i]);
	}*/

	// ATTEMPT : implementation with MyMap of previous waypoints
	//m_start = unique_ptr<GeoCoord>(new GeoCoord());
	//m_destination = unique_ptr<GeoCoord>(new GeoCoord());
	// END_ATTEMPT

	// Get GeoCoord of starting attraction
	if (!m_attractionMapper->getGeoCoordForAttraction(startLocation, startGC))
	{
		return NAV_RESULT::invalid_source;
	}
	// Assuming each attraction is only associated with one StreetSegment
	/*if (1 < associatedSegments.size())
	{
		cerr << "BUG : attraction is associated with multiple street segments. Check attraction or revise code\n\n";
		return NAV_RESULT::invalid_impl;
	}*/

	// Get GeoCoord of ending attraction
	if (!m_attractionMapper->getGeoCoordForAttraction(endLocation, endGC))
	{
		return NAV_RESULT::invalid_destination;
	}

	// ATTEMPT : implementation with MyMap of previous waypoints
	//GeoCoord* shortestPathEnd = determineShortestPath(GeoLocation(*m_start), m_previousWaypointMap);
	
	// If shortestPathEnd == destination, find it in m_previousWaypointMap
	// Then work way back to find path
	// else unable to find path
	// END_ATTEMPT

	m_start = make_shared<GeoLocation>(startGC);
	m_destination = make_shared<GeoLocation>(endGC);

	shared_ptr<GeoLocation> emptyPtr;

	vector<StreetSegment> beginnings;
	if (!m_segmentMapper->lookupGeoCoord(startGC, beginnings))
	{
		cerr << "BUG : Unable to find starting location in any StreetSegment\n\n";
		return NAV_RESULT::invalid_source;
	}
	if (1 == beginnings.size())
	{
		m_start->updateVector(emptyPtr, endGC, &(*(beginnings.begin())));
	}
	else m_start->updateVector(emptyPtr, endGC, nullptr);

	vector<StreetSegment> destiny;
	if (!m_segmentMapper->lookupGeoCoord(endGC, destiny)) // Find all vectors containing destination GeoCoordinates
	{
		cerr << "BUG : Unable to find destination in any StreetSegment\n\n";
		return NAV_RESULT::invalid_destination;
	}
	if (1 == destiny.size())
	{
		// Store StreetSegment containing destination for quick check
		//m_destinationSeg = &(*destiny)[0];
		m_destination->updateVector(emptyPtr, endGC, &(*destiny.begin()));
	}
	else m_destination->updateVector(emptyPtr, endGC, nullptr);
	//else m_destinationSeg = nullptr;

	vector<shared_ptr<GeoLocation>> shortestPath;
	shortestPath.push_back(m_start);
	//shortestPath = determineShortestPath(shortestPath, m_potentialPaths);
	shortestPath = determineShortestPath(shortestPath);

	if (0 == shortestPath.size()) // This should not happen...
	{
		return NAV_RESULT::invalid_impl;
	}

	if (m_destination->getCurrentCoords() != (shortestPath.back())->getCurrentCoords())
	{
		// Deadend was reached in path-finding algorithm
		return NAV_RESULT::no_route;
	}

	stack<shared_ptr<GeoLocation>> shortestPathForward;
	shared_ptr<GeoLocation> locationInPath = shortestPath.back(); // Store destination
	for (auto vecIter = shortestPath.rbegin(); vecIter != shortestPath.rend(); vecIter++)
	{
		if (nullptr == locationInPath)
		{
			break;
		}

		shared_ptr<GeoLocation> locationInLoop = *vecIter;
		if (locationInPath->getCurrentCoords() == locationInLoop->getCurrentCoords())
		{
			// If location matches the previous waypoint of the last
			// location in the path, then push the location onto the
			// stack. Store the next previous waypoint to find.
			shortestPathForward.push(locationInLoop);
			try
			{
				locationInPath = locationInLoop->getPreviousWaypoint().lock(); // weak_ptr -> shared_ptr
			}
			catch (bad_weak_ptr&)
			{
				cerr << "Unable to get previous waypoint from GeoLocation\n\n";
				return NAV_RESULT::invalid_impl;
			}
		}
	}

	cout << "\nDiscovered.\n\n";

	shared_ptr<GeoLocation> previousLocation;
	shared_ptr<GeoLocation> currentLocation;
	weak_ptr<StreetSegment> previousSegment;
	weak_ptr<StreetSegment> currentSegment;
	double distTraveledOnStreet = 0;
	double totalDistTraveled = 0;
	while (!shortestPathForward.empty())
	{
		currentLocation = shortestPathForward.top();
		shortestPathForward.pop();
		currentSegment = currentLocation->getAssociatedSegment();
		if (previousSegment.lock())
		{
			GeoSegment nextTravSeg(previousLocation->getCurrentCoords(), currentLocation->getCurrentCoords());

			if (previousSegment.lock()->streetName != currentSegment.lock()->streetName)
			{
				// different connecting segments representing a turn
				GeoSegment prevTravSeg;
				if (!directions.empty() && NavSegment::NAV_COMMAND::proceed == directions.back().getCommandType())
				{
					prevTravSeg = directions.back().getSegment();
				}
				else
				{
					cerr << "BUG : No starting proceed-style NavSegment\n";
					return NAV_RESULT::invalid_impl;
				}
				string turnDir = angleToTurnDir(angleBetween2Lines(prevTravSeg, nextTravSeg));
				string streetName = currentSegment.lock()->streetName;
				NavSegment turnNav;
				turnNav.initTurn(turnDir, streetName);
				directions.push_back(turnNav);
				distTraveledOnStreet += distanceEarthMiles(previousLocation->getCurrentCoords(), currentLocation->getCurrentCoords());

				if (!shortestPathForward.empty() && shortestPathForward.top()->getAssociatedSegment()->streetName == streetName)
				{
					previousSegment = currentSegment;
					previousLocation = currentLocation;
					continue;
				}
			}

			//GeoSegment travelSeg(previousLocation->getCurrentCoords(), currentLocation->getCurrentCoords());
			string travelDir = angleToDirection(angleOfLine(nextTravSeg));
			string streetName = currentSegment.lock()->streetName;
			distTraveledOnStreet += distanceEarthMiles(previousLocation->getCurrentCoords(), currentLocation->getCurrentCoords());
			if (shortestPathForward.empty() || previousSegment.lock()->streetName != shortestPathForward.top()->getAssociatedSegment()->streetName)
			{
				NavSegment proceedNav;
				proceedNav.initProceed(travelDir, streetName, distTraveledOnStreet, nextTravSeg);
				directions.push_back(proceedNav);

				totalDistTraveled += distTraveledOnStreet;
				distTraveledOnStreet = 0;
			}
		}
		previousSegment = currentSegment;
		previousLocation = currentLocation;
	}

	// TEST
	//for (auto vecIter = directions.begin(); vecIter != directions.end(); vecIter++)
	//{
	//	cout << vecIter->getStreet() << " " << vecIter->getDirection() << " " << vecIter->getDistance() << " miles" << endl;
	//}
	//cout << "Total distance traveled: " << totalDistTraveled << " miles\n\n";
	//system("pause");
	// END_TEST

	// TEST
	//while (!shortestPathForward.empty())
	//{
	//	//cerr << shortestPathForward.top()->getCurrentCoords().sLatitude << ", "
	//		//<< shortestPathForward.top()->getCurrentCoords().sLongitude << endl;
	//	cerr << shortestPathForward.top()->getAssociatedSegment()->streetName << endl;
	//	cerr << "Distance traveled: " << shortestPathForward.top()->getDistFromStart() << " kilometers" << endl;
	//	shortestPathForward.pop();
	//}
	//system("pause");
	// END_TEST

	return NAV_RESULT::success;
}

//______ GeoLocation Implementation ________
// GeoLocation is an extension of GeoCoord (using inheritance)
// It serves to determine the shortest path by acting as a vector
// in the mapping of the path-search algorithm
NavImpl::GeoLocation::GeoLocation()
	: GeoCoord(), m_weight(-1), m_distFromStart(-1)
{}

NavImpl::GeoLocation::GeoLocation(const GeoCoord& gc)
	: GeoCoord(gc.sLatitude, gc.sLongitude), m_weight(-1), m_distFromStart(-1)
{}

NavImpl::GeoLocation::~GeoLocation()
{}

// GeoLocation is adjacent to current location
// Add to queue of potential paths
// Update variables to determine shortest path
void NavImpl::GeoLocation::updateVector(const shared_ptr<GeoLocation>& previous, const GeoCoord& destination, StreetSegment* associatedSeg)
{
	m_endCoords.reset();
	m_previous.reset();

	if (previous)
	{
		m_previous = previous;
	}
	m_endCoords = make_unique<GeoCoord>(destination);
	
	// Q: when using StreetSegment* m_streetSegPtr
	// Why can't I just do
	// m_streetSegPtr = associatedSeg;
	// Why do I need to create a new StreetSegment object and copy the associatedSeg?
	// I'm unable to point to the already existing object.. why?

	if (nullptr != associatedSeg)
	{
		m_streetSegPtr = make_shared<StreetSegment>(*associatedSeg);
	}

	setDistFromStart();
	relaxWeight();
}

weak_ptr<NavImpl::GeoLocation> NavImpl::GeoLocation::getPreviousWaypoint() const
{
	return m_previous;
}

GeoCoord NavImpl::GeoLocation::getCurrentCoords() const
{
	const string sLat = this->sLatitude;
	const string sLong = this->sLongitude;
	return GeoCoord(sLat, sLong);
}

shared_ptr<StreetSegment> NavImpl::GeoLocation::getAssociatedSegment() const
{
	return m_streetSegPtr;
}

double NavImpl::GeoLocation::getDistFromStart() const
{
	return m_distFromStart;
}

double NavImpl::GeoLocation::getWeight() const
{
	return m_weight;
}

void NavImpl::GeoLocation::setDistFromStart()
{
	shared_ptr<GeoLocation> prevLoc = m_previous.lock();
	if (!prevLoc) // Initializing a vector independent of others
	{
		m_distFromStart = 0;
		return;
	}
	// Add distance from previous waypoint to current waypoint to 
	// determine total distance traveled
	m_distFromStart = prevLoc->getDistFromStart() + distanceEarthMiles(prevLoc->getCurrentCoords(), getCurrentCoords());
}

void NavImpl::GeoLocation::relaxWeight()
{
	double curWeight = m_distFromStart + distanceEarthMiles(getCurrentCoords(), *m_endCoords);

	if (-1 == m_weight || curWeight < m_weight)
	{
		m_weight = curWeight;
	}
	//cerr << "Weight calculated: " << m_weight << endl;
}

//vector<NavImpl::GeoLocation*> NavImpl::determineShortestPath(vector<NavImpl::GeoLocation*> visited, queue<unique_ptr<NavImpl::GeoLocation>>& potentialPaths)
vector<shared_ptr<NavImpl::GeoLocation>> NavImpl::determineShortestPath(vector<shared_ptr<NavImpl::GeoLocation>> &visited)
{
	// PSEUDO-PSEUDOCODE: (general idea...)
	// -Find possible paths from current location
	// -Push into m_potentialPaths
	// --If path == destination
	// ---return
	// --As each location is pushed into the queue, relaxWeight()
	// -current_location = queue.front(); queue.pop();
	// -While (queue.size() is not reached)
	// --if (queue.front().getWeight() < current_location.getWeight())
	// ---queue.push(current_location)
	// ---current_location = queue.front(); queue.pop();
	// Recurse()
	
	// TEST
	//static unsigned int recursiveCalls = 0;
	//cerr << "Number of recursions = " << recursiveCalls++ << endl;
	// END_TEST

	if (0 == visited.size()) // No starting point passed
	{
		return visited;
	}

	cout << "Searching for shortest path...";

	shared_ptr<GeoLocation> curLocation = visited.back();

	// DEPRECIATED : Used in recursive call of function
	//if (curLocation->getCurrentCoords() == m_destination->getCurrentCoords()) // Destination reached!
	//{
	//	return visited;
	//}
	// END_DEPRECIATED ~CF February 02, 2017

	//unsigned int loops = 0;
	while (curLocation->getCurrentCoords() != m_destination->getCurrentCoords())
	{	
		cout << ".";
		//cerr << loops++ << endl;
		// Find adjacent coordinates
		vector<StreetSegment> associatedSegs;
		m_segmentMapper->lookupGeoCoord(curLocation->getCurrentCoords(), associatedSegs);

		for (auto segIter = associatedSegs.begin(); segIter != associatedSegs.end(); segIter++)
		{
			// Discover if destination is in this segment
			if (nullptr != m_destination->getAssociatedSegment())
			{
				if (segIter->segment == m_destination->getAssociatedSegment()->segment)
				{
					m_destination->updateVector(curLocation, m_destination->getCurrentCoords(), &(*segIter));
					visited.push_back(m_destination);
					return visited; // Destination found
				}
			}
			else
			{
				vector<Address>* attractions = &segIter->attractionsOnThisSegment;
				for (auto attrIter = attractions->begin(); attrIter != attractions->end(); attrIter++)
				{
					//if (m_destinationName == attrIter->attraction)
					if (m_destination->getCurrentCoords() == attrIter->location)
					{
						m_destination->updateVector(curLocation, m_destination->getCurrentCoords(), &(*segIter));
						visited.push_back(m_destination);
						return visited; // Destination found
					}
				}
			}
			// ToDo: Refactor above block of code to match 
			// StreetSegments first, then attractions. Less checks will
			// be required
			// January 30, 2017 : Actually, not always true. Attraction may be part of multiple
			// StreetSegments, resulting in checking vector<StreetSegment> with each iteration. This
			// will also result in extra space required to store the vector
			// Here is the code anyway...
			//for (auto vecIter = m_destinationSeg.begin(); vecIter != m_destinationSeg.end(); vecIter)
			//{
			//	if (*segIter == *vecIter)
			//	{
			//		m_destination->updateVector(curLocation, m_destination->getCurrentCoords());
			//		visited.push_back(m_destination.get());
			//		return visited; // Destination found
			//	}
			//}
			// I've decided to merge the codes so the algorithm will pick the quicker check

			//unique_ptr<GeoLocation> adjLocation;
			shared_ptr<GeoLocation> adjLocation;
			// Determine all possible paths from the current location
			if (segIter->segment.start != curLocation->getCurrentCoords())
			{
				if (!locationVisited(segIter->segment.start, visited))
				{
					adjLocation = make_shared<GeoLocation>(segIter->segment.start);
					adjLocation->updateVector(curLocation, m_destination->getCurrentCoords(), &(*segIter));
					//m_potentialPaths.push(adjLocation);
					setPossibleRoute(adjLocation);

					/*m_potentialPaths.push(make_shared<GeoLocation>(segIter->segment.start));
					adjLocation = m_potentialPaths.back();
					adjLocation->updateVector(curLocation, m_destination->getCurrentCoords());*/

					// The following may be producing error C2280 : attempting to reference a deleted function
					//adjLocation = unique_ptr<GeoLocation>(new GeoLocation(segIter->segment.start));
					//adjLocation->updateVector(curLocation, *m_destination);
					//unique_ptr<GeoLocation> queueThis(move(adjLocation));
					//potentialPaths.push(queueThis);
				}
			}
			if (segIter->segment.end != curLocation->getCurrentCoords())
			{
				if (!locationVisited(segIter->segment.end, visited))
				{
					adjLocation = make_shared<GeoLocation>(segIter->segment.end);
					adjLocation->updateVector(curLocation, m_destination->getCurrentCoords(), &(*segIter));
					//m_potentialPaths.push(adjLocation);
					setPossibleRoute(adjLocation);

					/*m_potentialPaths.push(make_shared<GeoLocation>(segIter->segment.end));
					adjLocation = m_potentialPaths.back();
					adjLocation->updateVector(curLocation, m_destination->getCurrentCoords());*/

					// The following may be producing error C2280 : attempting to reference a deleted function
					//adjLocation = unique_ptr<GeoLocation>(new GeoLocation(segIter->segment.end));
					//adjLocation->updateVector(curLocation, *m_destination);
					//unique_ptr<GeoLocation> queueThis(move(adjLocation));
					//potentialPaths.push(queueThis);
				}
			}
		}

		// Take first path in queue, and compare it against
		// every other paths in the queue. 
		// Choose path with lowest weight
		if (0 < m_potentialPaths.size())
		{
			// The following may be producing error C2280 : attempting to reference a deleted function
			// Could it be that the deleter was moved to pathToConsider, so when pop()
			// was called, no deleter could be called from the enqueued unique_ptr?
			//unique_ptr<GeoLocation> pathToConsider(move(m_potentialPaths.front()));
			//m_potentialPaths.pop();

			shared_ptr<GeoLocation> pathToConsider;
			//pathToConsider = m_potentialPaths[0];
			pathToConsider = m_potentialPaths.front();
			m_potentialPaths.pop();

			//size_t indexOfLowestWeight = 0;
			for (size_t i = 0; i < m_potentialPaths.size(); i++)
			{
				// iterate through queue of possible paths to find
				// path with lowest weight
				//unique_ptr<GeoLocation> pathInQueue(move(m_potentialPaths.front()));
				//m_potentialPaths.pop();
				/*unique_ptr<GeoLocation> pathInQueue(new GeoLocation());
				pathInQueue.swap(m_potentialPaths.front());
				m_potentialPaths.pop();*/

				shared_ptr<GeoLocation> pathInQueue;
				pathInQueue = m_potentialPaths.front();
				m_potentialPaths.pop();

				if (pathToConsider->getWeight() > pathInQueue->getWeight())
				{
					// GeoLocation in queue has a lower weight,
					// swap GeoLocations
					pathToConsider.swap(pathInQueue);
				}
				m_potentialPaths.push(pathInQueue); // Return path into queue
			}
			// ToDo: Above block can be refactored by first sorting by weight
			// then push each path into the queue.

				/*if (pathToConsider->getWeight() > m_potentialPaths[i]->getWeight())
				{
					pathToConsider = m_potentialPaths[i];
					indexOfLowestWeight = i;
				}
			}
			m_potentialPaths.erase(m_potentialPaths.begin()+indexOfLowestWeight);*/

			//cerr << "Number of paths available: " << m_potentialPaths.size() << endl;
			//cerr << "Path visited: " << pathToConsider->getCurrentCoords().sLatitude
				//<< ", " << pathToConsider->getCurrentCoords().sLongitude << endl;
			//cerr << "Weight: " << pathToConsider->getWeight() << endl;
			//cerr << "Distance traveled: " << pathToConsider->getDistFromStart() << " km\n";

			visited.push_back(pathToConsider);
			//return determineShortestPath(visited, potentialPaths);
			//return determineShortestPath(visited); // STACK OVERLOAD!!!!!!
			curLocation = pathToConsider;
		}
	}
	return visited; // curLocation == m_destination
	//return visited; // 0 == potentialPaths.size() // Deadend reached
}

bool NavImpl::locationVisited(const GeoCoord& gL, const vector<shared_ptr<NavImpl::GeoLocation>>& visitedLocations) const
{
	for (auto vecIter = visitedLocations.begin(); vecIter != visitedLocations.end(); vecIter++)
	{
		shared_ptr<GeoLocation> curGL = *vecIter;
		if (curGL->getCurrentCoords() == gL)
		{
			return true;
		}
	}
	return false;
}

void NavImpl::setPossibleRoute(shared_ptr<NavImpl::GeoLocation>& location)
{
	shared_ptr<GeoLocation> curLocation = location;
	// Find if location is currently in the queue of possible paths.
	// If it is, compare weights, use the shorter path.
	for (size_t i = 0; i < m_potentialPaths.size(); i++)
	{
		shared_ptr<GeoLocation> locationInQueue = m_potentialPaths.front();
		m_potentialPaths.pop();
		//shared_ptr<GeoLocation> locationInQueue = m_potentialPaths[i];

		if (curLocation->getCurrentCoords() == locationInQueue->getCurrentCoords())
		{
			if (curLocation->getWeight() < locationInQueue->getWeight())
			{
				curLocation.swap(locationInQueue);
			}
			m_potentialPaths.push(locationInQueue);
			return;
		}
		else
		{
			m_potentialPaths.push(locationInQueue);
		}
	}
	m_potentialPaths.push(curLocation);
	//m_potentialPaths.push_back(curLocation);
}

string NavImpl::angleToDirection(double angleInDegrees)
{
	if (0 > angleInDegrees || 360 < angleInDegrees)
	{
		return "Invalid angle, unable to obtain direction";
	}

	if (0 <= angleInDegrees && 22.5 >= angleInDegrees)
	{
		return "east";
	}
	else if (22.5 < angleInDegrees && 67.5 >= angleInDegrees)
	{
		return "northeast";
	}
	else if (67.5 < angleInDegrees && 112.5 >= angleInDegrees)
	{
		return "north";
	}
	else if (112.5 < angleInDegrees && 157.5 >= angleInDegrees)
	{
		return "northwest";
	}
	else if (157.5 < angleInDegrees && 202.5 >= angleInDegrees)
	{
		return "west";
	}
	else if (202.5 < angleInDegrees && 247.5 >= angleInDegrees)
	{
		return "southwest";
	}
	else if (247.5 < angleInDegrees && 292.5 >= angleInDegrees)
	{
		return "south";
	}
	else if (292.5 < angleInDegrees && 337.5 >= angleInDegrees)
	{
		return "southeast";
	}
	else if (337.5 < angleInDegrees && 360 > angleInDegrees)
	{
		return "east";
	}

	return "Invalid angle, unable to obtain direction";
}

string NavImpl::angleToTurnDir(double angleOfTurn)
{
	if (0 > angleOfTurn || 360 < angleOfTurn)
	{
		return "Invalid angle, unable to obtain turn direction";
	}

	if (0 == angleOfTurn || 360 == angleOfTurn)
	{
		return "straight";
	}
	if (0 < angleOfTurn && 22.5 >= angleOfTurn)
	{
		return "slight left";
	}
	else if (22.5 < angleOfTurn && 90 >= angleOfTurn)
	{
		return "left";
	}
	else if (90 < angleOfTurn && 180 > angleOfTurn)
	{
		return "sharp left";
	}
	else if (180 == angleOfTurn)
	{
		return "around";
	}
	else if (180 < angleOfTurn && 270 > angleOfTurn)
	{
		return "sharp right";
	}
	else if (270 <= angleOfTurn && 337.5 > angleOfTurn)
	{
		return "right";
	}
	else if (337.5 <= angleOfTurn && 360 > angleOfTurn)
	{
		return "slight right";
	}
	return "Invalid angle, unable to obtain turn direction";
}

// ATTEMPT : implementation with MyMap of previous waypoints <- incomplete

//// GeoLocation is being considered in the shortest path,
//// relax the weight of the GeoLocation if applicable to
//// the current path
//bool NavImpl::GeoLocation::relaxWeight(GeoLocation* previous, const GeoCoord& destination)
//{
//	double distTrav, wt;
//
//	if (nullptr != previous)
//	{
//		distTrav = previous->getDistanceTraveled() + distanceEarthKM(previous->getGeoCoord(), getGeoCoord());
//	}
//
//	wt = distTrav + distanceEarthKM(getGeoCoord(), destination);
//
//	if (0 == m_weight || wt < m_weight)
//	{
//		m_distanceTraveled = distTrav;
//		m_weight = wt;
//		return true;
//	}
//	return false;
//}
//
//void NavImpl::GeoLocation::findAdjacentLocations(SegmentMapper& segM)
//{
//	GeoCoord gc = this->getGeoCoord();
//	
//	vector<StreetSegment> segVec;
//	segM.lookupGeoCoord(gc, segVec);
//
//	for (auto vecIter = segVec.begin(); vecIter != segVec.end(); vecIter++)
//	{
//		if (vecIter->segment.start != gc)
//		{
//			m_adjacencyList.push_back(unique_ptr<GeoLocation>(new GeoLocation(vecIter->segment.start)));
//		}
//		if (vecIter->segment.end != gc)
//		{
//			m_adjacencyList.push_back(unique_ptr<GeoLocation>(new GeoLocation(vecIter->segment.end)));
//		}
//	}
//}
//
//vector<unique_ptr<NavImpl::GeoLocation>> NavImpl::GeoLocation::getAdjenctLocations() const
//{
//	return m_adjacencyList;
//}
//
//GeoCoord NavImpl::GeoLocation::getGeoCoord() const
//{
//	const string sLat = this->sLatitude;
//	const string sLong = this->sLongitude;
//
//	return GeoCoord(sLat, sLong);
//}
//
//double NavImpl::GeoLocation::getWeight() const
//{
//	return m_weight;
//}
//
//double NavImpl::GeoLocation::getDistanceTraveled() const
//{
//	return m_distanceTraveled;
//}
//
//GeoCoord* NavImpl::determineShortestPath(GeoLocation& curLocation, MyMap<GeoCoord, GeoCoord*>& visited)
//{
//	
//}