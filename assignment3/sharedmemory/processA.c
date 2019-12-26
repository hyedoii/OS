#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main(void)
{
        int shmid;
        int i;
        void *shmaddr;
        int ret;
        char puts[200];
        printf("Hello, this is A process. I'll give the data to B.\n");
        while(1)
        {
                fgets(puts, 200, stdin);
                //make a shared memory
                shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);

                if(shmid < 0)
                {
                        perror("shmget");
                        return 0;
                }

                //attach the shared memory
                shmaddr = shmat(shmid, (void*)0, IPC_CREAT|0666);
                if(shmaddr == (char*)-1)
                {
                        perror("attach failed\n");
                        return 0;
                }

                strcpy((char*)shmaddr, puts);
            
                //detach the shared memory
                shmdt(shmaddr);

                if(ret == -1)
                {
                        perror("detach failed\n");
                        return 0;
                }
                if(!strcmp(puts, "bye\n"))
                {
                        printf("this message will be sent to b process..\n");
                        return 0;
                }
                sleep(1);
        }
        return 0;
}
