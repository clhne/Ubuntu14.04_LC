/* ------------------------------------
 *
 * AttackOfTheAliens
 * ocl.cpp
 * ------------------------------------
 */

#include <assert.h> 
#include <stdlib.h>
#include <string>
#include "ocl.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#endif

void OCLContext::Cleanup(int exitCode)
{
	if (instance)
	{
		instance->cleanupMess();
	}
	exit(exitCode);
}

void OCLContext::cleanupMess()
{
	// Cleanup allocated objects
	shrLog("\nStarting Cleanup...\n\n");
/*
	if(ckKernel)
		clReleaseKernel(ckKernel); 

	if(cpProgram)
		clReleaseProgram(cpProgram);
*/

	if(cqCommandQueue)
		clReleaseCommandQueue(cqCommandQueue);
}


void (*pCleanup)(int) = &OCLContext::Cleanup;


OCLContext::OCLContext(int argc, char ** argv)
{
	cpProgram = 0;
	ckKernel = 0;
	cqCommandQueue = 0;
	compilerOptions = NULL;
	this->init(argc, argv);
}


// static instance
OCLContext * OCLContext::instance = NULL;

OCLContext * OCLContext::getInstance()
{
	assert(instance);
	return instance;
}
 
OCLContext * OCLContext::initContext( int argc, char ** argv )
{
	if (instance) {
		return instance;
	}
	
	instance = new OCLContext( argc, argv );
	
	return instance;
}

// this assumes that the OpenGL context has been initialized
// if not, it will fail!
void OCLContext::init( int argc, char ** argv )
{
	cl_int ciErrNum;
	
	// start logs 
	shrQAStart(argc, argv);
	cExecutableName = argv[0];
	shrSetLogFileName ("AttackOfTheAliens.txt");
	shrLog("%s Starting...\n\n", argv[0]); 
	
	//Get the NVIDIA platform-- can be non-nvidia
	ciErrNum = oclGetPlatformID(&cpPlatform);
	oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
	
	// Get the number of GPU devices available to the platform
	ciErrNum = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 0, NULL, &uiDevCount);//will return back one or more gpus
	oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
	//typically want gpu not integrated graphics.  
	    
	// Create the device list
	cdDevices = new cl_device_id [uiDevCount];
	ciErrNum = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, uiDevCount, cdDevices, NULL);
	oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
	
	// Get device requested on command line, if any
	uiDeviceUsed = 0;
	unsigned int uiEndDev = uiDevCount - 1;
	if(shrGetCmdLineArgumentu(argc, (const char**)argv, "device", &uiDeviceUsed ))
	{
		uiDeviceUsed = CLAMP(uiDeviceUsed, 0, uiEndDev);
		uiEndDev = uiDeviceUsed; 
	} 


	// Check if the requested device (or any of the devices if none requested) supports context sharing with OpenGL
	bool bSharingSupported = false;
	for(unsigned int i = uiDeviceUsed; (!bSharingSupported && (i <= uiEndDev)); ++i) 
	{
		size_t extensionSize;
		ciErrNum = clGetDeviceInfo(cdDevices[i], CL_DEVICE_EXTENSIONS, 0, NULL, &extensionSize );
		oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
		if(extensionSize > 0) 
		{
			char* extensions = (char*)malloc(extensionSize);
			ciErrNum = clGetDeviceInfo(cdDevices[i], CL_DEVICE_EXTENSIONS, extensionSize, extensions, &extensionSize);
			oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
			std::string stdDevString(extensions);
			free(extensions);
			
			size_t szOldPos = 0;
			size_t szSpacePos = stdDevString.find(' ', szOldPos); // extensions string is space delimited
			while (szSpacePos != stdDevString.npos)
			{
				if( strcmp(GL_SHARING_EXTENSION, stdDevString.substr(szOldPos, szSpacePos - szOldPos).c_str()) == 0 ) 
				{
					// Device supports context sharing with OpenGL
					uiDeviceUsed = i;
					bSharingSupported = true;
					break;
				}
				
				do 
				{
					szOldPos = szSpacePos + 1;
					szSpacePos = stdDevString.find(' ', szOldPos);
				} 
				while (szSpacePos == szOldPos);
			}
		}
	}
       
	shrLog("%s...\n\n", bSharingSupported ? "Using CL-GL Interop" : "No device found that supports CL/GL context sharing");  
	oclCheckErrorEX(bSharingSupported, true, pCleanup);

	// Define OS-specific context properties and create the OpenCL context
	#if defined (__APPLE__)
		CGLContextObj kCGLContext = CGLGetCurrentContext();
		CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
		cl_context_properties props[] = 
		{
			CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, 
			(cl_context_properties) kCGLShareGroup, 
			0
		};
		
		cxGPUContext = clCreateContext(props, 0,0, NULL, NULL, &ciErrNum);
	#else
	
	#ifdef UNIX
		cl_context_properties props[] = 
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(), 
			CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(), 
			CL_CONTEXT_PLATFORM, (cl_context_properties)cpPlatform, 
			0
		};
		cxGPUContext = clCreateContext(props, 1, &cdDevices[uiDeviceUsed], NULL, NULL, &ciErrNum);
	#else // Win32
		cl_context_properties props[] = 
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), 
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), 
			CL_CONTEXT_PLATFORM, (cl_context_properties)cpPlatform, 
			0
		};
		cxGPUContext = clCreateContext(props, 1, &cdDevices[uiDeviceUsed], NULL, NULL, &ciErrNum);
	#endif
	
	#endif
	shrCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup); //if problem , bug out
	
	// Log device used (reconciled for requested requested and/or CL-GL interop capable devices, as applies)
	shrLog("Device # %u, ", uiDeviceUsed);
	oclPrintDevName(LOGBOTH, cdDevices[uiDeviceUsed]); // expect to get GPU
	shrLog("\n");
	
	// create a command-queue use 0 for no profiling or CL_QUEUE_PROFILING_ENABLE to enable profiling
	cqCommandQueue = clCreateCommandQueue(cxGPUContext, cdDevices[uiDeviceUsed], CL_QUEUE_PROFILING_ENABLE, &ciErrNum);
	shrCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
}

cl_kernel OCLContext::loadKernel(const char * kernel_file, const char * kernel_name)
{

	const char * executable_file = cExecutableName;
	
	cl_int ciErrNum;
	
	// Program Setup
	size_t program_length;
	
	// get kernel file
	char * cPathAndName = shrFindFilePath(kernel_file, executable_file);
	shrCheckErrorEX(cPathAndName != NULL, shrTRUE, pCleanup);
	char * cSourceCL = oclLoadProgSource(cPathAndName, "", &program_length);
	shrCheckErrorEX(cSourceCL != NULL, shrTRUE, pCleanup);
	
	// create the program
	cpProgram = clCreateProgramWithSource(cxGPUContext, 1,
		(const char **) &cSourceCL, &program_length, &ciErrNum);
	shrCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
	
	// build the program
	ciErrNum = clBuildProgram(cpProgram, 0, NULL, compilerOptions, NULL, NULL);
	if (ciErrNum != CL_SUCCESS)
	{
		// write out standard error, Build Log and PTX, then cleanup and exit
		shrLogEx(LOGBOTH | ERRORMSG, ciErrNum, STDERROR);
		oclLogBuildInfo(cpProgram, oclGetFirstDev(cxGPUContext));
		oclLogPtx(cpProgram, oclGetFirstDev(cxGPUContext), "oclAndyGalaxy.ptx");
		Cleanup(EXIT_FAILURE); 
	} 
	
	// create the kernel
	ckKernel = clCreateKernel(cpProgram, kernel_name, &ciErrNum);
	//shrCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
	
	// if there is a problem creating the kernel print out some detailed information about it
	if (ciErrNum) 
	{
		char log[10240] = "";
		ciErrNum = clGetProgramBuildInfo(cpProgram, cdDevices[uiDeviceUsed], CL_PROGRAM_BUILD_LOG, sizeof(log), log, NULL);
		fprintf(stderr, "Error(s) creating the kernel:\n%s\n", log);
		Cleanup(EXIT_FAILURE);
	}
	shrCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
	
	return ckKernel;
	/*
	shrDeltaT(1);  
	
	// Start main GLUT rendering loop for processing and rendering, 
	// or otherwise run No-GL Q/A test sequence
	shrLog("\nEntering main Render loop..."); 
	
	//glutMainLoop();
	
	// Normally unused return path
	*/
	
}


cl_kernel OCLContext::loadKernelGivenSource(const char * cSourceCL, const char * kernel_name)
{
	const char * executable_file = cExecutableName;
	
	cl_int ciErrNum;
	
	// Program Setup
	size_t program_length = strlen(cSourceCL);
	
	// create the program
	cpProgram = clCreateProgramWithSource(cxGPUContext, 1,
		(const char **) &cSourceCL, &program_length, &ciErrNum);
	shrCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
	
	// build the program
	ciErrNum = clBuildProgram(cpProgram, 0, NULL, compilerOptions, NULL, NULL);
	if (ciErrNum != CL_SUCCESS)
	{
		// write out standard error, Build Log and PTX, then cleanup and exit
		shrLogEx(LOGBOTH | ERRORMSG, ciErrNum, STDERROR);
		oclLogBuildInfo(cpProgram, oclGetFirstDev(cxGPUContext));
		oclLogPtx(cpProgram, oclGetFirstDev(cxGPUContext), "oclAndyGalaxy.ptx");
		Cleanup(EXIT_FAILURE); 
	} 
	
	// create the kernel
	ckKernel = clCreateKernel(cpProgram, kernel_name, &ciErrNum);
	//shrCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
	
	// if there is a problem creating the kernel print out some detailed information about it
	if (ciErrNum) 
	{
		char log[10240] = "";
		ciErrNum = clGetProgramBuildInfo(cpProgram, cdDevices[uiDeviceUsed], CL_PROGRAM_BUILD_LOG, sizeof(log), log, NULL);
		fprintf(stderr, "Error(s) creating the kernel:\n%s\n", log);
		Cleanup(EXIT_FAILURE);
	}
	shrCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
	
	return ckKernel;
	
}

