#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define M_LEN           3200

#define RESULT_LEN      M_LEN * M_LEN
#define MAX_CELL_MAG    100
#define TILE_SIZE       64

int mat1[M_LEN * M_LEN];
int mat2[M_LEN * M_LEN];
int mat2T[M_LEN * M_LEN];

int result[RESULT_LEN];
int solution[RESULT_LEN];
int numThreads;


int verify() {
   int row, col;

   for (row = 0; row < M_LEN; row++) {
      for (col = 0; col < M_LEN; col++) {
         if (result[row*M_LEN+col] != solution[row*M_LEN+col]) {
            printf("**** row: %d col: %d ****\n", row, col);
            printf("Expected: %d\n", solution[row*M_LEN+col]);
            printf("Actual: %d\n", result[row*M_LEN+col]);
            return 0;
         }
      }
   }
   
   return 1;
}


void tiledMultiply(int *A, int *B, int threadNum) {
   int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0, posA = 0, posB = 0;
   int sum = 0, a = 0, b = 0;
   
   for (i = threadNum * TILE_SIZE * M_LEN; i < M_LEN * M_LEN; i += TILE_SIZE * M_LEN * numThreads) {
      for (j = 0; j < M_LEN; j += TILE_SIZE) {
         for (k = i, l = j * M_LEN; k < i + M_LEN; k += TILE_SIZE, l += TILE_SIZE) {
            for (m = 0; m < TILE_SIZE * TILE_SIZE; m++) {
               sum = 0;
               posA = k + (m / TILE_SIZE * M_LEN);
               posB = l + (m % TILE_SIZE * M_LEN);
               
               for (n = 0; n < TILE_SIZE; n++) {
                  a = A[posA + n];
                  b = B[posB + n];
                  sum += (a*b);
               }
               
               result[i + (m / TILE_SIZE * M_LEN) + (j + (m % TILE_SIZE)) ] += sum;
            }
         }
      }
   }
}


void runTiled() {
   int i;
   struct timeval startTime, stopTime;
   
   omp_set_num_threads(numThreads);
   gettimeofday(&startTime, (struct timezone*)0);
   
   #pragma omp parallel for
   for (i = 0; i < numThreads; i++) {
      tiledMultiply(mat1, mat2T, i);
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


void simpleMultiply(int threadNum) {
   int i, k;
   int start, end;
   int col, row;
   int pValue = 0;

   start = threadNum * RESULT_LEN / numThreads;
   end = (threadNum + 1) * RESULT_LEN / numThreads;
   row = start / M_LEN;
   col = start % M_LEN;

   for (i = start; i < end; i++) {
      pValue = 0;

      for (k = 0; k < M_LEN; k++) {
         pValue += mat1[row*M_LEN+k] * mat2[k*M_LEN+col];
      }

      result[i] = pValue;
      col++;

      if (col == M_LEN) {
         col = 0;
         row++;
      }
   }
}


void runOpenMP() {
   int i;
   struct timeval startTime, stopTime;

   omp_set_num_threads(numThreads);
   gettimeofday(&startTime, (struct timezone*)0);

   #pragma omp parallel for
   for (i = 0; i < numThreads; i++) {
      simpleMultiply(i);
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


void sequentialMultiply(int *matrixA, int *matrixB) {
   int row, col, k;
   int Pvalue = 0;

   for (row = 0; row < M_LEN; row++) {
      for (col = 0; col < M_LEN; col++) {
         Pvalue = 0;
         
         for (k = 0; k < M_LEN; k++) {
            Pvalue += matrixA[row*M_LEN+k] * matrixB[k*M_LEN+col];
         }

         solution[row*M_LEN+col] = Pvalue;
      }
   }
}


void runSequential() {
   struct timeval startTime, stopTime;
   
   gettimeofday(&startTime, (struct timezone*)0);
   sequentialMultiply(mat1, mat2);
   gettimeofday(&stopTime, (struct timezone*)0);
   
   printf("Runtime Sequential: %f s\n",
             (double) (stopTime.tv_sec + stopTime.tv_usec*1.0e-6) -
                      (startTime.tv_sec + startTime.tv_usec*1.0e-6));
}


void clear(int *array, int size){
   int i;
   for (i = 0; i < size; i++) {
      array[i] = 0;
   }
}


void transposeMatrix(int *matrixA, int *matrixB){
   int i, j;
   
   for (i = 0; i < M_LEN; i++) {
      for (j = 0; j < M_LEN; j++) {
         matrixB[j * M_LEN + i] = matrixA[i * M_LEN + j];
      }
   }
}


void initMatrix(int *matrix, int len) {
   int i;

   for (i = 0; i < len; i++) {
      matrix[i] = rand() % MAX_CELL_MAG;
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


int main(int argc, char **argv) {
   numThreads = checkArgs(argc, argv);
   
   initMatrix(mat1, M_LEN * M_LEN);
   initMatrix(mat2, M_LEN * M_LEN);
   
   transposeMatrix(mat2, mat2T);
   
   runSequential();
   clear(result, RESULT_LEN);
   
   runOpenMP();
   clear(result, RESULT_LEN);
   
   runTiled();

   return 0;
}
