#include <iostream>
#include <stdlib.h>
using namespace std;
int main()
{
    struct point{
    int x;
    int y;
    }*ws;
    int a[5] = {1,2,3,4,5};
    int m = 10;
    int* dpp = &a[2];
    for(int i = 0; i < 5;i++)
        cout<<"a["<<i<<"] = "<<a[i]<<endl;
    cout<<endl;
    cout<<"a[1] = "<<a[1]<<endl;
    cout<<"a[+1] = "<<a[+1]<<endl;
    cout<<"if int* dpp = &a[2],\nso dpp[+2] = "<<dpp[+2]<<endl; 

    cout<<"m = 10, --m = "<<--m<<endl;
    cout<<m<<endl;
    cout<<"m = 10, m-- = "<<m--<<endl;
    cout<<m<<endl;
    ws = (point *)malloc(sizeof(struct point));
    ws->x = a[1];
    ws->y = a[3];
//    ws++;
    cout<<ws->x<<" "<<ws->y<<endl;

    return 0;
}
