#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct msgbuf
{
        long msgtype;
        char mtext[80];
};

int main(void)
{
        key_t key_id;
        int i = 0;
        struct msgbuf sndbuf;
        char puts[80];

        printf("Hello this is A process. I'll give the data to B.\n");
        while(1)
        {
                fgets(puts,80,stdin);
                key_id = msgget((key_t)1234, IPC_CREAT|0666);

                if (key_id == -1 )
                {
                        perror("msgget error : ");
                        return 0;
                }

                sndbuf.msgtype = 3;
                strcpy(sndbuf.mtext, puts);

                if(msgsnd( key_id, (void*)&sndbuf, sizeof(struct msgbuf), IPC_NOWAIT) == -1)
                {
                        perror("msgsnd error : ");
                }
                if(!strcmp(puts, "bye"))
                {
                        printf("this message will be sent to b process..\n");
                        return 0;
                }
        }
        return 0;
}
