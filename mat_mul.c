#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"
#include "fcntl.h"

#define MAXBUF 1024

int read_matrix_from_file(const char *filename, int *matrix, int rows, int cols) {
  char buf[MAXBUF];
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    printf(1, "Error opening %s\n", filename);
    exit();
  }

  /*int n = read(fd, buf, MAXBUF - 1);*/
  /*buf[n] = '\0';*/
  /*printf(0, "buf is %s \n", buf);*/
  /*close(fd);*/

  int idx = 0, val = 0, reading = 0;

  /*for (int i = 0; buf[i] != '\0'; i++) {*/
  /*  printf(0, "value of i is %d\n", i); */ 
  /*  if (buf[i] >= '0' && buf[i] <= '9') {*/
  /*    val = val * 10 + (buf[i] - '0');  // Build the number*/
  /*    reading = 1;*/
  /*  } else if ((buf[i] == ' ' || buf[i] == '\n') && reading) {*/
  /*    matrix[idx++] = val;  // Save the number to the matrix*/
  /*    val = 0;  // Reset for the next number*/
  /*    reading = 0;*/
  /*  }*/
  /*}*/
  

    while(1) 
    {
        int n = read(fd, buf, MAXBUF - 1);
        if(n == 0)
            break; 
        for(int j = 0; j < n; j++)
        {
          if (buf[j] >= '0' && buf[j] <= '9') {
            val = val * 10 + (buf[j] - '0');  // Build the number
            reading = 1;
          } else if ((buf[j] == ' ' || buf[j] == '\n') && reading) {
            matrix[idx++] = val;  // Save the number to the matrix
            val = 0;  // Reset for the next number
            reading = 0;
          }
        }
    }

  // Handle last value if no trailing space
  if (reading) {
    matrix[idx++] = val;
  }

  if (idx != rows * cols) {
    printf(1, "Error: Expected %d elements but read %d.\n", rows * cols, idx);
    exit();
  }

  return idx;
}


int main()
{
    int rows1 = 100;
    int cols1 = 100;
    int rows2 = 100;
    int cols2 = 100;
    printf(1, "checkpoint1 \n");
    
    int shmid1 = shmget(1, rows1 * cols1 * 4, IPC_CREAT | IPC_EXCL | 0666);
    int shmid2 = shmget(2, rows2 * cols2 * 4, IPC_CREAT | IPC_EXCL | 0666);
    int shmid3 = shmget(3, rows1 * cols1 * 4, IPC_CREAT | IPC_EXCL | 0666);
    printf(1, "checkpoint2 \n");
    
    int* Matrix1 = (int*)shmat(shmid1, 0, SHM_EXEC);
    int* Matrix2 = (int*)shmat(shmid2, 0, SHM_EXEC);
    int* Matrix3 = (int*)shmat(shmid3, 0, SHM_EXEC);
    printf(1, "checkpoint3 \n");

    read_matrix_from_file("Matrix1.txt", Matrix1, rows1, cols1);
    read_matrix_from_file("Matrix2.txt", Matrix2, rows2, cols2);
    
    printf(1, "checkpoint1 \n");
    int time = uptime();
    
    for(int i = 0; i < rows1; i++)
        for(int j = 0; j < cols1; j++)
            for(int k = 0; k < cols1; k++)
                Matrix3[i * cols1 + j] += Matrix1[i * cols1 + k]
                    * Matrix2[k * cols2 + j];
    
    int time2 = uptime();
    printf(1, "time for without shm is %d\n", time2 - time);
    
    time = uptime();
    if (cols1 != rows2) {
        printf(1, "Invalid matrix dimensions: %d x %d * %d x %d\n", rows1, cols1, rows2, cols2);
        exit();
    }
    
    int rows_per_child = rows1 / 4;
    for (int i = 0; i < 4; i++) {
        int start = i * rows_per_child;
        int end = (i == 4 - 1) ? rows1 : (i + 1) * rows_per_child;

        int pid = fork();
        if (pid == 0) {
            char s_row1[8], s_col1[8], s_col2[8], s_start[8], s_end[8];
            itoa(rows1, s_row1);
            itoa(cols1, s_col1);
            itoa(cols2, s_col2);
            itoa(start, s_start);
            itoa(end, s_end);

            char *argv[] = {
            "child", s_row1, s_col1, s_col2, s_start, s_end, 0
            };
            exec("child", argv);
            printf(1, "Exec failed\n");
            exit();
        }
    }

    for(int i = 0; i < 4; i++)
        wait();
    time2 = uptime();
    printf(1, "time for with shm is %d\n", time2 - time);
  exit();
}
