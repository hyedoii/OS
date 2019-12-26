#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARGUMENT_NUMBER 20

//long long result = 0;
long long reee[ARGUMENT_NUMBER];
void *ThreadFunc(void*n)
{
        long long result = 0;
        long long i;
        long long number = *((long long *)n);
        printf("number = %lld\n", number);
        for ( i = 0 ; i < 25000000; i++)
        {
                //result에 number 더해서 저장.
                result += number;
        }
        // 결과갑 배열에 저장
        reee[number] = result;
}

int main(void)
{
        pthread_t threadID[ARGUMENT_NUMBER];
        long long argument[ARGUMENT_NUMBER];
        long long i;
        long long aaa = 0;
        //int status;

        for(i = 0 ; i < ARGUMENT_NUMBER; i++)
        {
                //thread 생성
                argument[i] = i;
                pthread_create(&threadID[i], NULL, ThreadFunc, (void*)&argument[i]);
        }

        printf("Main Thread is waiting for Child Thread!\n");

        for(i = 0 ; i < ARGUMENT_NUMBER; i++)
        {
                //thread 종료를 기다린다.
                pthread_join(threadID[i], NULL);
        }

        for ( i = 0; i <ARGUMENT_NUMBER; i++)
        {
                //각 쓰레드의 결과값들을 합친다.
                aaa += reee[i];
        }

        //합친 값 출력
        printf("result = %lld\n", aaa);

        return 0;
}
