/* ------------------------------------
 *
 * OpenCL image convolution Benchmark
 * benchmark.cpp
 * ------------------------------------
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <math.h>
#include "benchmark.h"
#include "graphics.h"
#include "misc.h"
#include "texture_unit.h"

using namespace std;

#define PI				3.14159265
static const int MAX_LOCAL_MEM_SIZE =	16 * 1000;

static const bool SKIP_EXCESS_LOCAL_MEM = true;

static const float neutral3[3*3] = {
	0.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 0.0
};

static const float gaussian3[3*3] = {
	1.0 / 16.0,	2.0 / 16.0,	1.0 / 16.0,
	2.0 / 16.0,	4.0 / 16.0,	2.0 / 16.0,
	1.0 / 16.0,	2.0 / 16.0,	1.0 / 16.0	
};

static const float gaussian5[5*5] = {
	1.0 / 273.0,	4.0 / 273.0,	7.0 / 273.0,	4.0 / 273.0,	1.0 / 273.0,
	4.0 / 273.0,	16.0 / 273.0,	26.0 / 273.0,	16.0 / 273.0,	4.0 / 273.0,
	7.0 / 273.0,	26.0 / 273.0,	41.0 / 273.0,	26.0 / 273.0,	7.0 / 273.0,
	4.0 / 273.0,	16.0 / 273.0,	26.0 / 273.0,	16.0 / 273.0,	4.0 / 273.0,
	1.0 / 273.0,	4.0 / 273.0,	7.0 / 273.0,	4.0 / 273.0,	1.0 / 273.0,
};


// return name of image
const char * getInputFile();

/* ------------------------------
 * Kernel
 * ------------------------------
 */
 
Kernel::Kernel(int _w, int _h, const char * file, const char * kernelName, const char * source): w(_w), h(_h)
{
	OCLContext * ocl = OCLContext::getInstance();
	
	kernel = ocl->loadKernelGivenSource(source, kernelName);
	program = ocl->getProgram();
	
	results = new float[ w * h * 4 ];
	avgRunTime = -1.0f;
	
	filename = file;
	glGenTextures(1, &texture);
	
	localMem = false;
	localWorkSizeX = localWorkSizeY = 0;
	localMemSize = 0;
}

Kernel::~Kernel()
{
	delete [] results;
	glDeleteTextures(1, &texture);
	
	clReleaseKernel(kernel);
	clReleaseProgram(program);
}

void Kernel::averageRunTimes()
{
	if (runTimes.size() == 0)
	{
		avgRunTime = -1.0f;
	}
	else
	{
		float total = 0.0f;
		for (int i = 0; i < runTimes.size(); i++)
		{
			total += runTimes[i];
		}
		avgRunTime = total / (float) runTimes.size();
	}
}

unsigned char clamp255(float f)
{
	if (f < 0.0)
	{
		return 0;
	}
	else if (f > 255.0)
	{
		return 255;
	}
	else
	{
		return (unsigned char) f;
	}
}


void Kernel::makeOpenGLTexture()
{
	unsigned char * pic = new unsigned char[w * h * 4];
	unsigned char * p = pic;
	const float * source = results;
	
	for (int r = 0; r < h; r++)
	{
		for (int c = 0; c < w; c++, p += 4, source += 4)
		{
			p[0] = (unsigned char) clamp255(floor( source[0] + .5f ));
			p[1] = (unsigned char) clamp255(floor( source[1] + .5f ));
			p[2] = (unsigned char) clamp255(floor( source[2] + .5f ));
			p[3] = (unsigned char) clamp255(floor( source[3] + .5f ));

		}
	}
	
	// uploaad texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pic);

	// set parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	// cleanup
	delete [] pic;
}


/* ------------------------------
 * Benchmarker
 * ------------------------------
 */

bool Benchmarker::unrollLoop_local(int _filterSize, int localWorkSizeX, string input, string & output)
{
	char buffer[1024];
	string loop;
	
	int HALF_FILTER_SIZE = _filterSize / 2;
	int localRowLen = (HALF_FILTER_SIZE * 2 + localWorkSizeX);
	int fIndex = 0;
	
	vector<string> iterations;
	
	for (int r = -HALF_FILTER_SIZE; r <= HALF_FILTER_SIZE; r++)
	{	
		int curRow = r * localRowLen;
				
		for (int c = -HALF_FILTER_SIZE; c <= HALF_FILTER_SIZE; c++, fIndex++)
		{
			int cachedOffset = curRow + c;
			if (cachedOffset < 0)
			{
				sprintf(buffer, "\tsum += cached[ myLocal %d ] * filter[ %d ]; \n", cachedOffset, fIndex);
				iterations.push_back( buffer );
			}
			else if (cachedOffset > 0)
			{
				sprintf(buffer,  "\tsum += cached[ myLocal + %d ] * filter[ %d ]; \n", cachedOffset, fIndex);
				iterations.push_back( buffer );
			}
			else
			{
				sprintf(buffer,  "\tsum += cached[ myLocal ] * filter[ %d ]; \n", fIndex);
				iterations.push_back( buffer );
			}
			//loop += buffer;
		}
		//loop += "\n";
	}
	
	// randomize order of iterations
	/*
	for (int i = 0; i < 1000; i++)
	{
		int r1 = rand() % iterations.size();
		int r2 = rand() % iterations.size();
		
		string temp = iterations[r2];
		iterations[r2] = iterations[r1];
		iterations[r1] = temp;
	}
	*/
	
	// put in the loop
	for (int i = 0; i < iterations.size(); i++)
	{
		loop += iterations[i];
	}
	
	
	const string needle("@@UNROLLED LOCAL LOOP@@");
	size_t replacePos = input.find( needle );
	if (replacePos == string::npos)
	{
		return false;
	}
	else
	{
		output = input.replace(replacePos, needle.length(), loop); 
		return true;
	}
	
}

bool Benchmarker::unrollLoop(int _filterSize, string input, string & output)
{
	char buffer[1024];
	string loop;
	
	int HALF_FILTER_SIZE = _filterSize / 2;
	int IMAGE_W = inputImage.getWidth();
	int fIndex = 0;
	
	vector<string> iterations;
	
	for (int r = -HALF_FILTER_SIZE; r <= HALF_FILTER_SIZE; r++)
	{
		
		int curRow = r * IMAGE_W;
		/*
		sprintf(buffer, "\tcurRow = my + (%d); \n", (r * IMAGE_W));
		loop += buffer;
		*/
		
		
		for (int c = -HALF_FILTER_SIZE; c <= HALF_FILTER_SIZE; c++, fIndex++)
		{
			int inputOffset = curRow + c;
			if (inputOffset < 0)
			{
				sprintf(buffer, "\tsum += input[ my %d ] * filter[ %d ]; \n", inputOffset, fIndex);
				iterations.push_back( buffer );
			}
			else if (inputOffset > 0)
			{
				sprintf(buffer,  "\tsum += input[ my + %d ] * filter[ %d ]; \n", inputOffset, fIndex);
				iterations.push_back( buffer );
			}
			else
			{
				sprintf(buffer,  "\tsum += input[ my ] * filter[ %d ]; \n", fIndex);
				iterations.push_back( buffer );
			}
			//loop += buffer;
		}
		//loop += "\n";
	}
	
	// randomize order of iterations
	/*
	for (int i = 0; i < 1000; i++)
	{
		int r1 = rand() % iterations.size();
		int r2 = rand() % iterations.size();
		
		string temp = iterations[r2];
		iterations[r2] = iterations[r1];
		iterations[r1] = temp;
	}
	*/

	// put in the loop
	for (int i = 0; i < iterations.size(); i++)
	{
		loop += iterations[i];
	}
	
	
	const string needle("@@UNROLLED LOOP@@");
	size_t replacePos = input.find( needle );
	if (replacePos == string::npos)
	{
		return false;
	}
	else
	{
		output = input.replace(replacePos, needle.length(), loop); 
		return true;
	}
	
	
}

Benchmarker::Benchmarker(int _filterSize)
{
	OCLContext * ocl = OCLContext::getInstance();
	
	filterSize = _filterSize;
	generateFilter();

	// load input image
	if (!inputImage.isLoaded())
	{
		inputImage.readFile(getInputFile(), 4);
	}
	//cout << "\nLoaded image: " << inputImage.getWidth() << " x " << inputImage.getHeight() << endl;

	// allocate memory
	int totalSize = inputImage.getWidth() * inputImage.getHeight() * 4;	
	float * inputBuffer = new float[ totalSize ];
	
	// set pointers to beginning of buffers
	const unsigned char * p = inputImage.getPixel(0, 0);
	float * b = inputBuffer;
	
	for (int r = 0; r < inputImage.getHeight(); r++)
	{
		for (int c = 0; c < inputImage.getWidth(); c++, p += 4, b += 4)
		{
			b[0] = (float) p[0];
			b[1] = (float) p[1];
			b[2] = (float) p[2];
			b[3] = (float) p[3];
		}
	}
	
	// set compiler options
	// (two predefined macros specifying size of image and filter)
	sprintf(compilerOptions, "-D IMAGE_W=%d -D IMAGE_H=%d -D FILTER_SIZE=%d -D HALF_FILTER_SIZE=%d -D TWICE_HALF_FILTER_SIZE=%d -D HALF_FILTER_SIZE_IMAGE_W=%d",
		inputImage.getWidth(),
		inputImage.getHeight(),
		filterSize,
		filterSize/2,
		(filterSize/2) * 2,
		(filterSize/2) * inputImage.getWidth()
	);
	ocl->setCompilerOptions(compilerOptions);
	
	// input buffer
	cl_int ciErrNum;
	input_cl = clCreateBuffer(ocl->getGPUContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		totalSize * sizeof(float), inputBuffer, &ciErrNum);
	shrCheckErrorEX(ciErrNum, CL_SUCCESS, &OCLContext::Cleanup);
	
	// output buffer
	output_cl = clCreateBuffer(ocl->getGPUContext(), CL_MEM_WRITE_ONLY,
		totalSize * sizeof(float), NULL, &ciErrNum);
	shrCheckErrorEX(ciErrNum, CL_SUCCESS, &OCLContext::Cleanup);

	// filter
	filter_cl = clCreateBuffer(ocl->getGPUContext(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		filterSize * filterSize * sizeof(float) * 4, filter, &ciErrNum);
	shrCheckErrorEX(ciErrNum, CL_SUCCESS, &OCLContext::Cleanup);
	
	// cleanup
	delete [] inputBuffer;
	
	// load all kernels
	// --------------------------
	vector<string> clDir;
	if (! listDir("cl", clDir) )
	{
		cerr << "Error opening 'cl/'" << endl;
		return;
	}
	
	cout << "Loading kernels for filter size " << filterSize << " x " << filterSize << " ..." << endl;
	for (int i = 0; i < clDir.size(); i++)
	{
		string clName = getFileName(clDir[i]);
		string filename = string("cl/") + clDir[i];
		cout << "\t--> " << filename;

		// determine if kernel uses local memory (name should have the world 'local')
		bool kernelOk = true;
		bool localMem = false;
		int localWorkSizeX = 0, localWorkSizeY = 0;
		int localMemSize = 0;
		
		if (clName.find("local") != string::npos)
		{
			localMem = true;
			kernelOk = false;
			
			vector<string> * tokens = split(getFileName( clName ), "-");
			if (tokens->size() != 2)
			{
				localMem = false;
				cout << " uses local mem, but no local work size defined.";
			}
			else
			{				
				// find the X
				size_t pos = tokens->at(1).find("x");
				if (pos == string::npos)
				{
					localMem = false;
					cout << " uses local mem, but no local work size defined.";
					goto skip;
				}
				
				if (
					!from_string(localWorkSizeX, tokens->at(1).substr(0, pos)) ||
					!from_string(localWorkSizeY, tokens->at(1).substr(pos+1))
				)
				{
					localMem = false;
					cout << " uses local mem, but no local work size defined.";
					goto skip;
					
				}
			
				kernelOk = true;
					
				// calculate local memory size
				localMemSize = 
					( localWorkSizeX + 2 * (filterSize / 2) ) *
					( localWorkSizeY + 2 * (filterSize / 2) );
				localMemSize *= 4 * sizeof(float);
				
				//kernel->localMemSize = localMemSize;
				cout << " LOCAL work: " << localWorkSizeX << " x " << localWorkSizeY << ", LOCAL mem: " << localMemSize << endl << "\t\t\t";
			}
		skip:
			delete tokens;
		}
		
		// load the file
		char * sourceFile = readTextFile(filename.c_str());
		string modifiedSource = sourceFile;
		
		if (localMem)
		{
			if (!unrollLoop_local(filterSize, localWorkSizeX, sourceFile, modifiedSource))
			{
				modifiedSource = sourceFile;
			}
		}
		else
		{
			if (!unrollLoop(filterSize, sourceFile, modifiedSource))
			{
				modifiedSource = sourceFile;
			}
		}
		delete [] sourceFile;
				
		// load the kernel
		Kernel * kernel = new Kernel( 
			inputImage.getWidth(), inputImage.getHeight(),
			filename.c_str(), "convolute", modifiedSource.c_str()
		);
		kernel->filename = clName;
		kernel->localMem = localMem;
		if (localMem)
		{
			kernel->localMemSize = localMemSize;
			kernel->localWorkSizeX = localWorkSizeX;
			kernel->localWorkSizeY = localWorkSizeY;
		}
		
		if (kernelOk)
		{
			// setup arguments for kernel
			ciErrNum  = clSetKernelArg(kernel->getKernel(), 0, sizeof(cl_mem), (void *) &input_cl);
			ciErrNum |= clSetKernelArg(kernel->getKernel(), 1, sizeof(cl_mem), (void *) &output_cl);
			ciErrNum |= clSetKernelArg(kernel->getKernel(), 2, sizeof(cl_mem), (void *) &filter_cl);
			
			if (kernel->usesLocalMem())
			{
				ciErrNum |= clSetKernelArg(kernel->getKernel(), 3, kernel->localMemSize, 0 );
			}
			
			// check error
			shrCheckErrorEX(ciErrNum, CL_SUCCESS, &OCLContext::Cleanup);
		
			cout << "\t\tOK" << endl;
		
			// add to list of kernel
			kernels.push_back(kernel);
		}
		else
		{
			delete kernel;
			cout << "\t\tFailed." << endl;
		}
	}
}

Benchmarker::~Benchmarker()
{
	// delete kernels
	for (int i = 0; i < kernels.size(); i++)
	{
		delete kernels[i];
	}
	delete [] filter;
}


void Benchmarker::run(int runTimes)
{
	OCLContext * ocl = OCLContext::getInstance();
	int totalSize = inputImage.getWidth() * inputImage.getHeight() * 4;

	// error code
	cl_int ciErrNum;
	
	// events and profiling vars
	cl_event eventGlobal;
	cl_int errcode_ret;
	cl_ulong end, start;
	
	size_t globalWorkSize[] = { inputImage.getWidth(), inputImage.getHeight() };
	
	for (int i = 0; i < kernels.size(); i++)
	{
		Kernel * kernel = kernels[i];
		kernel->clear();
		
		// get local work size
		size_t localWorkSize[] = { kernel->getLocalWorkSizeX(),  kernel->getLocalWorkSizeY() };
		
		cout << kernel->filename << "\t\t\t" << flush;
		
		bool failure = false;
		bool skipped = SKIP_EXCESS_LOCAL_MEM &&
			kernel->usesLocalMem() && kernel->getLocalMemSize() > MAX_LOCAL_MEM_SIZE ? true : false;
		if (skipped)
		{
			goto skipSim;
		}
		for (int t = 0; t < runTimes; t++)
		{
			// run all kernels
			ciErrNum = clEnqueueNDRangeKernel(
				ocl->getQueue(), kernel->getKernel(), 
				2, NULL, globalWorkSize, kernel->usesLocalMem() ? localWorkSize : NULL, 0, 0, &eventGlobal );
			if (ciErrNum != CL_SUCCESS)
			{
				failure = true;
				break;
			}
	
			// lets do some profiling
			errcode_ret = clWaitForEvents(1, &eventGlobal);
			oclCheckError(errcode_ret, CL_SUCCESS);
			errcode_ret = clGetEventProfilingInfo(eventGlobal, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, 0);
			errcode_ret |= clGetEventProfilingInfo(eventGlobal, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, 0);
			
			kernel->runTimes.push_back( (end-start)*1.0e-6f );
		}
		
	skipSim:
		if (failure || skipped)
		{
			/*
			memset( kernel->results, 0, totalSize * sizeof(float) );
			kernel->makeOpenGLTexture();
			*/
			cout << (skipped ? "exceeds local mem" : "FAILED !");
			cout << endl;
			
		}
		else
		{
			// read alien / milirary data
				
			ciErrNum = clEnqueueReadBuffer(ocl->getQueue(), output_cl, true, 0, 
				totalSize * sizeof(float), kernel->results, 0, 0, 0);
			shrCheckErrorEX(ciErrNum, CL_SUCCESS, &OCLContext::Cleanup);
			kernel->makeOpenGLTexture();

			// average run times
			kernel->averageRunTimes();
			cout  << fixed << setprecision( 1 ) << kernel->avgRunTime << endl;
			cout << flush;
		}
	}
}

void Benchmarker::bindTexture(int index) const
{
	glBindTexture(GL_TEXTURE_2D, kernels[index]->texture);
}

void Benchmarker::generateFilter()
{
	const float DELTA = 1.84089642f * ((float) filterSize / 7.0);
	const float TWO_DELTA_SQ = 2.0f * DELTA * DELTA;
	const float k = 1.0f / (PI * TWO_DELTA_SQ);

	
	filter = new float[filterSize * filterSize * 4];
	float * fP = filter;
	int w = filterSize / 2;
	
	const float * precomputed = NULL;
	
	if (filterSize == 3)
	{
		precomputed = gaussian3;
	}
	else if (filterSize == 5)
	{
		precomputed = gaussian5;
	}
	
	
	for (int r = -w; r <= w; r++)
	{
		for (int c = -w; c <= w; c++, fP += 4)
		{
			if (precomputed)
			{
				fP[0] = *precomputed;
				fP[1] = *precomputed;
				fP[2] = *precomputed;
				fP[3] = r == c && c == 0 ? 1.0f : 0.0f;
				
				precomputed++;
			}
			else
			{
				const float v = k * exp( -(r*r + c*c) / TWO_DELTA_SQ );
				fP[0] = v;
				fP[1] = v;
				fP[2] = v;
				fP[3] = r == c && c == 0 ? 1.0f : 0.0f;
			}
		}
		
	}
}


// initialize static instance to NULL
Benchmarker * Benchmarker::instance = NULL;
TextureUnit Benchmarker::inputImage;
