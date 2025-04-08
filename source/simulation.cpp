#include <stdlib.h>
#include <immintrin.h>
#include "ittnotify.h"
#include <mkl.h>
#define MATRIX_SIZE 4096

double A[MATRIX_SIZE][MATRIX_SIZE];
double B[MATRIX_SIZE][MATRIX_SIZE];
double C[MATRIX_SIZE][MATRIX_SIZE];

// cache_line_size / double_size = 64/8 = 8 個數據在同一 cache line 裡。
// avx2 一次讀 32 bytes，也就是 4 double 數據。




int main(int argc, const char *argv[]) {
  __itt_pause ();
  for (int i = 0; i < MATRIX_SIZE; ++i) {
    for (int j = 0; j < MATRIX_SIZE; ++j) {
      A[i][j] = (double)rand() / (double)RAND_MAX;
      B[i][j] = (double)rand() / (double)RAND_MAX;
      C[i][j] = 0;
    }
  }

  int size = MATRIX_SIZE;
  __itt_resume ();
  //matrix_mult (A, B, C, size);
  matrix_mult_avx2 (A, B, C, size);
  //matmul_mkl();
  __itt_pause ();


  return 0;
}

/*
#include <immintrin.h>
#include <omp.h>

void matrix_mult_avx2(double A[MATRIX_SIZE][MATRIX_SIZE],
                      double B[MATRIX_SIZE][MATRIX_SIZE],
                      double C[MATRIX_SIZE][MATRIX_SIZE],
                      int matrix_size) {
    // ** 先將 B 轉置，優化 cache 效率 **
    static double B_T[MATRIX_SIZE][MATRIX_SIZE];
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            B_T[j][i] = B[i][j]; // 轉置 B，使訪問 B_T[k][j] 成為順序訪問
        }
    }

    // ** 開始矩陣乘法 **
    #pragma omp parallel for collapse(2) reduction(+:C[:MATRIX_SIZE][:MATRIX_SIZE])
    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            // ** 初始化 sum **
            __m256d sum = _mm256_setzero_pd(); 
            __m256d sum2 = _mm256_setzero_pd();
            
            for (int k = 0; k < matrix_size; k += 8) {
                // ** 預取，避免超出界限 **
                if (k + 8 < matrix_size) {
                    __builtin_prefetch(&A[i][k + 8], 0, 3);
                    __builtin_prefetch(&B_T[j][k + 8], 0, 3);
                }

                // ** 加載 A[i][k:k+3] 和 A[i][k+4:k+7] **
                __m256d vecA = _mm256_loadu_pd(&A[i][k]);
                __m256d vecA2 = _mm256_loadu_pd(&A[i][k + 4]);

                // ** 加載轉置後的 B_T[j][k:k+3] 和 B_T[j][k+4:k+7] **
                __m256d vecB = _mm256_loadu_pd(&B_T[j][k]);
                __m256d vecB2 = _mm256_loadu_pd(&B_T[j][k + 4]);

                // ** sum = A * B + sum (AVX2 FMA) **
                sum = _mm256_fmadd_pd(vecA, vecB, sum);
                sum2 = _mm256_fmadd_pd(vecA2, vecB2, sum2);
            }

            // ** 把 sum 存入 temp 並累加 **
            double temp[4];
            _mm256_storeu_pd(temp, sum);
            C[i][j] = temp[0] + temp[1] + temp[2] + temp[3];

            _mm256_storeu_pd(temp, sum2);
            C[i][j] += temp[0] + temp[1] + temp[2] + temp[3];
        }
    }
}
*/


