#pragma once

// Test driver for SegmentMapperImpl
// friend class of SegmentMapperImpl

#include "SegmentMapperImpl.h"
#include <iostream>

using namespace std;

class SegmentMapperImplTests
{
public:
	SegmentMapperImplTests()
	{}

	~SegmentMapperImplTests()
	{
		if (nullptr != m_mapLoader)
			delete m_mapLoader;
		if (nullptr != m_testerSMI)
			delete m_testerSMI;
	}

	bool initializeTest()
	{
		m_mapLoader = new MapLoader();

		m_testerSMI = new SegmentMapperImpl();
		return (m_mapLoader->load(DATA_FILENAME));
	}

	// Test init()
	bool should_mapGeoCoordToStreetSegment()
	{
		if (!m_testerSMI->init(*m_mapLoader))
		{
			cerr << "init() FAILED!\n\n";
			return false;
		}

		m_testerSMI->m_geoCoordAssociations.printBST_BreadthFirst();
		return true;
	}

	// Test lookupGeoCoord()
	bool should_findGeoCoordAndStoreStreetSegments()
	{
		if (!m_testerSMI->init(*m_mapLoader))
		{
			cerr << "init() FAILED!\n\n";
			return false;
		}

		for (;;)
		{
			cout << "Enter GeoCoord, latitude then longitude, separated by \'\,\' ('q' to quit): ";
			string userInput;
			getline(cin, userInput); // Assume proper longitude and latitude values entered
			cout << "Press Enter to continue";
			cin.ignore();

			if ("q" == userInput)
				break;

			string normalizedInput;
			string inputLatitude, inputLongitude;
			// remove spaces from userInput
			for (size_t i = 0; userInput[i]; i++)
			{
				if (' ' == userInput[i])
				{
					continue;
				}

				normalizedInput += userInput[i];
			}

			bool latitudeStored = false;
			for (size_t i = 0; normalizedInput[i]; i++)
			{
				if (!latitudeStored)
				{
					if (',' == normalizedInput[i])
					{
						latitudeStored = true;
						continue;
					}
					inputLatitude += normalizedInput[i];
					continue;
				}

				inputLongitude += normalizedInput[i];
			}

			cout << "\nSearching for " << inputLatitude << ", " << inputLongitude << "...\n\n";

			GeoCoord geoCoordToSearch(inputLatitude, inputLongitude);

			cout.precision(12);
			cout << "GeoCoord object:\n" << geoCoordToSearch.latitude << endl << geoCoordToSearch.longitude << endl;

			vector<StreetSegment> segments;

			if (!m_testerSMI->lookupGeoCoord(geoCoordToSearch, segments))
			{
				cout << "No associated segments\n\n";
				return false;
			}

			cout << "Associated segments:\n";

			for (auto vecIter = segments.begin(); vecIter != segments.end(); vecIter++)
			{
				cout << vecIter->streetName << endl;
			}
			cout << endl;
		}
		return true;
	}

private:
	MapLoader* m_mapLoader;
	SegmentMapperImpl* m_testerSMI;
	const string DATA_FILENAME = "mapdata.dat";
};