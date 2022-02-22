#include <iostream>
#include <stdio.h>
#include <vector>

#ifdef __APPLE__ //Mac OSX has a different name for the header file
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (128)//suppose we have a vector with 128 elements
#define MAX_SOURCE_SIZE (0x100000)

void ocl_main() {
    //In general Intel CPU and NV/AMD's GPU are in different platforms
    //But in Mac OSX, all the OpenCL devices are in the platform "Apple"
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL; //"stream" in CUDA
    cl_mem memobj = NULL;//device memory
    cl_program program = NULL; //cl_prgram is a program executable created from the source or binary
    cl_kernel kernel = NULL; //kernel function
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret; //accepts return values for APIs

    float mem[MEM_SIZE]; //alloc memory on host(CPU) ram

    //OpenCL source can be placed in the source code as text strings or read from another file.
    FILE *fp;
    const char fileName[] = "./src/kernels/kernel.cl";
    size_t source_size;
    char *source_str;
    cl_int i;

    // read the kernel file into ram
    fp = fopen(fileName, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char *)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp );
    fclose( fp );

    //initialize the mem with 1,2,3...,n
    for( i = 0; i < MEM_SIZE; i++ ) {
        mem[i] = i;
    }

    //get the device info
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    cl_device_info deviceInfo;
    char deviceName[1024];
    size_t deviceNameSize;
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, 1024, deviceName, &deviceNameSize);
    printf("Selected device: %s\n", deviceName);

    //create context on the specified device
    context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);

    //create the command_queue (stream)
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    //alloc mem on the device with the read/write flag
    memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(float), NULL, &ret);

    //copy the memory from host to device, CL_TRUE means blocking write/read
    ret = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, MEM_SIZE * sizeof(float), mem, 0, NULL, NULL);

    //create a program object for a context
    //load the source code specified by the text strings into the program object
    program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

    //build (compiles and links) a program executable from the program source or binary
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    //create a kernel object with specified name
    kernel = clCreateKernel(program, "vecAdd", &ret);

    //set the argument value for a specific argument of a kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);

    //define the global size and local size (grid size and block size in CUDA)
    size_t global_work_size[3] = {MEM_SIZE, 0, 0};
    size_t local_work_size[3]  = {MEM_SIZE, 0, 0};

    //Enqueue a command to execute a kernel on a device ("1" indicates 1-dim work)
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);

    //copy memory from device to host
    ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, MEM_SIZE * sizeof(float), mem, 0, NULL, NULL);

    //print out the result
    for(i=0; i<MEM_SIZE; i++) {
        printf("mem[%d] : %.2f\n", i, mem[i]);
    }

    //clFlush only guarantees that all queued commands to command_queue get issued to the appropriate device
    //There is no guarantee that they will be complete after clFlush returns
    ret = clFlush(command_queue);
    //clFinish blocks until all previously queued OpenCL commands in command_queue are issued to the associated device and have completed.
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(memobj);//free memory on device
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    free(source_str);//free memory on host
}

