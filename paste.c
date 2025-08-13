#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"
#include "fcntl.h"

int main()
{
    
    int num = shmget(10, 1000, 0666);
    char* addr = (char *)shmat(num, 0, SHM_EXEC);


    printf(1, "string is %s", addr);
    exit();
}
