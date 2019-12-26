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

int main(int argc, char **argv)
{
        key_t key_id;
        int i = 0;
        struct msgbuf rsvbuf;
        int msgtype = 3;

        printf("Hello, this is B process. give me the data.\n");
        while(1)
        {
                key_id = msgget((key_t)1234, IPC_CREAT|0666);

                if( key_id == -1)
                {
                        perror("msgget error : ");
                        return 0;
                }

                if(msgrcv( key_id, (void*)&rsvbuf, sizeof(struct msgbuf), msgtype, 0) == -1)
                {
                        perror("msgrcv error : ");
                }
                else
                {
                        printf("%s\n", rsvbuf.mtext);
                }
        }
        return 0;
}
