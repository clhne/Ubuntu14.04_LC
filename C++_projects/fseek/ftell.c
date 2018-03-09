#include <stdio.h>
int main()
{
    FILE * pFile;
    long size;
    pFile = fopen("test.txt","rb");
    if(pFile == NULL)
        perror("Error Opening file");
    else
    {
        fseek(pFile,0,SEEK_END);
        size = ftell(pFile);
        fclose(pFile);
        printf("size of text.txt: %ld bytes. \n",size);
    }
    return 0;
}
