#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#define COUNTING_NUMBER 2000000

#define ID 1

int critical_section_variable = 0;
int turn = 0;
int flag[2] = {1,1};
int shmid1, shmid2, shmid3, shmid4;
char *shmaddr1, *shmaddr2, *shmaddr3, *shmaddr4;
int ret1, ret2, ret3, ret4;

void lock(int self, int *flag, int *turn){
	printf("B : locking\n");
	flag[self] = 1;
	printf("B : flag[%d] %d \n",self , flag[self]);
	*turn = (self+1)%2;
	printf("B: locking turn : %d\n", *turn);
	while(flag[*turn] && *turn == (self+1)%2);
}

void unlock(int self, int*flag)
{
	printf("B : unlocking\n");
	flag[self] = 0;
}

void getSharedMemory(int* shmid, key_t key)
{
	*shmid = shmget((key_t)key, 1024, IPC_CREAT|0666);
	printf("B gSM : shmid %d\n", *shmid);
	if(*shmid == -1)
	{
		perror("B : shared memory access is failed.\n");
	}
}

void attach(char* shmaddr, int* shmid)
{
	shmaddr = shmat(*shmid, NULL , IPC_CREAT|0666);
	printf("B attach : %p\n", shmaddr);
	if(shmaddr == "-1")
	{
		perror("B : attach failed\n");
	}
}
void detach(char* shmaddr, int* ret)
{
	*ret = shmdt(shmaddr);
	if( *ret == -1)
	{
		perror("B : detach failed\n");
	}
}

/*
   void *func(int ID)
   {
   int i;
   for( i = 0 ; i <COUNTING_NUMBER; i++)
   {
   lock(ID, flag, &turn);
   critical_section_variable++;
   printf("B : %d\n", critical_section_variable);
   unlock(ID , flag);
   }
   }
 */
int main(void)
{
	int local_count = 0;
	int i;
	char* temp;
	for( i = 0 ; i < COUNTING_NUMBER ; i++)
	{
		sleep(1);
		printf("B : %dth loop\n", i);
		printf("flag[0] : %d \n",flag[0]);
		printf("flag[1] : %d \n", flag[1]);

		printf("B : locking success\n");

		shmid1 = shmget((key_t)1224, 1024, IPC_CREAT|0666);
		shmid2 = shmget((key_t)1246, 1024, IPC_CREAT|0666);
		shmid3 = shmget((key_t)1288, 1024, IPC_CREAT|0666);
		shmid4 = shmget((key_t)1296, 1024, IPC_CREAT|0666);

		printf("B : getSharedMemory success\n");

		shmaddr1 = shmat(shmid1, (void*)0 , IPC_CREAT|0666);
		shmaddr2 = shmat(shmid2, (void*)0 , IPC_CREAT|0666);
		shmaddr3 = shmat(shmid3, (void*)0 , IPC_CREAT|0666);
		shmaddr4 = shmat(shmid4, (void*)0 , IPC_CREAT|0666);

		printf("%s\n%s\n%s\n%s\n", shmaddr1, shmaddr2, shmaddr3, shmaddr4);

		printf("B : attach success\n");
		printf("B flag[0] : %d\n", flag[0]);
		printf("B flag[1] : %d\n", flag[1]);
		printf("B *shmaddr1 : %p\n", shmaddr1);

		printf("HELLO~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

		flag[0] = atoi(shmaddr1);
		printf("B flag[0] : %d\n", flag[0]);
		flag[1] = atoi(shmaddr2);
		printf("B flag[1] : %d\n", flag[1]);
		turn = atoi(shmaddr3);
		printf("B turn : %d\n", turn);
		critical_section_variable = atoi(shmaddr4);
		printf("B critcal : %d\n", critical_section_variable);

		//              lock(ID, flag, &turn);

		printf("B : locking\n");
		flag[ID] = 1;
		printf("B : flag[%d] %d \n",ID , flag[ID]);
		turn = (ID+1)%2;
		printf("B: locking turn : %d\n", turn);
		while(flag[turn] && turn == (ID+1)%2);

		printf("Yop!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        
		sprintf(temp, "%d", flag[0]);
		strcpy(shmaddr1, temp);

		sprintf(temp, "%d", flag[1]);
		strcpy(shmaddr2, temp);

		sprintf(temp, "%d", turn);
		strcpy(shmaddr3, temp);

		printf("HPOOOOOOOOOOOOOOOO\n");
		//              unlock(ID, flag);

		flag[ID] = 0;

		sprintf(temp, "%d", flag[ID]);
		strcpy(shmaddr2, temp);

		printf("B : %d\n" , critical_section_variable);
		local_count++;
		printf("B : %d\n", local_count);

		//              strcpy(shmaddr1, (char*)flag[0]);
		//              strcpy(shmaddr2, (char*)flag[1]);
		//              strcpy(shmaddr3, &turn);
		//`             strcpy(shmaddr4, &critical_section_variable);

		ret1 = shmdt(shmaddr1);
		ret2 = shmdt(shmaddr2);
		ret3 = shmdt(shmaddr3);
		ret4 = shmdt(shmaddr4);


		//              detach(shmaddr1, &ret1);
		//              detach(shmaddr2, &ret2);
		//              detach(shmaddr3, &ret3);
		//              detach(shmaddr4, &ret4);

		//unlock(ID, flag);

	}
	return 0;
}
