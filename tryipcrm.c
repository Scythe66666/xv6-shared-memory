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
    struct shm_ds buf;
    printf(0, "shmid is %d\n", shm_id); 
    shmctl(shm_id, IPC_RMID, &buf);
    printf(0, "shmid is %d is marked for deletion\n", shm_id); 

    exit();
}
