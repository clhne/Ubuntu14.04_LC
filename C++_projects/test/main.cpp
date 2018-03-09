#if(1)
	
#include <iostream>
using namespace std;

int main()
{
        cout<<endl;
        cout<<"*****************************\n";
        cout<<"          Author LuCL       \n \n";
	cout<<"Hello CMake!"<<endl;
	cout<<"Please modify this source file~\nThank you~"<<endl;
        cout<<"\n*****************************\n";
	cout<<endl;
	
	return 0;
}

#endif

#if(0)
#include <iostream>
using namespace std;
int main() {

#ifdef _WIN64
	cout << "_WIN64 is defined as " << _WIN64 << endl;
#endif

#ifdef _WIN32
	cout << "_WIN32 is defined as " << _WIN32 << endl;
#endif

	cin.get();
	return 0;
}

#endif
