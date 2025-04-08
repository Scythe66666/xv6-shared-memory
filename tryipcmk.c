#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"
#include "user.h"

int main(int argc, char* argv[])
{
    printf(0, "make sure you have created the shm using ipcmk of size 1000\n");
    int shm_id = atoi(argv[1]); 
    printf(0, "writing and reading to shared memory\n"); 
    printf(0, "writing 1 to 100\n"); 
    shm_id = shmget(shm_id, 500, 0666);
    printf(0, "shmid is %d\n", shm_id); 
    int* addr = (int *)shmat(shm_id, 0, 0666);  
    
    printf(0, "addr is %d\n", addr); 

    for(int i = 0; i < 100; i++)
        *addr = i + 1; 
    
    printf(0, "printing from the shared memory \n");
    for(int i = 0; i < 100; i++)
        printf(0, "%d\t ", i);
    exit();
}
