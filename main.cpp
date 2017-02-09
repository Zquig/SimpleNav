#define DEFAULT
//#define TESTER

#if defined(DEFAULT)
#include <stdio.h>
#include <vector>
#include <map>
#include "NavSegment.h"
#include "Nav.h"
#include "mode.h"
#include <iostream>

std::string getMajorityDir(const std::map<std::string, double> &dirstToAggDist)
{
	std::map<std::string, int> dirToCount;


	double biggestDist = 0;
	std::string bestDir;
	for (auto dc : dirstToAggDist)
		if (dc.second > biggestDist)
		{
			biggestDist = dc.second;
			bestDir = dc.first;
		}

	return bestDir;
}


void printDirections(const std::string &start, const std::string &dest, std::vector<NavSegment> &ns)
{
	double aggDist = 0, totalDist = 0;
	std::string lastStreet;
	std::map<std::string, double> dirsToAggDist;

	printf("You are starting at: %s\n", start.c_str());
	for (auto n : ns)
	{
		if (n.getCommandType() == NavSegment::turn)
		{
			if (aggDist > 0)
			{
				printf("Proceed %.2lf miles %s on %s\n", aggDist, getMajorityDir(dirsToAggDist).c_str(), lastStreet.c_str());
				aggDist = 0;
				lastStreet = "";
				dirsToAggDist.clear();
			}
			printf("Take a %s turn on %s\n", n.getDirection().c_str(), n.getStreet().c_str());
		}
		else
		{
			aggDist += n.getDistance();
			totalDist += n.getDistance();
			lastStreet = n.getStreet();
			dirsToAggDist[n.getDirection()] += n.getDistance();
		}
	}

	if (aggDist > 0)
		printf("Proceed %.2lf miles %s on %s\n", aggDist, getMajorityDir(dirsToAggDist).c_str(), lastStreet.c_str());

	printf("You have reached your destination: %s\n", dest.c_str());
	printf("Total travel distance: %.1lf miles\n", totalDist);
}

void printDirectionsRaw(const std::string &start, const std::string &dest, std::vector<NavSegment> &ns)
{
	printf("%s\n", start.c_str());
	printf("%s\n", dest.c_str());
	for (auto n : ns)
	{
		if (n.getCommandType() == NavSegment::proceed)
		{
			printf("%s, %s %s, %s %s\n",
				n.getSegment().start.sLatitude.c_str(), n.getSegment().start.sLongitude.c_str(),
				n.getSegment().end.sLatitude.c_str(), n.getSegment().end.sLongitude.c_str(),n.getDirection().c_str());
		}
		else
		{
			printf("#%s\n", n.getDirection().c_str());
		}
	}
}

int main(int argc, char *argv[])
{
	NavImpl nav;

	if (argc != 4 && argc != 5)
	{
		printf("usage: BruinNav mapdata.dat \"start location name\" \"end location name\"\n");
		printf("usage: BruinNav mapdata.dat \"start location name\" \"end location name\" -raw\n");
		system("pause");
		return -1;
	}
	
	bool raw = false; 

	if (argc == 5 && _stricmp(argv[4], "-raw") == 0)
		raw = true;

	if (!nav.loadMapData(argv[1]))
	{
		printf("Unable to load map data: %s\n",argv[1]);
		system("pause");
		return -1;
	}

	std::string start = argv[2];
	std::string dest = argv[3];

	if (raw)
	{
		std::vector<NavSegment> ns;
		std::string error;
		NAV_RESULT result = nav.navigate(start, dest, ns);
		printf("%d\n", result);
		if (result == success)
			printDirectionsRaw(start, dest, ns);
		system("pause");
		return 0;
	}
	else
	{
		printf("\nRouting...\n\n");

		std::vector<NavSegment> ns;
		std::string error;
		NAV_RESULT result = nav.navigate(start,dest, ns);
		switch (result)
		{
			case success:
				printDirections(start, dest, ns);
				printf("\n");
				break;
			case no_route:
				printf("No route found between %s and %s\n", start, dest);
				break;
			case invalid_source:
				printf("Attrction %s not found\n", start);
				break;
			case invalid_destination:
				printf("Attrction %s not found\n", dest);
				break;
		}
	}

	system("pause");
	return 0;
}
#endif // DEFAULT

using namespace std;

#if defined(TESTER)
//#define TEST_MYMAP
//#define TEST_MAPLOADERIMPL
//#define TEST_ATTRACTIONMAPPERIMPL
//#define TEST_SEGMENTMAPPERIMPL
#define TEST_NAVIMPL

#if defined(TEST_MYMAP)
#include "Test_MyMap.h"
using namespace TestMyMap;
#endif // TEST_MYMAP

#if defined(TEST_MAPLOADERIMPL)
#include "MapLoaderImplTests.h"

namespace MapLoaderImplTest
{
	void runTest()
	{
		MapLoaderImplTests testObject;

		/*if (!testObject.should_LoadAndStoreMapData())
		{
			cout << "Failed MapLoaderImpl.load()" << endl << endl;
		}*/

		if (!testObject.should_ReturnAccurateNumSegmentsLoaded())
		{
			cout << "Failed MapLoaderImpl.getNumSegments()" << endl << endl;
		}

		if (!testObject.should_GetSegmentAtIndexAndStoreSegment())
		{
			cout << "Failed MapLoaderImpl.getSegment()" << "\n\n";
		}
	}
}

using namespace MapLoaderImplTest;
#endif // TEST_MAPLOADERIMPL

#if defined(TEST_ATTRACTIONMAPPERIMPL)
#include "AttractionMapperImplTests.h"

namespace AttractionMapperImplTest
{
	void runTest()
	{
		AttractionMapperImplTests testAMI;
		testAMI.initializeTest();
		/*if (testAMI.should_mapEachAttractionToGeoCoord())
			cout << "init() SUCCESS!\n\n";
		else cout << "init() FAILED...\n\n";*/

		if (testAMI.should_findAttractionAndStoreItsGeoCoords())
			cout << "getGeoCoordForAttraction() SUCCESS!\n\n";
		else cout << "getGeoCoordForAttraction() FAILED...\n\n";
	}
}

using namespace AttractionMapperImplTest;
#endif // TEST_ATTRACTIONMAPPERIMPL

#if defined(TEST_SEGMENTMAPPERIMPL)
#include "SegmentMapperImplTests.h"

namespace SegmentMapperImplTest
{
	void runTest()
	{
		SegmentMapperImplTests tester;
		tester.initializeTest();
		if (!tester.should_mapGeoCoordToStreetSegment())
		{
			cerr << "init() FAILED...\n\n";
		}
		else cerr << "init() WORKS!\n\n";

		/*if (!tester.should_findGeoCoordAndStoreStreetSegments())
		{
			cerr << "lookupGeoCoord() FAILED...\n\n";
		}
		else cerr << "lookupGeoCoord() WORKS!\n\n";*/
	}
}

using namespace SegmentMapperImplTest;
#endif // TEST_SEGMENTMAPPERIMPL

#if defined (TEST_NAVIMPL)
#include "NavImplTests.h"

namespace NavImplTest
{
	void runTest()
	{
		NavImplTests tester;
		if (!tester.initializeTest())
		{
			cerr << "Test intialization failed!\n\n";
			return;
		}

		tester.should_printProperNavigationSteps();
	}
}

using namespace NavImplTest;
#endif // TEST_NAVIMPL

#include<iostream>

int main()
{
	runTest();
	system("pause");
	return 0;
}

#endif // TESTER