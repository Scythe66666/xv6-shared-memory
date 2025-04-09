#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"

void print_shm_error(int errno);

int main(){
    int shm_id;
    shm_id = shmget(IPC_PRIVATE, 10000, 0664);  // note only read permission is given
    if(shm_id < 0){
        print_shm_error(shm_id);
        exit();
    }
    printf(0, "NOTE: Done getting a segment since its required for testing ahead.\n");

    int ptr;

    printf(0, "\nSimulating error: shmid points to a non-existing identifier...\n");
    ptr = shmat(2, 0, SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\nSimulating error: Calling shmat without shmget...\n");

    // just for demo purpose specific to this case since we want to bypass the removed identified check in shmat
    
    if(fork() == 0)
    { 
        shmget(2, 5000, IPC_CREAT | IPC_EXCL | 0664);
        exit();
    } 
    wait();
    ptr = shmat(2, 0, 0664);
    if(ptr < 0){
        print_shm_error(ptr);
         printf(0, "*****Error check passed.*****\n");
    }
    else{
         printf(0, "*****Error check FAILED!!*****\n");
         exit();
    }



    // attaching one segment which would be required for testing ahead.

    ptr = shmat(1, 0, SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        exit();
    }
    printf(0, "NOTE: Done attaching segment with id: 1 to the current process. Required for testing ahead.\n");

    printf(0, "\nSimulating error: Calling shmat multiple times without REMAP flag...\n");
    ptr = shmat(1, 0, SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    // getting another segment for demo aheadh.
    shm_id = shmget(IPC_PRIVATE, 10000, 0664);  // note only read permission is given
    if(shm_id < 0){
        print_shm_error(shm_id);
        exit();
    }
    printf(0, "NOTE: Done getting another segment since its required for testing ahead.\n");


    printf(0, "\nSimulating error: Segment has only read but we want more (SHM_EXEC or 0)...\n");
    ptr = shmat(3, 0, SHM_EXEC);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\nSimulating error: address < HEAPLIMIT or > KERNBASE...\n");
    ptr = shmat(3, 0x50000000, SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\nSimulating error: address not aligned to page boundary...\n");
    ptr = shmat(3, 0x80000005, SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\nSimulating error: SHM_REMAP flag but addr is 0(NULL)...\n");
    ptr = shmat(3, 0, SHM_REMAP | SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }


    printf(0, "\n*****ALL TEST CASES PASSED!******\n");
    exit();
}


void print_shm_error(int errno) {
    switch (errno) {
        case EACCES:
            printf(0, "Error: User does not have permission to access the shared memory segment\n");
            break;
        case EEXIST:
            printf(0, "Error: IPC_CREAT and IPC_EXCL were specified in shmflg, but a shared memory segment already exists for key.\n");
            break;
        case EINVAL_LIMITS:
            printf(0, "Error: A new segment was to be created and size is less than SHMMIN or greater than SHMMAX.\n");
            break;
        case EINVAL_SIZE:
            printf(0, "Error: A segment for the given key exists, but size is greater than the size of that segment.\n");
            break;
        case ENFILE:
            printf(0, "Error: The system-wide limit on the total number of open files has been reached.\n");
            break;
        case ENOENT:
            printf(0, "Error: No segment exists for the given key, and IPC_CREAT was not specified.\n");
            break;
        case ENOMEM:
            printf(0, "Error: Could not allocate memory for the descriptor or for the page tables.\n");
            break;
        case ENOSPC:
            printf(0, "Error: All possible shared memory IDs have been taken (SHMMNI), or allocating a segment of the requested size would cause the system to exceed the system-wide limit on shared memory (SHMALL).\n");
            break;
        case EIDRM:
            printf(0, "Error: shmid points to a removed identifier.\n");
            break;
        case EINVAL:
            printf(0, "Error: Invalid shmid value, unaligned shmaddr, or invalid shmaddr value, or cannot attach segment at shmaddr, or SHM_REMAP was specified and shmaddr was NULL.\n");
            break;
        default:
            printf(0, "Error: as mentioned above in the kernel code itself\n");
            break;
    }
}
