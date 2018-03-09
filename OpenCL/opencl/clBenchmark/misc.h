
#include <string>
#include <sstream>
#include <vector>

using namespace std;

vector<string> splitWhite( string in );

// text file input
char * readTextFile(const char * filename);
char * readBinaryFile(const char * filename, int * filesize);
bool listDir(string dir, vector<string> &files);


// processing style functions
vector<string> * loadStrings(const char * filename);
vector<string> * split(string input, string delimeter);
float pmap(float i, float a1, float b1, float a2, float b2);
string trim( const string & instr );

// get file extension or name
string fileExtension(string input);
string getFileName(string input);

// template convertor from string to other stuff
template <class T>
bool from_string(T& t, 
                 const std::string& s)
{
  std::istringstream iss(s);
  return !(iss >> t).fail();
}

inline float sign(float x) { return x >= 0.0f ? 1.0f : -1.0f; }

