#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MSG_SIZE 80
#define PIPENAME "./named_pipe_file"

int main(void)
{
        char msg[MSG_SIZE];
        int fd;
        int ret, i;
        char something[80];

        fd = open(PIPENAME, O_WRONLY);
        if(fd < 0)
        {
                printf("Open failed\n");
                return 0;
        }

        printf("Hello, this is A process. I'll give the data to B.\n");

        while(1)
        {
                sleep(1);
                fgets(something, 80, stdin);
                ret = write(fd, msg, sizeof(msg));
                if(ret < 0)
                {
                        printf("Write failed\n");
                        return 0;
                }
                if(strcmp(something, "bye")==0)
                {
                        printf("this message will be sent to b process..\n");
                        return 0;
                }
        }
        return 0;
}
