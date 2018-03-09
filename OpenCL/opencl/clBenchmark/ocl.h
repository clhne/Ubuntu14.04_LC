/* ------------------------------------
 *
 * AttackOfTheAliens
 * ocl.h
 * ------------------------------------
 */
 
#ifndef _OCL_H___
#define _OCL_H___

#ifdef _WIN32
#  define WINDOWS_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#endif

// include graphics
#include "graphics.h"

// Utilities, OpenCL and system includes
#include <oclUtils.h>
#include <shrQATest.h>

// defines
#if defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#else
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

// forward class declaration
class OCLContext;
class OCLKernel;


/* -------------------------------------------------------------
 * OCLContext: manages the OpenCL context and sharing between
 * OpenGL and OpenCL
 * -------------------------------------------------------------
 */

class OCLContext
{
public:
	// accesses and initializes cl context
	static OCLContext * getInstance();
	static OCLContext * initContext( int argc, char ** argv );
	static void Cleanup(int exitCode);
	
	cl_kernel loadKernel(const char * kernel_file, const char * kernel_name);
	cl_kernel loadKernelGivenSource(const char * cSourceCL, const char * kernel_name);

	cl_context getGPUContext() { return cxGPUContext; } 
	cl_command_queue getQueue() { return cqCommandQueue; }
	cl_program getProgram() { return cpProgram; }
	
	void setCompilerOptions(const char * options) { compilerOptions = options; }
	
private:
	// private constructor
	OCLContext(int argc, char ** argv);
	
	// initializer function
	void init(int, char **);
	
	// class-based cleanup function
	void cleanupMess();
	
	// static instance
	static OCLContext * instance;
	
	// OpenCL vars
	cl_platform_id		cpPlatform;
	cl_context		cxGPUContext;
	cl_device_id *		cdDevices;
	cl_uint			uiDevCount;
	cl_command_queue		cqCommandQueue;

	// program / kernel vars
	cl_program		cpProgram;
	cl_kernel		ckKernel;	
	// OpenCL device to be used
	unsigned int uiDeviceUsed;
	
	// executable path
	const char * cExecutableName;
	const char * compilerOptions;
};


/* -------------------------------------------------------------
 * OCLKernel: manages an OpenCL kernel and uploads the code
 * -------------------------------------------------------------
 */


class OCLKernel
{
public:
	OCLKernel();

private:
	cl_program		cpProgram;
	cl_kernel		ckKernel;
};


#endif


