#pragma once

#include <stdio.h>
#include <mkl.h>
#include <immintrin.h>
#include <ittnotify.h>
#include <iostream>

#include "macros.h"

//#define MATRIX_SIZE 4096

class Matrix_multiplication
{
  public:

    void
      cache_miss
      (double C[MATRIX_SIZE][MATRIX_SIZE]
       , const double A[MATRIX_SIZE][MATRIX_SIZE]
       , const double B[MATRIX_SIZE][MATRIX_SIZE])
      {
        for (int i = 0; i < MATRIX_SIZE; ++i)
        {
          for (int j = 0; j < MATRIX_SIZE; ++j)
          {
            for (int k = 0; k < MATRIX_SIZE; ++k)
            {
              C[i][j] += A[i][k] * B[k][j];
            }
          }
        }
      }

    void
      basic
      (double C[MATRIX_SIZE][MATRIX_SIZE]
       , const double A[MATRIX_SIZE][MATRIX_SIZE]
       , const double B[MATRIX_SIZE][MATRIX_SIZE])
      {
        for (int i = 0; i < MATRIX_SIZE; ++i)
        {
          for (int k = 0; k < MATRIX_SIZE; ++k)
          {
            for (int j = 0; j < MATRIX_SIZE; ++j)
            {
              C[i][j] += A[i][k] * B[k][j];
            }
          }
        }
      }

    void
      prefetch
      (double C[MATRIX_SIZE][MATRIX_SIZE]
       , const double A[MATRIX_SIZE][MATRIX_SIZE]
       , const double B[MATRIX_SIZE][MATRIX_SIZE]
      )
      {

        for (int i = 0; i < MATRIX_SIZE; ++i)
        {
          for (int k = 0; k < MATRIX_SIZE; ++k)
          {
            for (int j = 0; j < MATRIX_SIZE; ++j)
            {
              C[i][j] += A[i][k] * B[k][j];
            }
          }
        }
      }
    void
      omp
      (double C[MATRIX_SIZE][MATRIX_SIZE]
       , const double A[MATRIX_SIZE][MATRIX_SIZE]
       , const double B[MATRIX_SIZE][MATRIX_SIZE]
      )
      {
#pragma omp parallel for  collapse(2)
        for (int i = 0; i < MATRIX_SIZE; ++i)
        {
          for (int j = 0; j < MATRIX_SIZE; ++j)
          {
#pragma omp simd
            for (int k = 0; k < MATRIX_SIZE; ++k)
            {
              C[i][j] += A[i][k] * B[k][j];
            }
          }
        }
      }

    //__builtin_prefetch(&C[i][j + 8], 0, 3);

    void
      avx2
      (double C[MATRIX_SIZE][MATRIX_SIZE]
       , double A[MATRIX_SIZE][MATRIX_SIZE]
       , double B[MATRIX_SIZE][MATRIX_SIZE])
      {

        double B_T[MATRIX_SIZE][MATRIX_SIZE];
#pragma omp parallel for collapse(2)
        for (int i = 0; i < MATRIX_SIZE; ++i)
        {
          for (int j = 0; j < MATRIX_SIZE; ++j)
          {
            // 轉置 B
            B_T[j][i] = B[i][j];
          }
        }

#pragma omp parallel for collapse(2)
        for (int i = 0; i < MATRIX_SIZE; ++i)
        {
          for (int j = 0; j < MATRIX_SIZE; ++j)
          {
            // 同時做 8 個 double 運算，也就是兩 avx2, 8*8*8=512，滿足一個內存行 ( 64 bytes)

            // init, set 0
            __m256d sum = _mm256_setzero_pd();
            __m256d sum2 = _mm256_setzero_pd();

            int k = 0;
            for (; k + 8 < MATRIX_SIZE; k += 8)
            {
              // 0 = read, 3 = to L1
              __builtin_prefetch (&A[i][k + 8], 0, 3);
              __builtin_prefetch (&B_T[j][k + 8], 0, 3);


              // load A[i][k:k+3]
              __m256d vecA = _mm256_loadu_pd (&A[i][k]);
              // load B[j][k:k+3]
              __m256d vecB = _mm256_loadu_pd (&B_T[j][k]);
              // load A[i][k+4:k+7]
              __m256d vecA2 = _mm256_loadu_pd (&A[i][k+4]);
              // load B[j][k+4:k+7]
              __m256d vecB2 = _mm256_loadu_pd (&B_T[j][k+4]);

              // sum = vecA * vecB + sum
              sum = _mm256_fmadd_pd (vecA, vecB, sum);
              sum2 = _mm256_fmadd_pd (vecA2, vecB2, sum2);
            }

            // 把 sum 內的 4 個數據加總，存回 C[i][j]
            double temp[4];
            _mm256_storeu_pd (temp, sum); // store __mm256 to temp[0:3]
            double total = temp[0] + temp[1] + temp[2] + temp[3];
            _mm256_storeu_pd (temp, sum2); // store __mm256 to temp[0:3]
            total += temp[0] + temp[1] + temp[2] + temp[3];

            for (; k < MATRIX_SIZE; ++k)
              total += A[i][k] * B[k][j];

            C[i][j] = total;
          }
        }
      }



    void
      mkl
      (double C[MATRIX_SIZE][MATRIX_SIZE]
       , const double A[MATRIX_SIZE][MATRIX_SIZE]
       , const double B[MATRIX_SIZE][MATRIX_SIZE])
      {
        // [ref](https://www.intel.com/content/www/us/en/docs/onemkl/developer-reference-c/2023-0/cblas-gemm-001.html)
        // formula: C = \alpha * ( A * B ) + beta * C
        // void cblas_cgemm
        // (const CBLAS_LAYOUT Layout // 排列方式
        // , const CBLAS_TRANSPOSE transa // 是否 tr (A)
        // , const CBLAS_TRANSPOSE transb // 是否 tr (B)
      // , const MKL_INT m // size of A's row
      // , const MKL_INT n // size of B's column
      // , const MKL_INT k // size of A's column
      // , const void *alpha // scalar alpha
      // , const void *a // pointer of first data in A
        // , const MKL_INT lda // leading dimension of A (每列的實際儲存間距)
        // , const void *b, const MKL_INT ldb
        // , const void *beta // scalar beta
        // , void *c, const MKL_INT ldc);

        cblas_dgemm
          (CblasRowMajor, CblasNoTrans, CblasNoTrans,
           MATRIX_SIZE, MATRIX_SIZE, MATRIX_SIZE
           , 1.0
           , &A[0][0], MATRIX_SIZE
           , &B[0][0], MATRIX_SIZE
           , 0.0
           , &C[0][0], MATRIX_SIZE);

      }

    void
      print_matrix
      (const double A[MATRIX_SIZE][MATRIX_SIZE])
      {
        for (int i = 0; i < MATRIX_SIZE; ++i)
        {
          for (int j = 0; j < MATRIX_SIZE; ++j)
          {
            std::cout << A[i][j] << ' ';
          }
          std::cout << '\n';
        }
      }

};




