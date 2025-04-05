#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"

int main()
{
    int shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    int* ptr = (int*) shmat(shm_id, 0, 0666);  
    printf(0, "value of ptr is %d shmid is: %d", ptr, shm_id);
    for(int i = 0; i < 10000 / 4; i++)
    { 
        int *ptr2 = ptr + i;
        printf(0, " ok at i = %d value of ptr= %d\n", i, ptr2);
        *(ptr + i) = i;
    } 
    /*for(int i = 0; i < 10000 / 4; i++)*/
    /*    printf(0, "ptr[%d]= %d\n", i, ptr[i]);*/
    
    exit();
}
