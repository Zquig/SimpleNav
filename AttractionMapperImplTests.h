#pragma once

// Test driver for AttractionMapperImpl
// Testing as a friend class

#include "AttractionMapperImpl.h"
#include <iostream>

using namespace std;

class AttractionMapperImplTests
{
public:
	AttractionMapperImplTests()
	{
	}

	~AttractionMapperImplTests()
	{
		if (nullptr != m_mapLoader)
			delete m_mapLoader;
		if (nullptr != m_attrMI)
			delete m_attrMI;
	}

	bool initializeTest()
	{
		m_mapLoader = new MapLoader();
		if (!m_mapLoader->load(DATA_FILENAME))
		{
			cerr << "Unable to load mapdata.dat\n\n";
			return false;
		}

		m_attrMI = new AttractionMapperImpl();
	}

	// Test init()
	bool should_mapEachAttractionToGeoCoord()
	{
		if (!m_attrMI->init(*m_mapLoader))
		{
			return false;
		}

		m_attrMI->m_attractions.printBST_BreadthFirst();
		return true;
	}

	// Test getGeoCoordForAttraction()
	bool should_findAttractionAndStoreItsGeoCoords()
	{
		if (!m_attrMI->init(*m_mapLoader))
		{
			return false;
		}

		if (0 == m_attrMI->m_attractions.size())
		{
			cerr << "m_attractions is empty\n\n";
			return false;
		}

		bool exit = false;
		while (!exit)
		{
			//const string attractionName = "Riviera CounTry club teNNis courts";
			string attractionName;
			
			cout << "Enter name of attraction: ('q' to quit)\n";
			getline(cin, attractionName);

			if ("q" == attractionName)
			{
				exit = true;
			}
			else
			{
				GeoCoord associatedGC;
				if (!m_attrMI->getGeoCoordForAttraction(attractionName, associatedGC))
				{
					cout << "Unable to find attraction (" << attractionName << ")\n\n";
					continue;
				}

				cout << "Attraction searched: " << attractionName << endl;
				cout << "GeoCoords: " << associatedGC.sLatitude << ", " << associatedGC.sLongitude << endl << endl;
			}
		}
		return true;
	}

private:
	const string DATA_FILENAME = "mapdata.dat";

private:
	MapLoader* m_mapLoader;
	AttractionMapperImpl* m_attrMI;
};