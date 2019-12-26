#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main(void)
{
        int shmid;
        void *shmaddr;
        int ret;
        printf("Hello, this is B process. give me the data.\n");
        while(1)
        {
                sleep(1);
        //get shared memory id
                shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);

                if(shmid == -1)
                {
                        perror("shared memory access is failed\n");
                        return 0;
                }

                //attach the shared memory
                shmaddr = shmat(shmid, (void*)0, IPC_CREAT|0666);
                if(shmaddr == (char*)-1)
                {
                        perror("attach failed\n");
                        return 0;
                }
                //printf("%d\n", strlen((char*)shmaddr));
                if(strlen((char*)shmaddr)>0)
                {
                        printf("%s", (char*)shmaddr);
                }


                ret = shmdt(shmaddr);
                if(ret == -1)
                {
                        perror("detach failed\n");
                        return 0;
                }

                //remove the shared memory
                ret = shmctl(shmid, IPC_RMID, 0);
                if(ret == -1)
                {
                        perror("remove failed\n");
                        return 0;
                }
        }
        return 0;
}
