#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <atomic>

#define COUNTING_NUMBER 2000000

using namespace std;

int critical_section_variable = 0;
std::atomic<int> turn;
std::atomic<int> flag[2] = {{0},{0}};

void lock(int self)
{
        flag[self].store(1);
        turn.store((self+1)%2);
        while(flag[turn].load() && turn.load() == (self+1)%2);
}

void unlock(int self)
{
        flag[self].store(0);
}

void *func(void *s)
{
        int* thread_num = (int*)s;
        int i;
        for(i=0;i<COUNTING_NUMBER; i++)
        {
                lock(*thread_num);
                critical_section_variable++;
                unlock(*thread_num);
        }
}

int main(void)
{
        pthread_t p1, p2;

        int parameter[2] = {0,1};

        pthread_create(&p1, NULL, func, (void*)&parameter[0]);
        pthread_create(&p2, NULL, func, (void*)&parameter[1]);

        pthread_join(p1, NULL);
        pthread_join(p2, NULL);

        printf("Actual Count: %d | Expected Count: %d\n", critical_section_variable, COUNTING_NUMBER*2);

        return 0;
}
