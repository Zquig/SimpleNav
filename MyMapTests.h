#pragma once

#include "MyMap.h"
#include <random>
#include <time.h>
#include<iostream>

using namespace std;

namespace TestMyMap
{
	const int ARRAY_SIZE = 10;

	void fillArray(int* intArray, int arraySize)
	{
		for (int i = 0; i < arraySize; i++)
		{
			*(intArray + i) = rand() % 100; // Place a random integer between 0 and 99 in array index
		}

		return;
	}

	void printArray(int* intArray, int arraySize)
	{
		for (int i = 0; i < arraySize; i++)
		{
			cerr << *(intArray + i) << endl;
		}
	}

	void runTest()
	{
		srand(time(NULL)); // initialize random seed

		int testArray[ARRAY_SIZE];
		fillArray(testArray, ARRAY_SIZE);
		printArray(testArray, ARRAY_SIZE);
		cout << endl;

		MyMap<const int, int> testBST;

		//cerr << "Tree size: " << testBST.size() << endl;

		for (int i = 0; i < ARRAY_SIZE; i++)
		{
			int key = testArray[i];
			int value = rand() % 10;
			testBST.associate(key, value);
			cout << i << ". " << key << " --> " << value << endl;
			cerr << "Tree size: " << testBST.size() << endl;
		}

		cerr << endl;

		for (int i = ARRAY_SIZE - 1; i >= 0; i--)
		{
			int key = testArray[i];
			int* valuePtr = testBST.find(key);
			cerr << key << " --> ";
			if (nullptr != valuePtr)
				cerr << *valuePtr << endl;
			else cerr << " xx " << endl;
		}

		cerr << endl;
		//testBST.printBST_BreadthFirst();
	}
}