#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "kernel.h"
#pragma pack(1)

int main() {
	
	const char *filename = "sweep_output.csv";
    FILE *file = fopen(filename, "r");

    if (file) {
        // File exists
        printf("File '%s' already exists.\n", filename);
        fclose(file);
    } else {
        // File does not exist, create it
        file = fopen(filename, "w");
        if (file) {
            fclose(file);
        } else {
            printf("Failed to create file '%s'.\n", filename);
        }
    }
	
	size_t size, bytes=0;
	double exec_time, gflops;
	int max=100;
	struct timespec start, end;
	bool avx = false;
	
	float a_32 = (float)rand()/(float)(RAND_MAX/max);

	for (int i = 1; i <= 32; i++) {
	
		bytes = pow(2, i);
		size = bytes/4; //f32
		
		float *f32_x = (float*)malloc(size * sizeof(float));
		float *f32_y = (float*)malloc(size * sizeof(float));
		
		for (size_t i = 0; i < size; i++) {
			f32_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f32_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		
		
		if (avx) {
			clock_gettime(CLOCK_MONOTONIC, &start);
			saxpy_avx(size, a_32, f32_x, f32_y);
			clock_gettime(CLOCK_MONOTONIC, &end);
		} else {
			clock_gettime(CLOCK_MONOTONIC, &start);
			saxpy_scalar(size, a_32, f32_x, f32_y);
			clock_gettime(CLOCK_MONOTONIC, &end);
		}
				
		exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
					   (end.tv_nsec - start.tv_nsec);
				
		gflops = (double)(2 * size) / exec_time;
				
		printf("Run %d time to execute: %.2lf ns\n", i, exec_time);
		printf("Run %d GFLOPS: %.2lf\n", i, gflops);
		
		file = fopen(filename, "a");
		fprintf(file, "%d,%.2lf\n",size,gflops);
		fclose(file);
		
		free(f32_x);
		free(f32_y);
	}
	
	return 0;
}