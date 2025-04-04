#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"

int main()
{
    int shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    printf(1, "value of shm_id: %d", shm_id);
    shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    printf(1, "value of shm_id: %d", shm_id);
    shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    printf(1, "value of shm_id: %d", shm_id);
    shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    printf(1, "value of shm_id: %d", shm_id);
    shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    printf(1, "value of shm_id: %d", shm_id);
    exit();
}
