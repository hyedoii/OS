#include <stdio.h>
#include <unistd.h>

int main(void) {
    char gets[100], *string;
    int i;

    for(i=0; i<7; i++) {
        FILE* fp = fopen("text.txt", "r");
        if((string = fgets(gets, 10, fp))!=NULL){ printf("%s", string); }
        fclose(fp);
        sleep(2);
    }
    return 0;
}
