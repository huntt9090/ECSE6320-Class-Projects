#include "kernel.h"

void saxpy_scalar(size_t n, float a, float *x, float *y) {
	for (size_t i=0; i < n; i++) {
		y[i] = a * x[i] + y[i]; // y <- ax + y
	}
}

void saxpy_scalar_64(size_t n, double a, double *x, double *y) {
	for (size_t i=0; i < n; i++) {
		y[i] = a * x[i] + y[i]; // y <- ax + y
	}
}

void saxpy_avx(size_t n, float a, float *x, float *y) {
    __m256 a_vec = _mm256_set1_ps(a);  // Set scalar 'a' to all elements of the vector
    for (size_t i = 0; i < n; i += 8) {
        __m256 x_vec = _mm256_load_ps(&x[i]);  // Load 8 elements from x
        __m256 y_vec = _mm256_load_ps(&y[i]);  // Load 8 elements from y
        __m256 result = _mm256_fmadd_ps(a_vec, x_vec, y_vec);  // Fused Multiply-Add
        _mm256_store_ps(&y[i], result);  // Store result back into y
    }
}

void saxpy_avx_64(size_t n, double a, double *x, double *y) {
    __m256d a_vec = _mm256_set1_pd(a);  // Set scalar 'a' to all elements of the vector
    for (size_t i = 0; i < n; i += 4) {
        __m256d x_vec = _mm256_load_pd(&x[i]);  // Load 8 elements from x
        __m256d y_vec = _mm256_load_pd(&y[i]);  // Load 8 elements from y
        __m256d result = _mm256_fmadd_pd(a_vec, x_vec, y_vec);  // Fused Multiply-Add
        _mm256_store_pd(&y[i], result);  // Store result back into y
    }
}

float dot_prod_scalar(size_t n, float *x, float *y) {
	float sum = 0.;
	for (size_t i = 0; i < n; i++) {
		sum += (x[i] * y[i]);
	}
	return sum;
}

double dot_prod_scalar_64(size_t n, double *x, double *y) {
	double sum = 0.;
	for (size_t i = 0; i < n; i++) {
		sum += (x[i] * y[i]);
	}
	return sum;
}

float dot_prod_avx(size_t n, float *x, float *y) {
	__m256 vsum = _mm256_setzero_ps();
	for (size_t i = 0; i < n; i += 8) {
		__m256 x_vec = _mm256_load_ps(&x[i]);
		__m256 y_vec = _mm256_load_ps(&y[i]);
		vsum = _mm256_fmadd_ps(x_vec, y_vec, vsum);
	}
	
	float buffer[8];
	_mm256_store_ps(buffer, vsum);
	float sum = buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4]
				+ buffer[5] + buffer[6] + buffer[7];
	
	return sum;
}

double dot_prod_avx_64(size_t n, double *x, double *y) {
	__m256d vsum = _mm256_setzero_pd();
	for (size_t i = 0; i < n; i += 4) {
		__m256d x_vec = _mm256_load_pd(&x[i]);
		__m256d y_vec = _mm256_load_pd(&y[i]);
		vsum = _mm256_fmadd_pd(x_vec, y_vec, vsum);
	}
	
	double buffer[4];
	_mm256_store_pd(buffer, vsum);
	double sum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
	return sum;
}

void elemult_scalar(size_t n, float *x, float *y, float *z) {
	for(size_t i = 0; i < n; i++) {
		z[i] = x[i] + y[i];
	}
}

void elemult_scalar_64(size_t n, double *x, double *y, double *z) {
	for(size_t i = 0; i < n; i++) {
		z[i] = x[i] + y[i];
	}
}

void elemult_avx(size_t n, float *x, float *y, float *z) {
	for(size_t i = 0; i < n; i += 8) {
		__m256 x_vec = _mm256_load_ps(&x[i]);
		__m256 y_vec = _mm256_load_ps(&y[i]);
		__m256 result = _mm256_add_ps(x_vec, y_vec);
		_mm256_store_ps(&z[i], result);
	}
}

void elemult_avx_64(size_t n, double *x, double *y, double *z) {
	for(size_t i = 0; i < n; i += 4) {
		__m256d x_vec = _mm256_load_pd(&x[i]);
		__m256d y_vec = _mm256_load_pd(&y[i]);
		__m256d result = _mm256_add_pd(x_vec, y_vec);
		_mm256_store_pd(&z[i], result);
	}
}