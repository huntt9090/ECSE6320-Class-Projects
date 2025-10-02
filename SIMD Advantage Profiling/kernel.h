#include <stdlib.h>
#include <immintrin.h>

void saxpy_scalar(size_t n, float a, float *x, float *y);
void saxpy_scalar_64(size_t n, double a, double *x, double *y);

void saxpy_avx(size_t n, float a, float *x, float *y);
void saxpy_avx_64(size_t n, double a, double *x, double *y);

float dot_prod_scalar(size_t n, float *x, float *y);
double dot_prod_scalar_64(size_t n, double *x, double *y);

float dot_prod_avx(size_t n, float *x, float *y);
double dot_prod_avx_64(size_t n, double *x, double *y);

void elemult_scalar(size_t n, float *x, float *y, float *z);
void elemult_scalar_64(size_t n, double *x, double *y, double *z);

void elemult_avx(size_t n, float *x, float *y, float *z);
void elemult_avx_64(size_t n, double *x, double *y, double *z);