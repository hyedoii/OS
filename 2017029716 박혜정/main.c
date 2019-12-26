#include <stdio.h>
#include <unistd.h>

int main(void){
    pid_t pid1, pid2;
    int status1, status2;

    printf("Waiting for Child Processes.\n");

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
    }
    return 0;
}
