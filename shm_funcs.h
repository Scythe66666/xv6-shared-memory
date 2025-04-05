void shm_ds_init(uint index, uint size, uint shmflag);
int shmmap(pde_t *pgdir, uint va, uint alloclist[], int max_phy_pages, int size, int perm, int remap);
int shmget(uint key, uint size, uint shmflag);
int shmat(uint shmid, uint shmaddr, uint shmflag);
int shmdt(void* addr);
void shm_unget(int index);
