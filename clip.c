#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
/*#include "shm.h"*/
#include "fcntl.h"


int main(int argc, char* argv[])
{
    int num = shmget(10, 1000, 0666);
    if(num < 0)
        num = shmget(10, 1000, IPC_CREAT | IPC_EXCL | 0666);

    /*printf(1, "checkpoint 0 \n");*/
    /*printf(1, "value of num is %d \n", num);*/
    char* addr = (char *)shmat(num, 0, SHM_EXEC);
    /*printf(1, "checkpoint 1 reached \n");*/
    if(argc != 1)
    {
        int i = 0;
        int arg_count = 1;
        while(arg_count < argc && i < 1999)
        {
            int j = 0;
            char c;
            while((c = argv[arg_count][j]) != '\0')
            {
                addr[i] = c;
                i++;
                j++;
            }
            addr[i++] = ' ';
            arg_count++;
        }

        if(i == 1999)
            printf(1, "buffer overflow");
        addr[i] = '\0';
    }
    printf(1, "string is %s", addr);
    exit();
}
