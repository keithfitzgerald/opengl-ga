#include <stdio.h>
#include <stdlib.h>
//#include <CL/opencl.h>
#include <OpenCL/opencl.h>

#define BUF_SZ 2048
#define byte unsigned char

#define CERR(x,msg)               \
    if (x != 0) {                 \
        printf("ERROR [%d]: %s\n",x,msg);  \
        return x;                \
    } else {\
        printf("%s ... OK!\n",msg); \
    }                              

void load_cl(char *filename, char *buffer) {
    // TODO: error handling
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long pos = ftell(f);
    fseek(f, 0, SEEK_SET);

    fread(buffer, pos, 1, f);
    buffer[pos] = '\0';
    fclose(f);
}

int setup_opencl(cl_context *context, cl_command_queue *commandQueue, cl_kernel *kernel) {

    char source[4096];
    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_program program;

    err = clGetPlatformIDs(1, &platform, NULL);
    CERR(err,"clGetPlatformIDs");

    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    CERR(err,"clGetDeviceIDs");

    *context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    CERR(err,"clCreateContext");
    
    *commandQueue = clCreateCommandQueue(*context, device, 0, &err);
    CERR(err,"clCreateCommandQueue");

    load_cl("kernel.cl",source);
    const char *str = source;
    //printf("%s\n",str);
    program = clCreateProgramWithSource(*context, 1, &str, 0, &err);
    CERR(err,"clCreateProgramWithSource");
    
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[16000];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        return 1;
    }

    *kernel = clCreateKernel(program, "fitness", &err);
    CERR(err,"clCreateKernel");
    
    clReleaseProgram(program);

    return 0;
}

long cl_calc_fitness(byte *src, byte *ref, int bufsz, cl_context *context, 
                  cl_command_queue *commandQueue, cl_kernel *kernel) {
    
    cl_int err;
    // compute work group sizes
    size_t local = 256;
    int work_size = (BUF_SZ + local - 1) / local;
    size_t global = work_size * local;
    int work_groups = global/local;
    printf("local: %ld, global: %ld\n",local,global);
    
    cl_mem cl_src = clCreateBuffer(*context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, 
                               sizeof(byte) * bufsz, src, &err);
    CERR(err,"clCreateBuffer (src)");

    cl_mem cl_ref = clCreateBuffer(*context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, 
                               sizeof(byte) * bufsz, ref, &err);
    CERR(err,"clCreateBuffer (ref)");

    //unsigned long res[work_groups];
    cl_mem cl_res = clCreateBuffer(*context, CL_MEM_READ_WRITE, 
                                   sizeof(long) * work_groups, NULL, &err);
    CERR(err,"clCreateBuffer (res)");

    err = clSetKernelArg(*kernel, 0, sizeof(cl_src), &cl_src);
    CERR(err,"clSetKernelArg (0)");

    err = clSetKernelArg(*kernel, 1, sizeof(cl_ref), &cl_ref);
    CERR(err,"clSetKernelArg (1)");

    err = clSetKernelArg(*kernel, 2, sizeof(cl_long) * local, NULL);
    CERR(err,"clSetKernelArg (2)");

    err = clSetKernelArg(*kernel, 3, sizeof(int), &bufsz);
    CERR(err,"clSetKernelArg (3)");

    err = clSetKernelArg(*kernel, 4, sizeof(cl_res), &cl_res);
    //err = clSetKernelArg(*kernel, 4, sizeof(cl_res), NULL);
    CERR(err,"clSetKernelArg (4)");

    err = clEnqueueNDRangeKernel(*commandQueue, *kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    CERR(err,"clEnqueueNDRangeKernel");

    // TODO: heap?
    long result[2048];
    err = clEnqueueReadBuffer(*commandQueue, cl_res, CL_TRUE, 0, sizeof(long) * work_groups, result, 0, NULL, NULL);
    CERR(err,"clEnqueueReadBuffer");

    clReleaseMemObject(cl_src);
    clReleaseMemObject(cl_ref);
    clReleaseMemObject(cl_res);

    long cl_sum = 0;
    for(int i = 0; i < work_groups;i++) {
        if (result[i] > 0) {
            printf("%d:%ld\n",i,result[i]);
            cl_sum += result[i];
        }
    }

    return cl_sum;
}


int test(int argc, char** argv)
{
    cl_kernel kernel;
    cl_context context;
    cl_command_queue commandQueue;


    byte src[BUF_SZ];
    byte ref[BUF_SZ];

    for(int i = 0; i < BUF_SZ;i++) {
        src[i] = i;
    }

    for(int i = 0; i < BUF_SZ;i++) {
        ref[i] = i + 10;
    }

    long sum = 0;
    for (int i = 0;i < BUF_SZ;i++) {
        sum += abs(src[i] - ref[i]);
    }
   
    printf("setup_opencl\n");
    setup_opencl(&context,&commandQueue,&kernel);
 
    printf("calc_fitness\n");
    long cl_sum = cl_calc_fitness(src,ref,BUF_SZ,&context,&commandQueue,&kernel);

    printf("cl_sum = %ld\n",cl_sum);
    printf("sum = %ld\n",sum);


    clReleaseKernel(kernel);
    clReleaseCommandQueue(commandQueue);
    clReleaseContext(context);
  
    return 0;
}
