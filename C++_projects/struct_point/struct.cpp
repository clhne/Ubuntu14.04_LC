#include <iostream>
#include <stdlib.h>
using namespace std;
int main()
{
    struct point{
    int x;
    int y;
    }*ws;

    ws = (point *)malloc(sizeof(struct point));
    ws->x = 4;
    ws->y = 9;
    ws++;
    --ws;
    cout<<ws->x<<" "<<ws->y<<endl;

    return 0;
}
