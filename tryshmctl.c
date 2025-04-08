#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void print_shm_error(int errno);

struct shm_ds buf;

int main(){

    int shm_id1;
    
    shm_id1 = shmget(IPC_PRIVATE, 10000, 0666);  // note only read permission is given
    if(shm_id1 < 0){
        print_shm_error(shm_id1);
        exit();
    }
    printf(0, "\nNOTE: Done getting FIRST segment since its required for testing ahead.\n");

    buf.key = 0;

    // trying op == IPC_STAT
    printf(0, "\n Testing IPC_STAT......\n");

    shmctl(shm_id1, IPC_STAT, &buf);

    printf(0, "buf->key: %d\n", buf.key);
    printf(0, "buf->size: %d\n", buf.size);
    printf(0, "buf->pid: %d\n", buf.pid);
    printf(0, "buf->lpid: %d\n", buf.lpid);
    printf(0, "buf->nattac: %d\n", buf.nattach);
    printf(0, "buf->delete_mark: %d\n", buf.delete_mark);
    printf(0, "buf->shm_perm.key: %d\n", buf.shm_perm.key);
    printf(0, "buf->shm_perm.mode: %d\n", buf.shm_perm.mode);

    printf(0, "\n*******OK done*******\n");

    // trying op == IPC_SET
    printf(0, "\n Testing IPC_SET......\n");

    buf.shm_perm.mode = SHM_RDONLY;

    shmctl(shm_id1, IPC_SET, &buf);


    shmctl(shm_id1, IPC_STAT, &buf);    //since we cant directly acces shms in userland code we get it via shmctl
    printf(0, "Mode change after shcmtl -- ");
    printf(0, "New mode: : %d\n", buf.shm_perm.mode);

    printf(0, "\n*******OK done*******\n");

    // trying op == IPC_RMID
    printf(0, "\n Testing IPC_RMID......\n");

    shmctl(shm_id1, IPC_RMID, &buf);

    shmctl(shm_id1, IPC_STAT, &buf);    //since we cant directly acces shms in userland code we get it via shmctl
    printf(0, "Changed delete_mark flag after shcmtl -- ");
    printf(0, "delete_mark: %d\n", buf.delete_mark);

    printf(0, "\n*******OK done*******\n");

    printf(0, "\n********ALL TESTING DONE!********\n");

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
        case EINVAL_CTL:
            printf(0, "Error: shmid is not a valid identifier, or op is not a valid operation.\n");
            break;
        case EFAULT:
            printf(0, "Error: The argument op has value IPC_SET or IPC_STAT but the address pointed to by buf isn't accessible.\n");
            break;
        case EPERM:
            printf(0, "Error: IPC_SET or IPC_RMID is attempted, and the effective user ID of the calling process is not that of the creator (found  in  shm_perm.cuid),  or the owner (found in shm_perm.uid), and the process was not privileged\n");
            break;
        default:
            printf(0, "Error: as mentioned above in the kernel code itself\n");
            break;
    }
}
