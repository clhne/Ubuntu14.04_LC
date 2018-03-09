/* ------------------------------------
 *
 * OpenCL image convolution Benchmark
 * benchmark.h
 * ------------------------------------
 */
 
#ifndef _BENCHMARK_H___
#define _BENCHMARK_H___

#include <string>
#include <vector>
#include "ocl.h"
#include "graphics.h"
#include "texture_unit.h"

using namespace std;

class Kernel;
class Benchmarker;

/* ------------------------------------------------------------------
 * Kernel: stores a single OpenCL kernel along with its benchmarking
 * results
 * ------------------------------------------------------------------
 */
class Kernel
{
public:
	Kernel(int _w, int _h, const char * filename, const char * kernelName, const char * source);
	~Kernel();
	
	// clears old performance results
	void clear() { runTimes.clear(); avgRunTime = -1.0f; }
	void averageRunTimes();
	cl_kernel getKernel() const { return kernel; }
	void makeOpenGLTexture();
	
	float getAvgRunTime() const { return avgRunTime; }
	int getLocalWorkSizeX() const { return localWorkSizeX; }
	int getLocalWorkSizeY() const { return localWorkSizeY; }
	
	int usesLocalMem() const { return localMem; }
	int getLocalMemSize() const { return localMemSize; }
	
private:
	// benchmark results
	vector<float> runTimes;
	float avgRunTime;
	
	// file name
	string filename;
	
	// data from kernel
	int w, h;
	float * results;
	GLuint texture;
	
	// OpenCL settings
	cl_kernel		kernel;
	cl_program		program;
	
	// whether kernel uses local memory
	bool			localMem;
	
	// local group size
	int			localWorkSizeX;
	int			localWorkSizeY;
	int			localMemSize;
	
	friend class Benchmarker;
};


/* ------------------------------------------------------------------
 * Benchmarker: manages all kernel in the test
 * ------------------------------------------------------------------
 */
 
class Benchmarker
{

public:
	Benchmarker(int _filterSize);
	~Benchmarker();
	static Benchmarker * getInstance();
	
	void run(int runTimes);
	int getImageWidth() const { return inputImage.getWidth(); }
	int getImageHeight() const { return inputImage.getHeight(); }
	void bindTexture(int index) const;
	
	int getFilterSize() const { return filterSize; }
	int getKernelCount() const { return kernels.size(); }
	const Kernel * getKernel(int i) const { return kernels[i]; }
	
	// unrolls the convolution loop, result string is in output. returns true for success
	static bool unrollLoop(int _filterSize, string input, string & output);
	static bool unrollLoop_local(int _filterSize, int localWorkSizeX, string input, string & output);

private:
	// private constrcutor
	Benchmarker();
	void generateFilter();
	
	// all kernels
	vector<Kernel *> kernels;
	
	// input image
	static TextureUnit inputImage;
	
	// buffers
	cl_mem input_cl, output_cl, local_cl, filter_cl;
	
	// filter
	float * filter;
	int filterSize;
	
	// compiler options
	char compilerOptions[1024];
	
	// static instance
	static Benchmarker * instance;
};


#endif


