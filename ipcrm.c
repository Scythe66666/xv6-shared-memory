#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"
#include "user.h"

struct shm_ds buf2;
int main(int argc, char* argv[])
{
    /*printf(0, "inside the main function\n");*/
    buf2.key = 0;
    buf2.size = 1000;
    int shm_id = atoi(argv[1]); 
    shmget(shm_id, 0, 0);
    shmat(shm_id, 0, 0);
    shmctl(shm_id, IPC_RMID, &buf2);
    printf(0, "shmid: %d is marked for deletion", shm_id);
    exit();
}
