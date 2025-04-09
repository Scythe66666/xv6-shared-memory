#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "shm.h"

void print_shm_error(int errno);

int main(){
    int shm_id;

    printf(0, "\nSimulating error: calling shmget using only IPC_EXCL...\n");
    shm_id = shmget(1, 10000, IPC_EXCL | 0666);
    if(shm_id < 0){
        print_shm_error(shm_id);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }


    printf(0, "\nSimulating: Creating segment using only IPC_CREAT flag ...\n");
    shm_id = shmget(1, 10000, IPC_CREAT | 0664);
    if(shm_id < 0){
        print_shm_error(shm_id);
        exit();
    }
    else{
        printf(0, "Segment created!\n"); 
    }



    printf(0, "\nSimulating error: Trying to create a segment which already exists...\n");
    shm_id = shmget(1, 10000, IPC_CREAT | IPC_EXCL | 0664);
    if(shm_id < 0){
        print_shm_error(shm_id);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\nSimulating error: Trying to get segment of size greater than existing one...\n");
    shm_id = shmget(1, 12000, 0664);
    if(shm_id < 0){
        print_shm_error(shm_id);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    //TODO: redo the permissions, this is not correct
    printf(0, "\nSimulating error: Permission not matching...\n");
    shm_id = shmget(1, 10000, 0666);
    if(shm_id < 0){
        print_shm_error(shm_id);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\nSimulating error: Segment not existing and IPC_CREAT not mentioned...\n");
    shm_id = shmget(5, 10000, 0660);
    if(shm_id < 0){
        print_shm_error(shm_id);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\n*****ALL TEST CASES PASSED!******");
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
