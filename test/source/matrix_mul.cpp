#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <algorithm>
#include <cstdlib>
#include <doctest/doctest.h>
#include "../include/matrix_multiplication.h"

alignas(64) double A[MATRIX_SIZE][MATRIX_SIZE];
alignas(64) double B[MATRIX_SIZE][MATRIX_SIZE];
alignas(64) double C1[MATRIX_SIZE][MATRIX_SIZE];
alignas(64) double C2[MATRIX_SIZE][MATRIX_SIZE];

void matrix_equal ()
{
  for (int i = 0; i < MATRIX_SIZE; ++i)
  {
    for (int j = 0; j < MATRIX_SIZE; ++j)
    {
      CHECK(C1[i][j] == doctest::Approx(C2[i][j]).epsilon(1e-5));
    }
  }
}

void matrix_init ()
{
  for (int i = 0; i < MATRIX_SIZE; ++i)
  {
    for (int j = 0; j < MATRIX_SIZE; ++j)
    {
      A[i][j] = (double)rand() / (double)RAND_MAX;
      B[i][j] = (double)rand() / (double)RAND_MAX;
      C1[i][j] = 0.0;
      C2[i][j] = 0.0;
    }
  }
}

TEST_CASE("Matrix_multiplication - cache_miss - Correctness")
{
  __itt_pause ();

  matrix_init ();
  Matrix_multiplication mat_mul;
  mat_mul.mkl (C2, A, B);

  __itt_resume ();
  mat_mul.cache_miss (C1, A, B);
  __itt_pause ();

  matrix_equal ();
}

TEST_CASE("Matrix_multiplication - avx2 - Correctness")
{
  __itt_pause ();

  matrix_init ();
  Matrix_multiplication mat_mul;
  mat_mul.mkl (C2, A, B);

  __itt_resume ();
  mat_mul.avx2 (C1, A, B);
  __itt_pause ();

  matrix_equal ();
}

TEST_CASE("Matrix_multiplication - basic - Correctness")
{
  __itt_pause ();

  matrix_init ();
  Matrix_multiplication mat_mul;
  mat_mul.mkl (C2, A, B);

  __itt_resume ();
  mat_mul.basic (C1, A, B);
  __itt_pause ();

  matrix_equal ();
}

TEST_CASE("Matrix_multiplication - omp - Correctness")
{
  __itt_pause ();

  matrix_init ();
  Matrix_multiplication mat_mul;
  mat_mul.mkl (C2, A, B);

  __itt_resume ();
  mat_mul.omp (C1, A, B);
  __itt_pause ();

  matrix_equal ();
}

TEST_CASE("Matrix_multiplication - mkl - Correctness")
{
  __itt_pause ();

  matrix_init ();
  Matrix_multiplication mat_mul;

  __itt_resume ();
  mat_mul.mkl (C2, A, B);
  __itt_pause ();

}




