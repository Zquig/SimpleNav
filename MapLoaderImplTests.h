#pragma once

// Test driver for MapLoaderImpl. 
// Testing using inheritence

#include "MapLoaderImpl.h"
#include <iostream>
#include <random>
#include <time.h>

using namespace std;

class MapLoaderImplTests : public MapLoaderImpl
{
public:
	MapLoaderImplTests()
	{}

	~MapLoaderImplTests()
	{}

	bool should_LoadAndStoreMapData()
	{
		if (load(DATA_FILENAME))
		{
			// load successful, print each item in the vector
			for (size_t i = 0; i < m_streetSegContainer.size(); i++)
			{
				StreetSegment curSegment = *m_streetSegContainer[i];
				cout << i + 1 << ". ";
				m_printSegment(curSegment);
			}
			cout << endl;
			return true;
		}

		return false;
	}

	bool should_ReturnAccurateNumSegmentsLoaded()
	{
		if (load(DATA_FILENAME))
		{
			cout << "Number of segments loaded from mapdata.dat: " << getNumSegments() << endl;
		}
		else
		{
			cerr << "ERROR - Unable to load mapdata.dat" << endl;
			return false;
		}

		if (this->getNumSegments() == m_streetSegContainer.size())
		{
			cout << "numSegments accurate." << endl;
			return true;
		}
		else
		{
			cerr << "ERROR - Does not match" << endl;
			return false;
		}
	}

	bool should_GetSegmentAtIndexAndStoreSegment()
	{
		if (load(DATA_FILENAME))
		{
			cout << "mapdata.dat loaded" << endl;
		}
		else
		{
			cerr << "ERROR - Unable to load mapdata.dat" << endl;
			return false;
		}

		srand(time(NULL));
		while (true) // loop infinitely, picking random segments
		{
			size_t randomSegIndex = m_streetSegContainer.size() % rand();

			StreetSegment curSeg;


			cout << "Picking segment # " << randomSegIndex << "..." << endl;
			if (!getSegment(randomSegIndex, curSeg))
			{
				cerr << "ERROR - Unable to load segment # " << randomSegIndex << endl;
				return false;
			}
			m_printSegment(curSeg);
			system("pause");
			cout << endl;
		}
		return true;
	}

private:
	const string DATA_FILE_PATH = "C:\\Users\\camer\\documents\\visual studio 2015\\Projects\\Project4_BruinNav\\Project4_BruinNav\\mapdata.dat";
	const string DATA_FILENAME = "mapdata.dat";
	
private:
	void m_printSegment(StreetSegment &seg)
	{
		cout << seg.streetName << endl;
		cout << seg.segment.start.sLatitude
			<< ", " << seg.segment.start.sLongitude
			<< " -> " << seg.segment.end.sLatitude
			<< ", " << seg.segment.end.sLongitude
			<< endl;
		if (0 < seg.attractionsOnThisSegment.size())
		{
			vector<Address> attractions = seg.attractionsOnThisSegment;
			cout << "With " << attractions.size() << " attractions:" << endl;
			for (auto vecIt = attractions.begin(); vecIt != attractions.end(); vecIt++)
			{
				cout << "    * " << vecIt->attraction << "\n";
			}
		}
		else cerr << "No attractions" << endl;
		cout << endl;
	}
};