#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define COUNTING_NUMBER 200000
int shmid1, shmid2, shmid3;
int *shmaddr1, *shmaddr2, *shmaddr3;

int main(void){
    pid_t pid1, pid2;
    int status1, status2;
    
    shmid1 = shmget((key_t)1224, sizeof(int) * 2, IPC_CREAT|0666); // flag
    shmid2 = shmget((key_t)1246, sizeof(int) * 1, IPC_CREAT|0666); // turn
    shmid3 = shmget((key_t)1288, sizeof(int) * 1, IPC_CREAT|0666); // critical_variable
    
    int *init = (int*)shmat(shmid3, NULL, 0);
    int one = 0, two = 1;
    
    if((pid1=fork())==-1){
        printf("fork failed.\n");
    }
    else if( pid1 == 0 ) {
        execl("child1", NULL);
    }
    else{ pid2 = fork();}
    
    if(pid2==-1){
        printf("fork failed.\n");
    }
    else if( pid2 == 0 ){
        execl("child2", NULL);
    }
    else{
        waitpid(pid1, &status1, 0);
        waitpid(pid2, &status2, 0);
        printf("Actual Count: %d | Expected Count: %d\n", *init, COUNTING_NUMBER * 2);
        int chk1 = shmctl(shmid1,IPC_RMID, 0);
        int chk2 = shmctl(shmid2,IPC_RMID, 0);
        int chk3 = shmctl(shmid3,IPC_RMID, 0);
    }
    return 0;
}
