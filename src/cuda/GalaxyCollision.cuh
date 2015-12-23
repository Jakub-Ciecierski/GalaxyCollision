#ifndef _SHORTEST_CUDA_H_
#define _SHORTEST_CUDA_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#define MAX_GRID_DIM_SIZE 65535

#define MAX_BLOCK_THREAD_COUNT 512

#define MAX_BLOCK_DIM_SIZE 32

#define C_ERR(err) (printf("%s in %s at line %d\n", cudaGetErrorString(err), __FILE__, __LINE__),\
                    exit(EXIT_FAILURE))

__device__
float3 bodiesInteraction(float4 body1, float4 body2, float3 acceleration);

__device__
float3 tileAcceleration(float4 currPosition, float3 acceleration);

__global__
void calculateForcesKernel(float4* bodyDescription, float3* acceleration, int n);

__host__
void galaxyCollisionLogic(float4* d_bodyDescription, float3* d_acceleration, int n);

__host__
void galaxyCollisionInit(float4* bodyDescription, float3* acceleration, int count);

#endif
