#ifndef __OPENCL_H__
#define __OPENCL_H__

#include <OpenCL/opencl.h>

setup_opencl(cl_context *context, 
             cl_command_queue *commandQueue, 
             cl_kernel *kernel);

long cl_calc_fitness(byte *src, byte *ref, int bufsz, 
                  cl_context *context, 
                  cl_command_queue *commandQueue, 
                  cl_kernel *kernel)

#endif /* __OPENCL_H__ */
