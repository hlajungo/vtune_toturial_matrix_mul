
#include "./include/matrix_multiplication.h"  



int main()
{
  double A[MATRIX_SIZE][MATRIX_SIZE];
  double B[MATRIX_SIZE][MATRIX_SIZE];
  double C1[MATRIX_SIZE][MATRIX_SIZE];
  double C2[MATRIX_SIZE][MATRIX_SIZE];

  for (int i = 0; i < MATRIX_SIZE; ++i)
  {
    for (int j = 0; j < MATRIX_SIZE; ++j)
    {
      A[i][j] = (double)rand() / (double)RAND_MAX;
      B[i][j] = (double)rand() / (double)RAND_MAX;
      C1[i][j] = 0;
      C2[i][j] = 0;
    }
  }

  Matrix_multiplication mat_mul;

  //printf ("TEST - MATRIX_SIZE=%d\n", MATRIX_SIZE);
  //mat_mul.cache_miss (C1, A, B);
  mat_mul.cache_miss (C2, A, B);
}
