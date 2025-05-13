#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"
#include "fcntl.h"

int main()
{

    char* addr = (char *)shmat(shmget(250, 20000, 0666), 0, SHM_EXEC);
    printf(1, "%s", addr); 

}
