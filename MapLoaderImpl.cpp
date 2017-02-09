#include "MapLoaderImpl.h"
#include <fstream>
#include <iostream>
#include "GeoCoord.h"

using namespace std;

MapLoaderImpl::MapLoaderImpl()
	: m_numSegments(0)
{
}

MapLoaderImpl::~MapLoaderImpl()
{
	for (size_t i = 0; i < m_streetSegContainer.size(); i++)
	{
		delete m_streetSegContainer[i];
	}
}

// Load mapdata.dat, mapFile is the filename
// Assume mapdata.dat is correctly formatted
bool MapLoaderImpl::load(const string &mapFile)
{
	ifstream mapDataStream(mapFile, ifstream::in);

	if (!mapDataStream.is_open())
	{
		cerr << "Unable to open file with filename " << mapFile << endl;
		return false;
	}

	cout << "Loading mapdata.dat...\n";
	// Each segment is formatted as follows:
	// Line 0: "Name of segment" (SEGMENT_NAME)
	// Line 1: "startLat, startLong endLat,endLong" (GEOCOORD)
	// Line 2: "Number of attractions" (NUM_ATTRACTIONS)
	// Line 3+: "Address or name of attaction|lat, long" (ATTRACTION)
	
	// Store each line of the data file into declared container of Street Segments
	dataType typeOfData = SEGMENT_NAME;
	while (mapDataStream.good())
	{
		string line;
		getline(mapDataStream, line);
		if ("" == line)
			continue;

		storeData(line, typeOfData);
		
		switch (typeOfData)
		{
		case SEGMENT_NAME:
		{
			typeOfData = GEOCOORD; // Line 2
			break;
		}
		case GEOCOORD:
		{
			typeOfData = NUM_ATTRACTIONS;
			break;
		}
		case NUM_ATTRACTIONS:
		{
			int numAttractions = stoi(line); // Converts string of number of attractions to int
			//int numAttractions = atoi(line.c_str()); // Using Carey's method in GeoCoordProvided.h
			// Q: What's the difference in the two above lines?
			// http://stackoverflow.com/questions/20583945/what-is-the-difference-between-stdatoi-and-stdstoi
			// A: atoi converts c-strings and is more prone to error. Errors, such as non-integer chars will return 0.
			// A: stoi converts strings	

			typeOfData = ATTRACTION;
			while (numAttractions > 0)
			{
				getline(mapDataStream, line);
				storeData(line, typeOfData);
				numAttractions--;
			}

			m_numSegments++; // increment total number of COMPLETED segments stored
			typeOfData = SEGMENT_NAME;
			break;
		}
		case ATTRACTION: // Case is handled above
		{
			break;
		}
		}
	}

	mapDataStream.close(); // Close file
	cout << "Map data loaded.\n";
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_numSegments;
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (0 > segNum || m_numSegments <= segNum)
		return false;
	if (nullptr == m_streetSegContainer[segNum])
		return false;
	
	StreetSegment* segPtr = m_streetSegContainer[segNum];
	seg = *segPtr;
	return true;
}

void MapLoaderImpl::storeData(const string &line, const dataType typeOfData)
{
	switch (typeOfData)
	{
	case SEGMENT_NAME:
	{
		m_streetSegContainer.push_back(new StreetSegment);

		StreetSegment* curStreetSegment = m_streetSegContainer.back();
		curStreetSegment->streetName = convertToLower(line);
		
		return;
	}
	case GEOCOORD:
	{
		GeoCoord startCoord;
		GeoCoord endCoord;
		extractGeoCoords(line, startCoord, endCoord);
		GeoSegment curGeoSegment = GeoSegment(startCoord, endCoord);
		StreetSegment* curStreetSegment = m_streetSegContainer.back(); // Get current street segment being modified
		curStreetSegment->segment = curGeoSegment; // Store start and end coordinates
		return;
	}
	case NUM_ATTRACTIONS:
	{
		// Nothing to store
		return;
	}
	case ATTRACTION:
	{
		StreetSegment* curStreetSegment = m_streetSegContainer.back();
		vector<Address>* curAttractionList = &(curStreetSegment->attractionsOnThisSegment);
		
		Address curAttraction = Address();
		string attractionName;
		GeoCoord attractionCoord;
		extractAttraction(line, attractionName, attractionCoord);
		curAttraction.attraction = attractionName;
		curAttraction.location = attractionCoord;

		curAttractionList->push_back(curAttraction);

		return;
	}
	}
}

void MapLoaderImpl::extractGeoCoords(const string &lineFromFile, GeoCoord &start, GeoCoord &end)
{
	// Assume proper formatting:
	// "startLat, startLong endLat,endLong"
	string startLat, startLong, endLat, endLong;
	int index = 0;

	while (lineFromFile[index] != ',')
	{
		startLat += lineFromFile[index++];
	} // index at first ','
	index = index + 2; // skip to the beginning of the start longitude
	while (lineFromFile[index] != ' ')
	{
		startLong += lineFromFile[index++];
	} // index at ' ' between start coordinates and end coordinates
	index++; // go to beginning of endLat
	while (lineFromFile[index] != ',')
	{
		endLat += lineFromFile[index++];
	} // index at ',' between endLat and endLong
	index++; // go to beginning of endLong
	while (lineFromFile[index])
	{
		endLong += lineFromFile[index++];
	}

	start = GeoCoord(startLat, startLong);
	end = GeoCoord(endLat, endLong);

	//int section = 0;
	//for (; lineFromFile[index]; index++)
	//{
	//	if (0 == section)
	//	{
	//		if (lineFromFile[index] == ',')
	//		{
	//			section++;
	//			index++;
	//			continue;
	//		}
	//		startLat += lineFromFile[index];
	//	}
	//	else if (1 == section)
	//	{
	//		if (lineFromFile[index] == ' ')
	//		{
	//			section++;
	//			continue;
	//		}
	//		startLong += lineFromFile[index];
	//	}
	//	else if (2 == section)
	//	{
	//		if (lineFromFile[index] == ',')
	//		{
	//			section++;
	//			continue;
	//		}
	//		endLat += lineFromFile[index];
	//	}
	//	else if (3 == section)
	//	{
	//		endLong += lineFromFile[index];
	//	}
	//}
}

void MapLoaderImpl::extractAttraction(const string &lineFromFile, string &attraction, GeoCoord &coords)
{
	// Assume proper formatting:
	// "Address or name of attaction|lat, long"
	int index = 0;
	attraction.clear();
	string latitude, longitude;
	while (lineFromFile[index] != '|')
	{
		attraction += tolower(lineFromFile[index++]); // normalize string to all lower-case letters
	} // index at '|'
	index++; // go to beginning of lat
	while (lineFromFile[index] != ',')
	{
		latitude += lineFromFile[index++];
	} // index at ", "
	index = index + 2; // go to beginning of long
	while (lineFromFile[index])
	{
		longitude += lineFromFile[index++];
	}

	coords = GeoCoord(latitude, longitude);
}

string MapLoaderImpl::convertToLower(const string &str)
{
	string lowercasedLine;
	for (int i = 0; i < str.length(); i++)
	{
		lowercasedLine += tolower(str[i]);
	}
	return lowercasedLine;
}