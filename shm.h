//defining rough #defines
#define IPC_PRIVATE 0     //since otherwise keys actually start from 1???
#define SHMBASE 1

// defining flag values for shmflg for shmget.
#define IPC_CREAT 512     // 10th least significant bit
#define IPC_EXCL 1024      // 11th least significant bit


// defining error numbers
#define EACCES     -2
#define EEXIST     -3
#define EINVAL_LIMITS -4
#define EINVAL_SIZE -5
#define ENFILE     -6
#define ENOENT     -7
#define ENOMEM     -8
#define ENOSPC     -9