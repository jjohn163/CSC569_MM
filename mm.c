#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define M1_HEIGHT       3200
#define M1_WIDTH        3200
#define M2_HEIGHT       3200
#define M2_WIDTH        3200

#define RESULT_LEN      M1_HEIGHT * M2_WIDTH
#define MAX_CELL_MAG    100
#define TILE_SIZE       64
int mat1[M1_WIDTH * M1_HEIGHT];
int mat2[M2_WIDTH * M2_HEIGHT];
int mat2T[M2_WIDTH * M2_HEIGHT];


int result[RESULT_LEN];
int solution[RESULT_LEN];
int numThreads;

void clear(int *array, int size){
   int i;
   for (i = 0; i < size; i++) {
      array[i] = 0;
   }
}
int verify() {
   int row, col;

   for (row = 0; row < M1_HEIGHT; row++) {
      for (col = 0; col < M2_WIDTH; col++) {
         if (result[row*M2_WIDTH+col] != solution[row*M2_WIDTH+col]) {
            printf("**** row: %d col: %d ****\n", row, col);
            printf("Expected: %d\n", solution[row*M2_WIDTH+col]);
            printf("Actual: %d\n", result[row*M2_WIDTH+col]);
            return 0;
         }
      }
   }
   return 1;
}

void sequentialMultiply(int *matrixA, int *matrixB) {
   int row, col, k;
   int Pvalue = 0;

   for (row = 0; row < M1_HEIGHT; row++) {
      for (col = 0; col < M2_WIDTH; col++) {
         Pvalue = 0;
         for (k = 0; k < M1_WIDTH; k++) {
            Pvalue += matrixA[row*M1_WIDTH+k] * matrixB[k*M2_WIDTH+col];
         }
         solution[row*M2_WIDTH+col] = Pvalue;
      }
   }
}

void matmul(int *A, int *B, int threadNum) {
   int i=0, j=0,k = 0, l = 0, m = 0, n = 0, posA = 0, posB = 0;
   int sum =0, a = 0, b = 0;
   for(i = threadNum * TILE_SIZE * M1_WIDTH; i < M1_WIDTH * M1_WIDTH; i += TILE_SIZE * M1_WIDTH * numThreads){
      for(j = 0; j < M1_WIDTH; j += TILE_SIZE){
         for(k = i, l = j * M1_WIDTH; k < i + M1_WIDTH; k+= TILE_SIZE, l += TILE_SIZE){
            for(m = 0; m < TILE_SIZE * TILE_SIZE; m++){
               sum = 0;
               posA = k + (m / TILE_SIZE * M1_WIDTH);
               posB = l + (m % TILE_SIZE * M1_WIDTH);
               for(n = 0; n < TILE_SIZE; n++){
                  a = A[posA + n];
                  b = B[posB + n];
                  sum += (a*b);
               }
               result[i + (m / TILE_SIZE * M1_WIDTH) + (j + (m % TILE_SIZE)) ] += sum;
            }
         }
      }
   }
}

void multiply(int threadNum) {
   int i, k;
   int start, end;
   int col, row;
   int pValue = 0;

   start = threadNum * RESULT_LEN / numThreads;
   end = (threadNum + 1) * RESULT_LEN / numThreads;
   row = start / M2_WIDTH;
   col = start % M2_WIDTH;


   for (i = start; i < end; i++) {
      pValue = 0;

      for (k = 0; k < M1_WIDTH; k++) {
         pValue += mat1[row*M1_WIDTH+k] * mat2[k*M2_WIDTH+col];
      }

      result[i] = pValue;

      col++;
      if (col == M2_WIDTH) {
         col = 0;
         row++;
      }
   }
}

void initMatrix(int *matrix, int len) {
   int i;

   for (i = 0; i < len; i++) {
      matrix[i] = rand() % MAX_CELL_MAG;
   }
}

void transposeMatrix(int *matrixA, int *matrixB){
   int i, j;
   for (i = 0; i < M1_WIDTH; i++) {
      for (j = 0; j < M1_WIDTH; j++) {
         matrixB[j * M1_WIDTH + i] = matrixA[i * M1_WIDTH + j];
      }
   }
}

int checkArgs(int argc, char **argv) {
   int threads;

   if (argc != 2) {
      fprintf(stderr, "Usage: %s <threads>\n", argv[0]);
      exit(-1);
   }

   if (!(threads = atoi(argv[1]))) {
      fprintf(stderr, "Error: Invalid thread count\n");
      exit(-1);
   }

   return threads;
}

void runOpenMP(){
   int i;
   struct timeval startTime, stopTime;

   omp_set_num_threads(numThreads);

   gettimeofday(&startTime, (struct timezone*)0);
   #pragma omp parallel for
   for (i = 0; i < numThreads; i++) {
      multiply(i);
   }
   #pragma omp barrier
   gettimeofday(&stopTime, (struct timezone*)0);

   if (!verify()) {
      printf("Incorrect Multiplication\n");
   }
   else {
      printf("Runtime OpenMP: %f s\n",
         (double) (stopTime.tv_sec + stopTime.tv_usec*1.0e-6) -
                  (startTime.tv_sec + startTime.tv_usec*1.0e-6));
   }
}

void runTiled(){
   int i;
   struct timeval startTime, stopTime;
   omp_set_num_threads(numThreads);
   gettimeofday(&startTime, (struct timezone*)0);
   #pragma omp parallel for
   for (i = 0; i < numThreads; i++) {
      matmul(mat1, mat2T, i);
   }
   #pragma omp barrier
   gettimeofday(&stopTime, (struct timezone*)0);

   if (!verify()) {
      printf("Incorrect Multiplication\n");
   }
   else {
      printf("Runtime Tiled: %f s\n",
         (double) (stopTime.tv_sec + stopTime.tv_usec*1.0e-6) -
                  (startTime.tv_sec + startTime.tv_usec*1.0e-6));
   }
}

void runSequential(){
   struct timeval startTime, stopTime;
   gettimeofday(&startTime, (struct timezone*)0);
   sequentialMultiply(mat1, mat2);
   gettimeofday(&stopTime, (struct timezone*)0);
   printf("Runtime Sequential: %f s\n",
             (double) (stopTime.tv_sec + stopTime.tv_usec*1.0e-6) -
                      (startTime.tv_sec + startTime.tv_usec*1.0e-6));
}


int main(int argc, char **argv) {
   numThreads = checkArgs(argc, argv);
   initMatrix(mat1, M1_HEIGHT * M1_WIDTH);
   initMatrix(mat2, M2_HEIGHT * M2_WIDTH);
   transposeMatrix(mat2, mat2T);
   runSequential();
   clear(result, RESULT_LEN);
   runOpenMP();
   clear(result, RESULT_LEN);
   runTiled();

   return 0;
}
