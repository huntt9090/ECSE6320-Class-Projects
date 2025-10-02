#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "kernel.h"
#pragma pack(1)

int main(int argc, char* argv[]) {
	
	size_t size, bytes=0;
	double exec_time, gflops;
	int runs, max=100;
	struct timespec start, end;
	char type;
	bool avx = false;
	
	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i],"--bytes")) {
			bytes = atoi(argv[i+1]);
			continue;
		} else if (!strcmp(argv[i],"--runs")) {
			runs = atoi(argv[i+1]);
			continue;
		} else if (!strcmp(argv[i],"--type")) {
			if (bytes == 0){
				printf("Err: --bytes must be defined before --type can be called.\n");
				exit(1);
			}
			if (!strcmp(argv[i+1],"f32")) {
				size = bytes / 4;
				type = 'f';
			} else if (!strcmp(argv[i+1],"f64")) {
				size = bytes / 8;
				type = 'd';
			} else {
				printf("Invalid --type arg!\n");
				exit(1);
			}
			continue;
		} else if (!strcmp(argv[i],"--avx-enable")) {
			avx = true;
			continue;
		} else {
			continue;
		}
	}

	printf("bytes: %d, runs: %d, size: %d\n", bytes, runs, size);
	printf("Generating arrays of size %d...\n", size);
	
	const char *filename = "output.csv";
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
	
	if ((type == 'f') && !avx) {
		
		float a_32 = (float)rand()/(float)(RAND_MAX/max);
		
		float *f32_x = (float*)malloc(size * sizeof(float));
		float *f32_y = (float*)malloc(size * sizeof(float));
		float *f32_z = (float*)malloc(size * sizeof(float));
		
		for (size_t i = 0; i < size; i++) {
			f32_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f32_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		
		printf("Running baseline f32 tests...\n");
		printf("\n==================SAXPY-Scalar=================\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			saxpy_scalar(size, a_32, f32_x, f32_y);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "SAXPY,AVX=FALSE,DATA=F32,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		for (size_t i = 0; i < size; i++) {
			f32_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f32_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		printf("\n===============Dot-Product-Scalar==============\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			float sum = dot_prod_scalar(size, f32_x, f32_y);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "DotProd,AVX=FALSE,DATA=F32,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		for (size_t i = 0; i < size; i++) {
			f32_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f32_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		printf("\n==========Elementwise-Multiply-Scalar==========\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			elemult_scalar(size, f32_x, f32_y, f32_z);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "EleMult,AVX=FALSE,DATA=F32,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		free(f32_x);
		free(f32_y);
		free(f32_z);
		
	} else if ((type == 'd') && !avx) {
		
		double a_64 = (double)rand()/(double)(RAND_MAX/max);
		
		double *f64_x = (double*)malloc(size * sizeof(double));
		double *f64_y = (double*)malloc(size * sizeof(double));
		double *f64_z = (double*)malloc(size * sizeof(double));
		
		for (size_t i = 0; i < size; i++) {
			f64_x[i] = (double)rand()/(double)(RAND_MAX/max);
			f64_y[i] = (double)rand()/(double)(RAND_MAX/max);
		}
		
		printf("Running baseline f64 tests...\n");
		printf("\n==================SAXPY-Scalar=================\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			saxpy_scalar_64(size, a_64, f64_x, f64_y);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "SAXPY,AVX=FALSE,DATA=F64,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		for (size_t i = 0; i < size; i++) {
			f64_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f64_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		printf("\n===============Dot-Product-Scalar==============\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			float sum = dot_prod_scalar_64(size, f64_x, f64_y);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "DotProd,AVX=FALSE,DATA=F64,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		for (size_t i = 0; i < size; i++) {
			f64_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f64_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		printf("\n==========Elementwise-Multiply-Scalar==========\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			elemult_scalar_64(size, f64_x, f64_y, f64_z);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "EleMult,AVX=FALSE,DATA=F64,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		free(f64_x);
		free(f64_y);
		free(f64_z);
		
	} else if ((type == 'f') && avx) {
		
		float a_32 = (float)rand()/(float)(RAND_MAX/max);
		
		float *f32_x = (float*)malloc(size * sizeof(float));
		float *f32_y = (float*)malloc(size * sizeof(float));
		float *f32_z = (float*)malloc(size * sizeof(float));
		
		for (size_t i = 0; i < size; i++) {
			f32_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f32_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		
		printf("Running vectorized f32 tests...\n");
		printf("\n==================SAXPY-AVX=================\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			saxpy_avx(size, a_32, f32_x, f32_y);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "SAXPY,AVX=TRUE,DATA=F32,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		for (size_t i = 0; i < size; i++) {
			f32_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f32_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		printf("\n===============Dot-Product-AVX==============\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			float sum = dot_prod_avx(size, f32_x, f32_y);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "DotProd,AVX=TRUE,DATA=F32,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		for (size_t i = 0; i < size; i++) {
			f32_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f32_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		printf("\n==========Elementwise-Multiply-AVX==========\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			elemult_avx(size, f32_x, f32_y, f32_z);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);

			file = fopen(filename, "a");
			fprintf(file, "EleMult,AVX=TRUE,DATA=F32,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		free(f32_x);
		free(f32_y);
		free(f32_z);
		
	} else if ((type == 'd') && avx) {
		
		double a_64 = (double)rand()/(double)(RAND_MAX/max);
		
		double *f64_x = (double*)malloc(size * sizeof(double));
		double *f64_y = (double*)malloc(size * sizeof(double));
		double *f64_z = (double*)malloc(size * sizeof(double));
		
		for (size_t i = 0; i < size; i++) {
			f64_x[i] = (double)rand()/(double)(RAND_MAX/max);
			f64_y[i] = (double)rand()/(double)(RAND_MAX/max);
		}
		
		printf("Running vectorized f64 tests...\n");
		printf("\n==================SAXPY-AVX=================\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			saxpy_avx_64(size, a_64, f64_x, f64_y);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "SAXPY,AVX=TRUE,DATA=F64,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		for (size_t i = 0; i < size; i++) {
			f64_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f64_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		printf("\n===============Dot-Product-AVX==============\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			float sum = dot_prod_avx_64(size, f64_x, f64_y);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "DotProd,AVX=TRUE,DATA=F64,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		for (size_t i = 0; i < size; i++) {
			f64_x[i] = (float)rand()/(float)(RAND_MAX/max);
			f64_y[i] = (float)rand()/(float)(RAND_MAX/max);
		}
		printf("\n==========Elementwise-Multiply-AVX==========\n");
		for (int i = 0; i < runs; i++) {

			clock_gettime(CLOCK_MONOTONIC, &start);
			elemult_avx_64(size, f64_x, f64_y, f64_z);
			clock_gettime(CLOCK_MONOTONIC, &end);
			
			exec_time = (end.tv_sec - start.tv_sec) * 1000000000.0 + 
						   (end.tv_nsec - start.tv_nsec);
			
			gflops = (double)(2 * size) / exec_time;
			
			printf("Run %d time to execute: %.2lf ns\n", i+1, exec_time);
			printf("Run %d GFLOPS: %.2lf\n", i+1, gflops);
			
			file = fopen(filename, "a");
			fprintf(file, "EleMult,AVX=TRUE,DATA=F64,%lu,%lu,%d,%.2lf,%.2lf\n",bytes,size,i+1,exec_time,gflops);
			fclose(file);
		}
		
		free(f64_x);
		free(f64_y);
		free(f64_z);
		
	} else {
		printf("Err: no valid array type found!\n");
		exit(1);
	}
	
	/*
	
	//L1_kb = 16 -> size= (16*1024)/4 = 16384 (f32);
	//L2_kb = 1 * 1024 -> size= (1024*1024)/4 = 262144 (f32);
	//L3_kb = 96 * 1024 -> size= (98304*1024)/4 = 25165824 (f32);
	
	char pause;
	scanf("%c\n", &pause);
	
	free(f64_x);
	free(f64_y);
	free(f64_z);
	*/
	
	return 0;
}