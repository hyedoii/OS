#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

#define COUNTING_NUMBER 2000000

#define ID 0

int critical_section_variable = 0;
int turn = 0;
int flag[2] = {1,0};
int shmid1, shmid2, shmid3, shmid4;
char *shmaddr1, *shmaddr2, *shmaddr3, *shmaddr4;
int ret1, ret2, ret3, ret4;

void lock(int self, int *flag, int *turn){
	flag[self] = 1;
	*turn = (self+1)%2;
	printf("A lock turn : %d\n" , *turn);
	while(flag[*turn] && *turn == (self+1)%2);
}

void unlock(int self, int*flag)
{
	printf("A : unlocking\n");
	flag[self] = 0;
}

void getSharedMemory(int* shmid, key_t key)
{
	*shmid = shmget((key_t)key, 1024, IPC_CREAT|0666);
	printf("A gSM shmid : %d\n", *shmid);
	if(*shmid == -1)
	{
		perror("A : shared memory access is failed.\n");
	}
}

void attach(char* shmaddr, int* shmid)
{
	shmaddr = shmat(*shmid, NULL , IPC_CREAT|0666);
	printf("A attach : %p\n)", shmaddr);
	if(shmaddr == "-1")
	{
		perror("A : attach failed\n");
	}
}

void detach(char* shmaddr, int* ret)
{
	*ret = shmdt(shmaddr);
	printf("A detach : %s\n", shmaddr);
	if( *ret == -1)
	{
		perror("A : detach failed\n");
	}
}

/*
   void *func(int id)
   {
   int i;
   for( i = 0 ; i <COUNTING_NUMBER; i++)
   {
   lock(ID, flag, &turn);
   critical_section_variable++;
   printf("A : %d\n", critical_section_variable);
   unlock(ID , flag);
   }
   }
 */
int main(void)
{
	int local_count = 0;
	int i;
	char *temp;
	for( i = 0 ; i < COUNTING_NUMBER; i++)
	{
		printf("A : %dth loop\n", i);
		//              lock(ID, flag, &turn);

		shmid1 = shmget((key_t)1224, 1024, IPC_CREAT|0666);
		shmid2 = shmget((key_t)1246, 1024, IPC_CREAT|0666);
		shmid3 = shmget((key_t)1288, 1024, IPC_CREAT|0666);
		shmid4 = shmget((key_t)1296, 1024, IPC_CREAT|0666);

		//              getSharedMemory(&shmid1, 1224);
		//              getSharedMemory(&shmid2, 1246);
		//              getSharedMemory(&shmid3, 1288);
		//              getSharedMemory(&shmid4, 1296);

		printf("1\n");

		shmaddr1 = shmat(shmid1, (void*)0, IPC_CREAT|0666);
		shmaddr2 = shmat(shmid2, (void*)0, IPC_CREAT|0666);
		shmaddr3 = shmat(shmid3, (void*)0, IPC_CREAT|0666);
		shmaddr4 = shmat(shmid4, (void*)0, IPC_CREAT|0666);

		printf("%s\n%s\n%s\n%s\n", shmaddr1, shmaddr2, shmaddr3, shmaddr4);

		//              attach(shmaddr1, &shmid1);
		//              attach(shmaddr2, &shmid2);
		//              attach(shmaddr3, &shmid3);
		//              attach(shmaddr3, &shmid4);
		printf("2");
		//              lock(ID , flag, &turn);

		flag[0] = atoi(shmaddr1);
		flag[1] = atoi(shmaddr2);
		turn = atoi(shmaddr3);
		critical_section_variable = atoi(shmaddr4);

		printf("3");
		lock(ID, flag, &turn);

		printf("A : locking\n");
		flag[ID] = 1;
		printf("A : flag[%d] %d \n",ID , flag[ID]);
		turn = (ID+1)%2;
		printf("A: locking turn : %d\n", turn);
		while(flag[turn] && turn == (ID+1)%2);

		printf("A : locking success");
		sprintf(temp, "%d", flag[0]);
		strcpy(shmaddr1, temp);
		sprintf(temp , "%d", flag[1]);
		strcpy(shmaddr2, temp);
		sprintf(temp, "%d", turn);
		strcpy(shmaddr3, temp);

		critical_section_variable++;

        sprintf(temp, "%d", critical_section_variable);
		strcpy(shmaddr4, temp);

		//              unlock(ID, flag);

		sprintf(temp, "%d", flag[0]);
		strcpy(shmaddr1, temp);

		local_count++;
		printf("4");
		//              strcpy(shmaddr1, (char*)flag[0]);
		//              strcpy(shmaddr2, (char*)flag[1]);
		//              strcpy(shmaddr3, (char*)turn);
		//              strcpy(shmaddr4, (char*)critical_section_variable);

		printf("5");
		ret1 = shmdt(shmaddr1);
		ret2 = shmdt(shmaddr2);
		ret3 = shmdt(shmaddr3);
		ret4 = shmdt(shmaddr4);
		//              detach(shmaddr1, &ret1);
		//              detach(shmaddr2, &ret2);
		//              detach(shmaddr3, &ret3);
		//              detach(shmaddr4, &ret4);

		//unlock(ID, flag);
		sleep(1);
	}
	return 0;
}

