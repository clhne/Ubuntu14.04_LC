#include <stdio.h>
long filesize(FILE *stream)
{
    long curpos,length;
    curpos = ftell(stream);
    fseek(stream,0L,SEEK_END);
    length = ftell(stream);
//    fseek(stream,curpos,SEEK_END);
    return length;
}
int main()
{
    FILE *stream;
    stream = fopen("test.txt","w+");
    fprintf(stream,"This is a test.");
    printf("file sizeof test.txt is %ld bytes\n",filesize(stream));
    fclose(stream);
    return 0;
}
