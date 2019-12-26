#include <stdio.h>
#include <unistd.h>

int main(void) {
    int i;

    for( i = 0 ; i < 12; ++i) {
        FILE* fp = fopen("text.txt", "w");
        fprintf(fp, "%d\n" , i);
        printf("Child1 wrote %d.\n" , i);
        fclose(fp);
        sleep(1);
    }
    return 0;
}
