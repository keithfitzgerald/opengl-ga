__kernel void fitness(__global unsigned char *buf1, 
                      __global unsigned char *buf2, 
                      __local unsigned long *ldata, 
                      __constant unsigned int n, 
                      __global unsigned long *result) {

    unsigned int tid = get_local_id(0);
    unsigned int i = get_global_id(0);

    ldata[tid] = (i < n) ? abs(buf1[i] - buf2[i]) : 0;

    barrier(CLK_LOCAL_MEM_FENCE);

    for(unsigned int s = get_local_size(0) / 2; s > 0; s >>= 1) {
        if (tid < s) {
            ldata[tid] += ldata[tid + s];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (tid == 0) {
        result[get_group_id(0)] = ldata[0];
    }
}
