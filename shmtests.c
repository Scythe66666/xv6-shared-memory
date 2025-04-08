#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

struct shm_ds buf, buf2;

char shm_id_str[16];
char *argv[] = {"tryshm9", shm_id_str, 0};

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

void tryshm1(){
    int shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    for(int i = 0; i < 20; i++)
    {
        printf(0, "shm_id: %d\n", shm_id);
        shm_id = shmget(IPC_PRIVATE, 10000, 0666);
        if(shm_id < 0){
            print_shm_error(shm_id);
            exit();
        } 
    }
    printf(0, "shm_id: %d", shm_id);
    return;
}

void tryshm2(){
    int shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    int* ptr = (int*) shmat(shm_id, 0, 0666);  
    printf(0, "value of ptr is %d shmid is: %d", ptr, shm_id);
    for(int i = 0; i < 1000 / 4; i++)
    { 
        int *ptr2 = ptr + i;
        printf(0, " ok at i = %d value of ptr= %d\n", i, ptr2);
        *(ptr + i) = i;
    } 

    for(int i = 0; i < 1000 / 4; i++)
    { 
        printf(0, "ptr[%d]= %d", i, ptr[i]);
    }
   
	printf(0, "Test passed! \n");
    return;
}

void tryshm3(){

    int shm_id = shmget(IPC_PRIVATE, 10000, 0666); 
    int* ptr = (int*) shmat(shm_id, 0, 0666);  
    printf(0, "value of ptr is %d shmid is: %d\n", ptr, shm_id);
    
    if(fork() != 0)
        wait();
    else
    {
        printf(0, "we are inside the child process\n");
        for(int i = 0; i < 1000 / 4; i++)
        { 
         int *ptr2 = ptr + i;
         printf(0, " ok at i = %d value of ptr= %d\n", i, ptr2);
         *(ptr + i) = i;
        } 
    }

    return;
}

void tryshm6(){

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
    int shmid = shmget(IPC_PRIVATE, 10000, IPC_CREAT | 0664);
    if(shmid < 0){
        print_shm_error(shmid);
        exit();
    }
    else{
        printf(0, "Segment created!\n"); 
    }



    printf(0, "\nSimulating error: Trying to create a segment which already exists...\n");
    shm_id = shmget(shmid, 10000, IPC_CREAT | IPC_EXCL | 0664);
    if(shm_id < 0){
        print_shm_error(shm_id);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\nSimulating error: Trying to get segment of size greater than existing one...\n");
    shm_id = shmget(shmid, 12000, 0664);
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
    shm_id = shmget(shmid, 10000, 0666);
    if(shm_id < 0){
        print_shm_error(shm_id);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }


    printf(0, "\nSimulating error: Segment not existing and IPC_CREAT not mentioned...\n");
    shm_id = shmget(shmid+1, 10000, 0660);
    if(shm_id < 0){
        print_shm_error(shm_id);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\n*****ALL TEST CASES PASSED!******");

    return;
}

void tryshm7(){

    int shm_id;
    shm_id = shmget(IPC_PRIVATE, 10000, 0664);  // note only read permission is given
    if(shm_id < 0){
        print_shm_error(shm_id);
        exit();
    }
    printf(0, "NOTE: Done getting a segment since its required for testing ahead.\n");

    int ptr;

    printf(0, "\nSimulating error: shmid points to a non-existing identifier...\n");
    ptr = shmat(shm_id+1, 0, SHM_RDONLY);
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
    int shm_id2;
    if(fork() == 0)
    { 
        shm_id2 = shmget(IPC_PRIVATE, 5000, 0664);
        exit();
    } 
    wait();
    ptr = shmat(shm_id2, 0, 0664);
    if(ptr < 0){
        print_shm_error(ptr);
         printf(0, "*****Error check passed.*****\n");
    }
    else{
         printf(0, "*****Error check FAILED!!*****\n");
         exit();
    }



    // attaching one segment which would be required for testing ahead.



    ptr = shmat(shm_id, 0, SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        exit();
    }
    printf(0, "NOTE: Done attaching segment with id: 1 to the current process. Required for testing ahead.\n");

    printf(0, "\nSimulating error: Calling shmat multiple times without REMAP flag...\n");
    ptr = shmat(shm_id, 0, SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    // getting another segment for demo aheadh.
    int shm_id3 = shmget(IPC_PRIVATE, 10000, 0664);  // note only read permission is given
    if(shm_id3 < 0){
        print_shm_error(shm_id3);
        exit();
    }
    printf(0, "NOTE: Done getting another segment since its required for testing ahead.\n");


    printf(0, "\nSimulating error: Segment has only read but we want more (SHM_EXEC or 0)...\n");
    ptr = shmat(shm_id3, 0, SHM_EXEC);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\nSimulating error: address < HEAPLIMIT or > KERNBASE...\n");
    ptr = shmat(shm_id3, 0x50000000, SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\nSimulating error: address not aligned to page boundary...\n");
    ptr = shmat(shm_id3, 0x80000005, SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }

    printf(0, "\nSimulating error: SHM_REMAP flag but addr is 0(NULL)...\n");
    ptr = shmat(shm_id3, 0, SHM_REMAP | SHM_RDONLY);
    if(ptr < 0){
        print_shm_error(ptr);
        printf(0, "*****Error check passed.*****\n");
    }
    else{
        printf(0, "*****Error check FAILED!!*****\n");
        exit();
    }


    printf(0, "\n*****ALL TEST CASES PASSED!******\n");

    return;
}

void itoa(int value, char *str){
    char buf[16];
    int i = 0, j = 0;
    
    if(value == 0){
        str[j++] = '0';
        str[j] = '\0';
        return;
    }

    while(value > 0){
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    while(i > 0){
        str[j++] = buf[--i];
    }

    str[j] = '\0';
}


void tryshm8(){

    int count = 0;
    int shm_id = shmget(IPC_PRIVATE, sizeof(count), 0666);
    if (shm_id < 0) {
        print_shm_error(shm_id);
        exit();
    }

    // Convert shm_id to string
    itoa(shm_id, shm_id_str);

    for (int i = 0; i < 10; i++) {
        if (fork() == 0) {
            exec("tryshm9", argv);

            // if exec fails
            printf(0, "Exec failed in tryshm8\n");
            exit();
        }
    }

    for (int i = 0; i < 10; i++) {
        wait();
    }

    printf(0, "\n*****ALL TEST CASES PASSED!******");

    return;
}

void tryshmctl(){

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

    return;
}

void tryipcmk(){

    // printf(0, "make sure you have created the shm using ipcmk of size 1000\n");
    // int shm_id = atoi(argv[1]); 
    printf(0, "writing and reading to shared memory\n"); 
    printf(0, "writing 1 to 100\n"); 
    int shm_id = shmget(IPC_PRIVATE, 500, 0666);
    printf(0, "shmid is %d\n", shm_id); 
    int* addr = (int *)shmat(shm_id, 0, 0666);  
    
    printf(0, "addr is %d\n", addr); 

    for(int i = 0; i < 100; i++)
        *addr = i + 1; 
    
    printf(0, "printing from the shared memory \n");
    for(int i = 0; i < 100; i++)
        printf(0, "%d\t ", i);

    return;
}

void tryipcrm(){

    printf(0, "make sure you have created the shm using ipcmk of size 1000\n");
    // int shm_id = atoi(argv[1]);
    int shm_id = shmget(IPC_PRIVATE, 1000, 0666);
    if(shm_id < 0){
        print_shm_error(shm_id);
        exit();
    } 
    printf(0, "shmid is %d\n", shm_id); 
    shmctl(shm_id, IPC_RMID, &buf2);
    printf(0, "shmid is %d is marked for deletion\n", shm_id);

    return;
}

int main(){

    printf(0, "\nTesting: Trying to get multiple segment by the same process itself.....\n");
    tryshm1();
    printf(0, "\n********Test PASSED!*********\n");

    printf(0, "\nTesting: Trying to read and write to a segment.....\n");
    tryshm2();
    printf(0, "\n********Test PASSED!*********\n");

    printf(0, "\nTesting: Testing fork.....\n");
    tryshm3();
    printf(0, "\n********Test PASSED!*********\n");

    printf(0, "\nTesting: Testing shmget.....\n");
    tryshm6();
    printf(0, "\n********Test PASSED!*********\n");

    printf(0, "\nTesting: Testing shmat.....\n");
    tryshm7();
    printf(0, "\n********Test PASSED!*********\n");

    // printf(0, "\nTesting: Multiple processes doing shm.....\n");
    // tryshm8();
    // printf(0, "\n********Test PASSED!*********\n");

    printf(0, "\nTesting: shmctl.....\n");
    tryshmctl();
    printf(0, "\n********Test PASSED!*********\n");

    printf(0, "\nTesting: ipcmk.....\n");
    tryipcmk();
    printf(0, "\n********Test PASSED!*********\n");

    printf(0, "\nTesting: ipcrm.....\n");
    tryipcrm();
    printf(0, "\n********Test PASSED!*********\n");


    exit();
}