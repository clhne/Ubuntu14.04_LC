/* ------------------------------------
 *
 * OpenCL image convolution Benchmark
 * batch_test.h
 * ------------------------------------
 */

#ifndef _BATCH_TEST_H___
#define _BATCH_TEST_H___

#include <vector>
#include "benchmark.h"

using namespace std;

class BatchTest
{
public:
	~BatchTest();
	
	static BatchTest * getInstance();
	static BatchTest * initialize(int, int);
	void runTest(int runTimes);
	
	int getTestCount() const { return tests.size(); }
	const Benchmarker * getTest(int i) const { return tests[i]; } 
private:
	// private constructor
	BatchTest(int beginSize, int endSize);
	
	vector<Benchmarker *> tests;
	
	// static instance
	static BatchTest * instance;
	
};


#endif

