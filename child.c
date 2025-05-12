#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"

int main(int argc, char *argv[]) {
  if (argc < 6) {
    printf(1, "child: insufficient arguments\n");
    exit();
  }

  int rows1 = atoi(argv[1]);
  int cols1 = atoi(argv[2]);
  int cols2 = atoi(argv[3]);
  int start_row = atoi(argv[4]);
  int end_row = atoi(argv[5]);

  shmget(1, rows1 * cols1 * 4, 0666); 
  shmget(2, rows1 * cols1 * 4, 0666);
  shmget(3, rows1 * cols1 * 4, 0666);
    
  int *Matrix1 = (int *) shmat(1, 0, SHM_EXEC);
  int *Matrix2 = (int *) shmat(2, 0, SHM_EXEC);
  int *Answer = (int *) shmat(3, 0, SHM_EXEC);

  for (int i = start_row; i < end_row; i++) {
    for (int j = 0; j < cols2; j++) {
      int sum = 0;
      for (int k = 0; k < cols1; k++) {
        int a = Matrix1[i * cols1 + k];
        int b = Matrix2[k * cols2 + j];
        sum += a * b;
      }
      Answer[i * cols2 + j] = sum;
    }
  }

  exit();
}

