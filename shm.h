//defining rough #defines
#define IPC_PRIVATE 0     //since otherwise keys actually start from 1???
#define SHMBASE 1

// defining flag values for shmflg for shmget.
#define IPC_CREAT 512     // 10th least significant bit
#define IPC_EXCL 1024      // 11th least significant bit

// flags for shmat

#define SHM_EXEC 6	//read and write both: 2nd and 3rd both bits set
#define SHM_RDONLY 4 	// only read bit set i.e. 3rd bit
#define SHM_REMAP 2048	//12th bit
#define SHM_RND 4096	//13th bit

// #define SHM_RD 292      // decimal equivalent of 100100100 (readonly for all three).
// #define SHM_RD_WR 438   // decimal equivalent of 110110110 (read and write both for all three).


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
#define EINVAL_CTL  -12
#define EFAULT      -13
#define EPERM       -14

struct ipc_perm{
	uint key;
	uint mode;
};


struct shm_ds {
  uint key;
  uint size;
  uint pid;
  uint lpid;
  uint nget;    //number of segements which called get
  uint alloclist[1000];   //physical page addresses
  int alloclist_index; //points to first unallocated index
  uint flags;
  int delete_mark;  // this will be one when marked 
    // for deletion
  struct ipc_perm shm_perm;    // access permissions
};



/* #define for the op paramter of shmctl */
#define IPC_STAT 1
#define IPC_SET 2
#define IPC_RMID 3



