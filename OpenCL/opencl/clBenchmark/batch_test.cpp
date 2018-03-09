/* ------------------------------------
 *
 * OpenCL image convolution Benchmark
 * batch_test.cpp
 * ------------------------------------
 */

#include <assert.h> 
#include "batch_test.h"

BatchTest * BatchTest::getInstance()
{
	assert(instance);
	return instance;
}

BatchTest * BatchTest::initialize(int beginSize, int endSize)
{
	assert(!instance);
	instance = new BatchTest(beginSize, endSize);
}

BatchTest::BatchTest(int beginSize, int endSize)
{
	// create benchmarkers
	for (int filterSize = beginSize; filterSize <= endSize; filterSize += 2)
	{
		Benchmarker * b = new Benchmarker(filterSize);
		tests.push_back( b );
	}
}

BatchTest::~BatchTest()
{
	for (int i = 0; i < tests.size(); i++)
	{
		delete tests[i];
	}
}


void BatchTest::runTest(int runTimes)
{
	cout << " ====== T E S T ===== " << runTimes << " iterations ==== " << endl;
	for (int i = 0; i < tests.size(); i++)
	{
		Benchmarker * b = tests[i];
		int filterSize = b->getFilterSize();
		cout << "\nFilter: " << filterSize << " x " << filterSize << endl;
		b->run( runTimes );
	}
}



BatchTest * BatchTest::instance = NULL; 
