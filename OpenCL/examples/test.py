import pyopencl as cl  
   
print("CL_VERSION:",cl.VERSION)  
print("CL_HEADER_VERSION:",cl.get_cl_header_version())  
print()  
   
platforms  = cl.get_platforms()  
print("Platform num:",len(platforms))  
   
for plat in platforms:  
    print("--Platform Name:",plat.get_info(cl.platform_info.NAME))  
#   print("--Platform Extensions:",plat.get_info(cl.platform_info.EXTENSIONS))  
    print("--Platform Profile:",plat.get_info(cl.platform_info.PROFILE))  
    print("--Platform Vendor:",plat.get_info(cl.platform_info.VENDOR))  
    print("--Platform Version:",plat.get_info(cl.platform_info.VERSION))  
   
    devices = plat.get_devices(cl.device_type.ALL)  
    print("--device num:",len(devices))  
   
    for device in devices:  
        print("----Name:",device.get_info(cl.device_info.NAME))  
        print("----OpenCL_C_Version:",device.get_info(cl.device_info.OPENCL_C_VERSION))  
        print("----Vendor:",device.get_info(cl.device_info.VENDOR))  
        print("----Version:",device.get_info(cl.device_info.VERSION))  
        print("----Driver Version:",device.get_info(cl.device_info.DRIVER_VERSION))  
           
        print("----MAX_WORK_GROUP_SIZE:",device.get_info(cl.device_info.MAX_WORK_GROUP_SIZE))  
        print("----MAX_COMPUTE_UNITS:",device.get_info(cl.device_info.MAX_COMPUTE_UNITS))  
        print("----MAX_WORK_ITEM_SIZES:",device.get_info(cl.device_info.MAX_WORK_ITEM_SIZES))  
        print("----LOCAL_MEM_SIZE:",device.get_info(cl.device_info.LOCAL_MEM_SIZE))  
   
   
ctx = cl.Context(dev_type=cl.device_type.ALL,  
    properties=[(cl.context_properties.PLATFORM,platforms[0])])   

