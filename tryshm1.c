#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"

int main()
{
    int shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    for(int i = 0; i < 20; i++)
    {
        printf(0, "shm_id: %d\n", shm_id);
        shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    }
    printf(0, "shm_id: %d", shm_id);

    exit();
}
