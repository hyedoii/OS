#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define COUNTING_NUMBER 100

int cur_writer = 0;
int cur_count = 0;
int rear = -1;
int front = -1;
//int cur_count = 0;

sem_t wc; // writer count
sem_t rc; // reader count
int wcVal = 0;
int rcVal = 0;

void *writer(int* name)
{
        int i;
        int count = 0;
        for(i=0; i<COUNTING_NUMBER;i++){
                usleep(1000);
                sem_getvalue(&wc, &wcVal);
                // check if there is another writer
                if(wcVal == 2){
                        sem_wait(&wc);

                        //critical section
                        cur_writer = *name;
                        count++;
                        cur_count = count;
                        printf("writer id : writer%d | cur_count : %d\n", cur_writer, cur_count);

                        sem_post(&wc);
                }
        }
}

void *reader(int* name)
{
        int i, data, dataCount;

        usleep(300);
        printf("reader%d in\n", *(int*)name);

        sem_wait(&rc);

        //critical section
        data = cur_writer;
        dataCount = cur_count;

        sem_post(&rc);
        printf("\t\treader - cur_writer : %d | cur_count :  %d\n", data, dataCount);

}
int main(void)
{
        int i;
        int w1 = 1;
        int w2 = 2;
        int r1 = 1;
        int r2 = 2;
        int r3 = 3;
        int r4 = 4;
        int r5 = 5;
    
        //do not share the semaphore with other processes
        sem_init(&rc, 1, 5);
    
        //share the semaphore with other processes
        sem_init(&wc, 0, 2);

        pthread_t writer1, writer2, reader1, reader2, reader3, reader4, reader5;
        //create writers
        pthread_create(&writer1, NULL, (void*)writer, &w1);
        pthread_create(&writer2, NULL, (void*)writer, &w2);

        //create readers
        pthread_create(&reader1, NULL, (void*)reader, &r1);
        pthread_create(&reader2, NULL, (void*)reader, &r2);
        pthread_create(&reader3, NULL, (void*)reader, &r3);
        pthread_create(&reader4, NULL, (void*)reader, &r4);
        pthread_create(&reader5, NULL, (void*)reader, &r5);

        pthread_join(writer1, NULL);
        pthread_join(writer2, NULL);
        pthread_join(reader1, NULL);
        pthread_join(reader2, NULL);
        pthread_join(reader3, NULL);
        pthread_join(reader4, NULL);
        pthread_join(reader5, NULL);

        sem_destroy(&rc);
        sem_destroy(&wc);

        return 0;
}
