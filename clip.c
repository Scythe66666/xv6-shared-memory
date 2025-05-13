#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"
#include "fcntl.h"


int main(int argc, char* argv[])
{
    printf(1, "checkpoint 0 \n");
    int num = shmget(1, 20000, 0666);
    printf(1, "value of num is %d \n", num);
    char* addr = (char *)shmat(shmget(num, 20000, 0666), 0, SHM_EXEC);
    printf(1, "checkpoint 1 reached \n");
    if(argc == 1)
    {
        int i = 0;
        int arg_count = 0;
        while(arg_count < argc && i < 20000)
        {
            int j = 0;
            char c;
            while((c = argv[arg_count][j]) != '\0')
            {
                addr[i] = c;
                i++;
                j++;
            }
            arg_count++;
        }

        if(i == 20000)
            printf(0, "buffer overflow");
        addr[19999] = '\0';
    }
    else 
    {
        char* buff[1024];
        int n = read(0, buff, 1023);
        buff[n] = '\0';

    }
    exit();
}
