#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"
#include "user.h"

int main(int argc, char* argv[])
{
    argc = 1;
    int size = atoi(argv[1]);
    char* octalStr = argv[2];
    int result = 0;
    for (int i = 0; octalStr[i] != '\0'; i++) {
        char ch = octalStr[i];

        // Check if valid octal digit
        if (ch < '0' || ch > '7') {
            printf(0, "Invalid octal digit: %c\n", ch);
            return 1;
        }

        result = result * 8 + (ch - '0');
    }
    int shmid = shmget(0, size, result); 
    printf(0, "shmid is %d", shmid);
    exit();
}
