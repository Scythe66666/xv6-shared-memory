//defining rough #defines
#define IPC_PRIVATE 0     //since otherwise keys actually start from 1???
#define SHMBASE 1

// defining flag values for shmflg for shmget.
#define IPC_CREAT 512     // 10th least significant bit
#define IPC_EXCL 1024      // 11th least significant bit

// flags for shmat
/*
*       shmflg structure for shmat
*       bit 0 : SHM_EXEC
*       bit 1 : SHM_RDONLY
*       bit 2 : SHM_REMAP
*       bit 3 : SHM_RND
*/
#define SHM_EXEC 1
#define SHM_RDONLY 2 
#define SHM_REMAP 4
#define SHM_RND 8

#define SHM_RD 292      // decimal equivalent of 100100100 (readonly for all three).
#define SHM_RD_WR 438   // decimal equivalent of 110110110 (read and write both for all three).


// defining error numbers
#define EACCES     -2
#define EEXIST     -3
#define EINVAL_LIMITS -4
#define EINVAL_SIZE -5
#define ENFILE     -6
#define ENOENT     -7
#define ENOMEM     -8
#define ENOSPC     -9
#define EIDRM       -10
#define EINVAL      -11