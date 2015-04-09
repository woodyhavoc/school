/**
* Author: Aaron Woods
* Class: CS497
* Assignment: P1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
* This device function accepts an integer array filled with data from an
* image file.  Each index of the array is looped through, and if the
* value at the current index is equal to the index variable, it is set to 1,
* otherwise it is set to 0. This creates a binary array, where the index value
* at the indicies containig a one are the integer values of the various
* colors in the image file.
*
* @param input - The array to be converted into a binary array.
* @param outut - The array in which the binary information will be stored.
* @param maxCount - The size of the input array.
* @param stepSize - The amount by which each thread is incremented on
* 					each iteration of the loop.
*/
__global__ void fixInput(int* input, int* output, int maxCount, int stepSize);

/**
* This device function accepts an integer array which will have been converted
* to a binary array in the previous device function.  A prefix sum is performed
* on the array.  At the end of the prefix sum process, the sum of all the values
* in the array will be stored in index 0.
* 
* @param input - The array, which contains only 0's and 1's.
* @param maxCount - The size of the input array.
* @param stepSize - The amount by which each thread is incrememnted on
* 					each iteration of the loop. 
*/
__global__ void prefixSum(int* input, int maxCount, int stepSize);

/**
* This device function accepts an integer array which contains the prefix sum
* information generated in the prefixSum() function.  The array is iterated 
* through and each time that a value at a particular index is not equal to the value 
* of the index which came before it, the value maxCount - the index is stored in
* the output array at the index represented by the value at the current index of
* the input array.  A more clear depiction is given below:
*
* 			output[input[tid]] = maxCount - tid;
* 
* @param input - The array which contains the prefix sum information.
* @param output - The array which the unique color information will be
*				  stored in.
* @param maxCount - The size of the input array.
* @param stepSize - The amount by which each thread is incremented on each
*					each iteration of the loop.
*/
__global__ void fillArray(int* input, int* output, int maxCount, int stepSize);

/**
* Fills the output array sequentially with the image color region
* information.
* 
* @param input - The array that contains the binary image color region
* 				 information.
* @param output - The array that will hold the actual integer values of the
*				  various color regions.
* @param - arraySize - The size of the output array. 
*/
void seqFill(int* input, int* output, int arraySize);

/**
* Prints the results of the image color mapping.  The number of regions
* is first printed, followed by the 50 regions with the smallest value.
*
* @param input - The array that contains the image color mapping information.
* @param arraySize - The size of the input array.
*/
void printResults(int* input, int arraySize);

// Block and thread dimensions
const dim3 NUM_BLOCKS(1);
const dim3 NUM_THREADS(256);

/**
* Main function, calls the appropriate functions to evaluate the image
* color region mapping.
* 
* @param argc - The number of command-line arguments including the program name.
* @param argv - The string value of each command-line argument including
*				the program name.
*/
int main(int argc, char** argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Incorrect number of arguments.\nUsage: <program name> <image file>\n");
		exit(1);
	}

	int* devColorArray, *devFileData, *devFillArray;
	int* colorArray = NULL;
	int* fileData = NULL;
	int* fill = NULL;
		
	FILE* pFile = fopen(argv[1], "rb");
	
	int sizeInt = sizeof(int);
	int stepSize = NUM_BLOCKS.x * NUM_THREADS.x;
	int height, width, sizeFile, maxCount, numRegions, i;	
	
	if(pFile == NULL)
	{
		fprintf(stderr, "File error.\n");
		exit(1);
	}

	fread(&height, sizeInt, 1, pFile);
	fread(&width, sizeInt, 1, pFile);

	sizeFile = width * height * sizeInt;
	maxCount = width * height;
	
	fileData = (int*)malloc(sizeFile + sizeInt);
	
	fread(fileData, sizeInt, width*height, pFile);
	
	cudaMalloc((void**)&devColorArray, sizeFile);
	cudaMalloc((void**)&devFileData, sizeFile);
		
	cudaMemcpy(devFileData, fileData, sizeFile, cudaMemcpyHostToDevice);
	
	fixInput<<<NUM_BLOCKS, NUM_THREADS>>>(devFileData, devColorArray, maxCount, stepSize);
	prefixSum<<<NUM_BLOCKS, NUM_THREADS>>>(devFileData, maxCount, stepSize);

	cudaMemcpy(fileData, devFileData, sizeFile, cudaMemcpyDeviceToHost);
	cudaMemcpy(colorArray, devColorArray, sizeFile, cudaMemcpyDeviceToHost);

	numRegions = fileData[0];

	fill = (int*)malloc(numRegions * sizeInt);
	cudaMalloc((void**)&devFillArray, numRegions * sizeInt);
	cudaMemset(devFillArray, 0, numRegions * sizeInt);

	/*
	* The multi-threaded portion to fill the output
	* array would not work, so that is why there is also
	* a sequential function that populates the output array.
	* I left the multi-threaded function so you could see
	* what I was attempting to do.
	*/

	//fillArray<<<NUM_BLOCKS, NUM_THREADS>>>(devFileData, devFillArray, maxCount, stepSize);
	
	//cudaMemcpy(fill, devFillArray, numRegions * sizeInt, cudaMemcpyDeviceToHost);

	seqFill(colorArray, fill, maxCount, numRegions);

	printResults(fill, numRegions);

	return 0;
}

__global__ void fixInput(int* input, int* output, int maxCount, int stepSize)
{
	int tid;

	for(tid = blockIdx.x * blockDim.x + threadIdx.x; tid < maxCount; tid += stepSize)
	{
		input[tid] = input[tid] == tid ? 1 : 0;
		output[tid] = input[tid];
	}

	__syncthreads();
}

__global__ void prefixSum(int* input, int maxCount, int stepSize)
{
	int threadIndex = blockIdx.x * blockDim.x + threadIdx.x;
	int i = 1;
	int tid = threadIndex;
	
	while(i < maxCount)
	{
		while(tid + i < maxCount)
		{
			input[tid] += input[tid + i];
			tid += stepSize;		
		}

		__syncthreads();
		i <<= 1;
		tid = threadIndex;
	}

	__syncthreads();	
}

__global__ void fillArray(int* input, int* output, int maxCount, int stepSize)
{
	int tid;

	for(tid = maxCount - (blockIdx.x * blockDim.x + threadIdx.x) - 1; tid >= 0; tid -= stepSize)
	{
		if(input[tid] != input[tid-1])
		{
			output[input[tid]] = maxCount - tid;
		}
	}

	__syncthreads();
}

void seqFill(int* input, int* output, int maxCount, int arraySize)
{
	int i, index = 0;

	for(i = 0; i < maxCount; i++)
	{
		if(input[i] == 1)
		{
			output[index++] = i;
		}
	}

	if(index != arraySize)
	{
		fprintf(stderr, "Actual output array size: %d != expected output array size: %d\n",
		index, arraySize);

		exit(1);
	}
}

void printResults(int* input, int arraySize)
{
	printf("\n\nNumber of regions in image: %d\n", arraySize);
	printf("The first 50 regions are:\n");

	int i;

	for(i = 0; i < 50; i++)
	{
		printf("%d\t", input[i]);

		if((i+1) % 5 == 0)
		{
			printf("\n");
		}
	}
}















