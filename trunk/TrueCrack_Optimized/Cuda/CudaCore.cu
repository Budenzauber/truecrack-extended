/*
 * Changes made 2012 Copyright (C)  Bastian Blankemeier
 * 									Henryk Jaskowiak
 *									Benjamin Meis
 *
 * Extending the original licensing of TrueCrack
 *
 *
 * TrueCrack Copyright (C) Luca Vaccaro
 *
 * TrueCrack is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "Tcdefs.h"
#include "Volumes.h"
#include <stdio.h>
#include <memory.h>
#include "Crypto.h"
#include "CudaCore.cuh"
#include "CudaPkcs5.cuh"
#include "CudaXts.cuh"



/* The max number of block grid; number of max parallel gpu blocks. */
int blockGridSizeMax;

/* The number of the current block grid; number of current parallel gpu blocks. */
int blockGridSizeCurrent;

/* Pointer of structures to pass to Cuda Kernel. */
unsigned char *dev_salt, *dev_blockPwd, *dev_header;
int *dev_blockPwd_init, *dev_blockPwd_length;
short int *dev_result;


int getMultiprocessorCount (int device_number){
	cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop,device_number); // 2nd parameter is the device number
	return prop.multiProcessorCount;
}

__global__ void cuda_Kernel ( unsigned char *salt, unsigned char *headerEncrypted, unsigned char *blockPwd, int *blockPwd_init, int *blockPwd_length, short int *result, short int GPU_number, short int device_modulo, int offset) {

  	int numData=threadIdx.x + blockIdx.x * 512;
  	numData += offset; // Add offset

	// Array of unsigned char in the shared memory
	if (numData%device_modulo == GPU_number)
	{
	    __align__(8) unsigned char headerkey[192];
	    __align__(8) unsigned char headerDecrypted[512];
	
		// Calculate the hash header key
		for(int i = 0; i < 10; i++)
		{
			cuda_Pbkdf2 (salt, blockPwd, blockPwd_init, blockPwd_length, headerkey, numData, i);
		}
		
		// Decrypt the header and compare the key
	    
	    int value;
	    value=cuda_Xts (headerEncrypted, headerkey, headerDecrypted);
	
	    if (value==SUCCESS)
	        result[numData]=MATCH;
	    else
	        result[numData]=NOMATCH;
	}
}

void cuda_Core ( short int *result, short int GPU_number, short int device_modulo) {

	cudaSetDevice(GPU_number);
	cudaMalloc ( &dev_result, blockGridSizeCurrent * sizeof(short int)) ;
    cudaMemcpy( dev_result, result, blockGridSizeCurrent * sizeof(short int) , cudaMemcpyHostToDevice) ;

	int threadCount = 512;
	int gridSize = blockGridSizeCurrent / threadCount;
	cuda_Kernel<<<gridSize, threadCount>>>(dev_salt, dev_header, dev_blockPwd, dev_blockPwd_init, dev_blockPwd_length, dev_result, GPU_number, device_modulo, 0);

	if (blockGridSizeCurrent % threadCount != 0)
	{
		int offset = gridSize * threadCount;
		gridSize = 1;
		threadCount = blockGridSizeCurrent % threadCount;
		cudaThreadSynchronize(); // Synchronize all Threads before starting next calculations
		cuda_Kernel<<<gridSize, threadCount>>>(dev_salt, dev_header, dev_blockPwd, dev_blockPwd_init, dev_blockPwd_length, dev_result, GPU_number, device_modulo, offset);
	}

    //cudaThreadSynchronize();
    cudaMemcpy(result, dev_result,blockGridSizeCurrent * sizeof(short int) , cudaMemcpyDeviceToHost) ;
}

/*The reference manual lists all the various functions used to copy memory between 
linear memory allocated with cudaMalloc(), linear memory allocated with 
cudaMallocPitch() or cudaMalloc3D(), CUDA arrays, and memory 
allocated for variables declared in global or constant memory space.
The following code sample illustrates various ways of accessing global variables via 
the runtime API:
__constant__ float constData[256];
float data[256];
cudaMemcpyToSymbol(constData, data, sizeof(data));
cudaMemcpyFromSymbol(data, constData, sizeof(data));
__device__ float devData;
float value = 3.14f;
cudaMemcpyToSymbol(devData, &value, sizeof(float));
__device__ float* devPointer;
float* ptr;
cudaMalloc(&ptr, 256 * sizeof(float));
cudaMemcpyToSymbol(devPointer, &ptr, sizeof(ptr));
cudaGetSymbolAddress() is used to retrieve the address pointing to the 
memory allocated for a variable declared in global memory space. The size of the 
allocated memory is obtained through cudaGetSymbolSize().*/

void cuda_Init (int block_maxsize, unsigned char *salt, unsigned char *header, int device_number) {
    blockGridSizeMax=block_maxsize;
    cudaSetDevice(device_number);
    cudaMalloc ( (void **)&dev_blockPwd, blockGridSizeMax * PASSWORD_MAXSIZE * sizeof(unsigned char)) ;
    cudaMalloc ( (void **)&dev_blockPwd_init, blockGridSizeMax * sizeof(int)) ;
    cudaMalloc ( (void **)&dev_blockPwd_length, blockGridSizeMax * sizeof(int)) ;
    cudaMalloc ( (void **)&dev_salt, SALT_LENGTH * sizeof(unsigned char)) ;
    cudaMalloc ( (void **)&dev_header, TC_VOLUME_HEADER_EFFECTIVE_SIZE * sizeof(unsigned char)) ;
    cudaMalloc ( (void **)&dev_result, blockGridSizeMax * sizeof(short int)) ;

    cudaMemcpy(dev_salt, salt, SALT_LENGTH * sizeof(unsigned char) , cudaMemcpyHostToDevice) ;
    cudaMemcpy(dev_header, header, TC_VOLUME_HEADER_EFFECTIVE_SIZE * sizeof(unsigned char) , cudaMemcpyHostToDevice) ;

}

void cuda_Set (	int block_currentsize, unsigned char *blockPwd, int *blockPwd_init, int *blockPwd_length, short int *result, int device_number) {

	cudaSetDevice(device_number);
	blockGridSizeCurrent=block_currentsize;
	int lengthpwd=0,i;
	for (i=0;i<blockGridSizeCurrent;i++) {
		lengthpwd+=blockPwd_length[i];
		result[i]=NODEFINED;
	}

	cudaMemcpy(dev_blockPwd, blockPwd, lengthpwd * sizeof(unsigned char) , cudaMemcpyHostToDevice) ;
	cudaMemcpy(dev_blockPwd_init, blockPwd_init, blockGridSizeCurrent * sizeof(int) , cudaMemcpyHostToDevice) ;
	cudaMemcpy(dev_blockPwd_length, blockPwd_length, blockGridSizeCurrent * sizeof(int) , cudaMemcpyHostToDevice) ;
	cudaMemcpy(dev_result, result, blockGridSizeCurrent * sizeof(short int) , cudaMemcpyHostToDevice) ;
}


void cuda_Free(void) {
    cudaFree(dev_salt);
    cudaFree(dev_blockPwd);
    cudaFree(dev_blockPwd_init);
    cudaFree(dev_blockPwd_length);
    cudaFree(dev_result);
    cudaFree(dev_header);
}
