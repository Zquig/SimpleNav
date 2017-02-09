#pragma once
// Test driver for NavImpl class
// Using "friend class" method

#include "NavImpl.h"
#include <iostream>

using namespace std;

class NavImplTests
{
public:
	bool initializeTest()
	{
		m_testNavImpl = unique_ptr<NavImpl>(new NavImpl);
		return (m_testNavImpl->loadMapData(DATA_FILENAME));
	}

	void should_printProperNavigationSteps()
	{
		while (true)
		{
			string start, end;
			cout << "Enter name of starting attraction ('q' to quit):" << endl;
			getline(cin, start);
			if ("q" == start)
			{
				break;
			}

			cout << "Enter name of ending attraction ('q' to quit):" << endl;
			getline(cin, end);
			if ("q" == end)
			{
				break;
			}

			vector<NavSegment> testVector;
			switch (m_testNavImpl->navigate(start, end, testVector))
			{
			case NAV_RESULT::invalid_source:
			{
				cout << "Invalid source\n";
				break;
			}
			case NAV_RESULT::invalid_destination:
			{
				cout << "Invalid destination\n";
				break;
			}
			case NAV_RESULT::invalid_impl:
			{
				cout << "Invalid implementation\n";
				return;
			}
			case NAV_RESULT::no_route:
			{
				cout << "No route\n";
				break;
			}
			case NAV_RESULT::success:
			{
				cout << "SUCCESS!\n";
				break;
			}
			}
		}
	}
private:
	const string DATA_FILENAME = "mapdata.dat";

private:
	unique_ptr<NavImpl> m_testNavImpl;
};