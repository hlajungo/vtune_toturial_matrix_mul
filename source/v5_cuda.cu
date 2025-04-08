#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <sys/time.h>

float tdiff(struct timeval *start, struct timeval *end)
{
  return (end->tv_sec - start->tv_sec) + 
    1e-6 * (end->tv_usec - start->tv_usec);
}


#define N 4096  // 矩陣大小

// CUDA 核心函數
__global__ void matrixMulKernel(double *A, double *B, double *C, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < n) {
        double sum = 0;
        for (int k = 0; k < n; ++k) {
            sum += A[row * n + k] * B[k * n + col];
        }
        C[row * n + col] = sum;
    }
}

int main() {
    // 分配主機端記憶體
    double *h_A = (double *)malloc(N * N * sizeof(double));
    double *h_B = (double *)malloc(N * N * sizeof(double));
    double *h_C = (double *)malloc(N * N * sizeof(double));

    // 初始化矩陣
    for (int i = 0; i < N * N; ++i) {
        h_A[i] = (double)rand() / RAND_MAX;
        h_B[i] = (double)rand() / RAND_MAX;
    }

  struct timeval start2, end2;
  gettimeofday(&start2, NULL);


    // 分配裝置端記憶體
    double *d_A, *d_B, *d_C;
    cudaMalloc((void **)&d_A, N * N * sizeof(double));
    cudaMalloc((void **)&d_B, N * N * sizeof(double));
    cudaMalloc((void **)&d_C, N * N * sizeof(double));

    // 將資料從主機傳輸到裝置
    cudaMemcpy(d_A, h_A, N * N * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, N * N * sizeof(double), cudaMemcpyHostToDevice);

    // 定義 CUDA 核心的執行配置
    dim3 threadsPerBlock(16, 16);
    dim3 blocksPerGrid((N + threadsPerBlock.x - 1) / threadsPerBlock.x, 
                       (N + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // 計時開始
    cudaEvent_t start, end;
    cudaEventCreate(&start);
    cudaEventCreate(&end);
    cudaEventRecord(start);

    // 執行 CUDA 核心
    matrixMulKernel<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);

    // 計時結束
    cudaEventRecord(end);
    cudaEventSynchronize(end);

    // 計算執行時間
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, end);
    printf("Execution time: %0.6f ms\n", milliseconds);

    // 將結果從裝置傳回主機
    cudaMemcpy(h_C, d_C, N * N * sizeof(double), cudaMemcpyDeviceToHost);

    // 釋放記憶體
    free(h_A);
    free(h_B);
    free(h_C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

  gettimeofday(&end2, NULL);
  printf("%0.6f\n", tdiff(&start2, &end2));


    return 0;
}

