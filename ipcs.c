#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"
#include "user.h"

struct shm_ds buf;

void print_padded_int(int val, int width) {
    char buf[16];
    int i = 0;
    int len;

    if (val == 0) {
        buf[i++] = '0';
    } else {
        int temp = val;
        while (temp > 0 && i < sizeof(buf)) {
            buf[i++] = (temp % 10) + '0';
            temp /= 10;
        }
    }

    for (int j = 0; j < i / 2; j++) {
        char t = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = t;
    }
    buf[i] = '\0';

    printf(0, "%s", buf);

    len = i;
    for (int j = 0; j < width - len; j++) {
        printf(0, " ");
    }
}


int main(int argc, char* argv[])
{

    printf(0, "shmid     nattach   bytes     \n");
    for (int i = 0; i < 256; i++) {
        int shm_id = i + 1; 
        if(shmctl(shm_id, IPC_STAT, &buf) < 0)
        {
            continue;
        }
        if(buf.key == 0)
            continue;
        printf(0, "Sumedh was here\n"); 
        print_padded_int(buf.key, 10);
        print_padded_int(buf.nattach, 10);
        print_padded_int(buf.size, 10);
        printf(0, "\n");
    }

    exit();
}
