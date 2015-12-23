#include "GalaxyCollision.cuh"

__device__ int doPrint = 0;

__device__
float3 bodiesInteraction2(float4 body1, float4 body2, float3 acceleration)
{
    float EPS2 = 0.1f;
    float3 r;

    r.x = body2.x - body1.x;
    r.y = body2.y - body1.y;
    r.z = body2.z - body1.z;

    // distSqr = dot(r_ij, r_ij) + EPS^2  [6 FLOPS]
    float distSqr = sqrtf(r.x * r.x + r.y * r.y + r.z * r.z);
    distSqr *= distSqr;
    distSqr += EPS2;

    // invDistCube =1/distSqr^(3/2)  [4 FLOPS (2 mul, 1 sqrt, 1 inv)]
    float distSixth = distSqr * distSqr * distSqr;
    float invDistCube = 1.0f / sqrtf(distSixth);

    // s = m_j * invDistCube [1 FLOP]
    float s = body2.w * invDistCube;

    acceleration.x += r.x * s;
    acceleration.y += r.y * s;
    acceleration.z += r.z * s;

    return acceleration;
}

__device__
float3 bodiesInteraction(float4 body1, float4 body2, float3 acceleration)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    float EPS2 = 0.01f;
    float ep = 1.0f;

    float3 r;
    r.x = body2.x - body1.x;
    r.y = body2.y - body1.y;
    r.z = body2.z - body1.z;

    float distSqr = (r.x * r.x) + (r.y * r.y) + (r.z * r.z);
    //distSqr *= distSqr;
    distSqr += EPS2;

    float dist = sqrtf(distSqr);
    float distCube = dist * dist * dist;

    float s = (body2.w) / distCube;

    acceleration.x += r.x * s * ep;
    acceleration.y += r.y * s * ep;
    acceleration.z += r.z * s * ep;
/*
    if(tid == 0 && r.y > 10.0)
    {
        doPrint = 99;
        printf("\n");

        printf("body1.w: %lf \n", body1.w);
        printf("body2.w: %lf \n", body2.w);

        printf("\n");

        printf("r.x: %lf \n", r.x);
        printf("r.y: %lf \n", r.y);
        printf("r.z: %lf \n", r.z);

        printf("\n");

        printf("distSqr: %lf \n", distSqr);
        printf("dist: %lf \n", dist);
        printf("distCube: %lf \n", distCube);
        printf("s: %lf \n", s);

        printf("\n");

        printf("acc.x: %lf \n", acceleration.x);
        printf("acc.y: %lf \n", acceleration.y);
        printf("acc.z: %lf \n", acceleration.z);
    }
*/
    return acceleration;
}

__device__
float3 tileAcceleration(float4 currPosition, float3 acceleration)
{
    int i;
    extern __shared__ float4 shPosition[];
    for (i = 0; i < blockDim.x; i++) {
        acceleration = bodiesInteraction(currPosition, shPosition[i], acceleration);
    }
    return acceleration;
}

__global__
void calculateForcesKernel(float4* bodyDescription, float3* acceleration, int size)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < size)
    {
        extern __shared__ float4 shPosition[];

        float4 currPosition;
        float3 acc = { 0.0f, 0.0f, 0.0f };

        int i, tile;

        currPosition = bodyDescription[tid];
        for (i = 0, tile = 0; i < size; i += blockDim.x, tile++)
        {
            int idx = tile * blockDim.x + threadIdx.x;
            if (idx < size){
                shPosition[threadIdx.x] = bodyDescription[idx];
                __syncthreads();
                acc = tileAcceleration(currPosition, acc);
                __syncthreads();
            }

        }

        // Save the result in global memory for the integration step.
        acceleration[tid] = acc;
    }
}

__host__
void galaxyCollisionLogic(float4* d_bodyDescription, float3* d_acceleration, int size)
{
    double4 test;
    dim3 DimGrid((size / MAX_BLOCK_THREAD_COUNT) + 1, 1, 1);
    dim3 DimBlock(MAX_BLOCK_THREAD_COUNT, 1, 1);

    //printf("DimGrid: x: %d, y: %d, z: %d \n", DimGrid.x, DimGrid.y, DimGrid.z);
    //printf("DimBlock: x: %d, y: %d, z: %d \n\n", DimBlock.x, DimBlock.y, DimBlock.z);

    calculateForcesKernel << <DimGrid, DimBlock, MAX_BLOCK_THREAD_COUNT * sizeof(float4)>> >
        (d_bodyDescription, d_acceleration, size);

    cudaDeviceSynchronize();

    doPrint = 0;
}

__host__
void galaxyCollisionInit(float4* bodyDescription, float3* acceleration, int count)
{
    float4* d_bodyDescription;
    float3* d_acceleration;

    int sizef3 = count * sizeof(float3);
    int sizef4 = count * sizeof(float4);

    cudaError_t err;

    /*********** MEMORY ALLOCATION ***********/
    if ((err = cudaMalloc((void**)&d_bodyDescription, sizef4)) != cudaSuccess) C_ERR(err);
    if ((err = cudaMalloc((void**)&d_acceleration, sizef3)) != cudaSuccess) C_ERR(err);

    /*********** COPY MEMORY TO DEVICE ***********/
    if ((err = cudaMemcpy(d_bodyDescription, bodyDescription, sizef4, cudaMemcpyHostToDevice)) != cudaSuccess) C_ERR(err);
    //if ((err = cudaMemcpy(d_acceleration, acceleration, sizef3, cudaMemcpyHostToDevice)) != cudaSuccess) ERR(err);

    // Work
    galaxyCollisionLogic(d_bodyDescription, d_acceleration, count);

    /*********** COPY MEMORY BACK TO HOST ***********/
    if ((err = cudaMemcpy(acceleration, d_acceleration, sizef3, cudaMemcpyDeviceToHost)) != cudaSuccess) C_ERR(err);

    /*********** FREE MEMORY ***********/
    if ((err = cudaFree(d_bodyDescription)) != cudaSuccess) C_ERR(err);
    if ((err = cudaFree(d_acceleration)) != cudaSuccess) C_ERR(err);

    // Update galaxies ...
    /*
    for (int i = 0; i < count; i+=100)
    {
        printf("Acc: %lf, %lf, %lf\n", acceleration[i].x, acceleration[i].y, acceleration[i].z);
    }*/

    // Init the body description - position and mass
}
