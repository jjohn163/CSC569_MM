#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define M1_HEIGHT       3000
#define M1_WIDTH        3000
#define M2_HEIGHT       3000
#define M2_WIDTH        3000

#define RESULT_LEN      M1_HEIGHT * M2_WIDTH
#define MAX_CELL_MAG    100

int mat1[M1_WIDTH * M1_HEIGHT];
int mat2[M2_WIDTH * M2_HEIGHT];

int result[RESULT_LEN];
int numThreads;

int verify() {
   int row, col, k;
   int Pvalue = 0;

   for (row = 0; row < M1_HEIGHT; row++) {
      for (col = 0; col < M2_WIDTH; col++) {
         Pvalue = 0;
         for (k = 0; k < M1_WIDTH; k++) {
            Pvalue += mat1[row*M1_WIDTH+k] * mat2[k*M2_WIDTH+col];
         }
         if (result[row*M2_WIDTH+col] != Pvalue) {
            printf("**** row: %d col: %d k: %d****\n", row, col, k);
            printf("Expected: %d\n", Pvalue);
            printf("Actual: %d\n", result[row*M2_WIDTH+col]);
            return 0;
         }
      }
   }

   return 1;
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

/*
void multiply(int threadNum) {
   int row, col, i, k;
   int pValue = 0;

   row = threadNum / M2_WIDTH;
   col = threadNum % M2_WIDTH;

   for (i = threadNum; i < RESULT_LEN; i+=numThreads) {
      pValue = 0;

      for (k = 0; k < M1_WIDTH; k++) {
         pValue += mat1[row*M1_WIDTH+k] * mat2[k*M2_WIDTH+col];
      }

      result[i] = pValue;

      col += numThreads;
      while (col >= M2_WIDTH) {
         col -= M2_WIDTH;
         row++;
      }
   }
}
*/

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
   int i;
   struct timeval startTime, stopTime;
   numThreads = checkArgs(argc, argv);

   omp_set_num_threads(numThreads);
   initMatrix(mat1, M1_HEIGHT * M1_WIDTH);
   initMatrix(mat2, M2_HEIGHT * M2_WIDTH);

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
      printf("Correct Multiplication\n");
      printf("Runtime: %f s\n",
         (double) (stopTime.tv_sec + stopTime.tv_usec*1.0e-6) -
                  (startTime.tv_sec + startTime.tv_usec*1.0e-6));
   }

   return 0;
}