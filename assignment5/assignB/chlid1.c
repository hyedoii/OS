#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

#define COUNTING_NUMBER 200000

#define ID 0

int *critical_section_variable;
int *turn;
int *flag;
int shmid1, shmid2, shmid3;
int *shmaddr1, *shmaddr2, *shmaddr3;
int ret1, ret2, ret3;

int main(void)
{
    int local_count = 0;
    int i;
    
    shmid1 = shmget((key_t)1224, sizeof(int) * 2, IPC_CREAT|0666);
    shmid2 = shmget((key_t)1246, sizeof(int) * 1, IPC_CREAT|0666);
    shmid3 = shmget((key_t)1288, sizeof(int) * 1, IPC_CREAT|0666);
    
    shmaddr1 = (int*)shmat(shmid1, NULL, 0);
    shmaddr2 = (int*)shmat(shmid2, NULL, 0);
    shmaddr3 = (int*)shmat(shmid3, NULL, 0);
    
    flag = shmaddr1;
    turn = shmaddr2;
    critical_section_variable = shmaddr3;
    
    for(i=0; i<COUNTING_NUMBER; ++i){
        flag[ID] = 1;
        *turn = (ID+1)%2;
        
        while(flag[*turn] && *turn == (ID+1)%2);
        
        shmaddr1 = flag;
        shmaddr2 = turn;
        
        (*critical_section_variable)++;
        local_count++;
        shmaddr2 = critical_section_variable;
        flag[ID] = 0;
        shmaddr1 = flag;
    }
    printf("thread num : %d ,local count %d\n", ID, local_count);
    printf("child finish!\n");
    ret1 = shmdt(shmaddr1);
    ret2 = shmdt(shmaddr2);
    ret3 = shmdt(shmaddr3);
    sleep(1);
    
    return 0;
}
