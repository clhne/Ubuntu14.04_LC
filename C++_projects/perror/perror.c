#include <stdio.h>
int main()
{
    FILE *fp;
    fp = fopen("test.txt","r++");
    if(NULL == fp)
    {
        perror("test.txt ");
    }
    else
        printf("no error!\n");
    perror("No Error ");
    return 0;
}
