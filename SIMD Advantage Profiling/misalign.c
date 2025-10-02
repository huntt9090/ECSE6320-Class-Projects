#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "kernel.h"

int main() {
	
	const char *filename = "saxpy_alignment.csv";
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
	
	size_t offset, size, bytes=0;
	double exec_time, gflops;
	int max=100;
	struct timespec start, end;
	bool avx = true;
	
	bytes = pow(2, 24);
	size = bytes/4; //f32
	
	float a_32 = (float)rand()/(float)(RAND_MAX/max);
	
	for (int i = 0; i < 60; i++) {
		
		offset = i;
		
		void *x_raw = malloc(size * sizeof(float) + offset);
		void *y_raw = malloc(size * sizeof(float) + offset); //1 byte extra
			
		float *f32_x = (float *)((uint8_t *)x_raw + offset);
		float *f32_y = (float *)((uint8_t *)y_raw + offset);
		
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
		fprintf(file, "%d,%.2lf\n",offset,gflops);
		fclose(file);
		
		free(x_raw);
		free(y_raw);
	}
	
	return 0;
}