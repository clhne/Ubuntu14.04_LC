/* ------------------------------------
 *
 * Ant3dine
 * misc.cpp
 * ------------------------------------
 */
 
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

using namespace std;

bool listDir(string dir, vector<string> &files)
{
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(dir.c_str())) == NULL) 
	{
		cerr << "Error(" << errno << ") opening " << dir << endl;
		return false;
	}

	while ((dirp = readdir(dp)) != NULL) 
	{
		if (  strcasecmp( dirp->d_name, "." ) == 0 || strcasecmp( dirp->d_name, ".." ) == 0 )
		{
			// ignore "." and ".."
			continue;
		}
		
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return true;
}

vector<string> splitWhite( string in )
{
	vector<string> tokens;
	string temp = "";
	int bracketLevel = 0;
	
	for (int i = 0; i < in.size(); i++)
	{
		char c = in.at( i );
		
		// look at brackets
		/*
		if ( c == '(' )
		{
			bracketLevel++;
		}
		else if ( c == ')' )
		{
			bracketLevel--;
		}
		*/
		
		if ( (c == '\t' || c == ' ') && bracketLevel == 0 )
		{
			// add whatever temp we have
			if (temp.size() > 0)
			{
				tokens.push_back( temp );
				temp = "";
			}
		}
		else
		{
			// add to temp
			temp += c;
		}
	}
	
	if (temp.size() > 0) {
		tokens.push_back( temp );
	}
	
	return tokens;	
}

char * readTextFile(const char * filename)
{
	int length;
	char * buffer;

	ifstream input;
	input.open(filename, ios::in);
	
	if (! input.is_open() )
	{
		cerr << "Could not open text file: " << filename << endl;
		return NULL;
	}
	
	input.seekg(0, ios::end);
	length = input.tellg();
	input.seekg(0, ios::beg);
	
	buffer = new char[length + 1];
	input.read(buffer, length);
	buffer[length] = '\0';
	
	input.close();
	return buffer;
}

char * readBinaryFile(const char * filename, int * fileSize)
{
	int length;
	char * buffer;

	ifstream input;
	input.open(filename, ios::in | ios::binary);
	
	if (! input.is_open() )
	{
		//cerr << "Could not open binary file: " << filename << endl;;
		return NULL;
	}
	
	input.seekg(0, ios::end);
	length = input.tellg();
	input.seekg(0, ios::beg);
	
	buffer = new char[length];
	input.read(buffer, length);
	
	input.close();
	*fileSize = length;
	return buffer;
}

vector<string> * loadStrings(const char * filename)
{
	ifstream input;
	input.open(filename, ios::in);
	
	if (! input.is_open() )
	{
		cerr << "Could not open text file: " << filename << endl;;
		return NULL;
	}
	
	vector<string> * theFile = new vector<string>;
	while (! input.eof() )
	{
		string line;
		getline(input, line);
		theFile->push_back(line);
	}
	
	input.close();
	return theFile;
}

vector<string> * split(string input, string delimeter)
{
	size_t lastPos = 0;
	
	vector<string> * tokens = new vector<string>;
	
	while (lastPos < input.length() - 1)
	{
		size_t res = input.find(delimeter, lastPos);
		if (res == string::npos) {
			res = input.length() - 1;
		}
		else
		{
			res--;
		}
		
		tokens->push_back( input.substr( lastPos, res - lastPos + 1 ) );
		lastPos = res + 1 + delimeter.length();
	}
	return tokens;
}

float pmap(float i, float a1, float b1, float a2, float b2)
{
	// normalize
	float n = (i - a1) / (b1 - a1);
	return n * (b2 - a2) + a2;
}

string trim( const string & instr )
{
	string str = instr;

	// Trim Both leading and trailing spaces
	size_t startpos = str.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces
	size_t endpos = min( 
		str.find_last_not_of("\r"),
		str.find_last_not_of(" \t")
	); // Find the first character position from reverse af

	// if all spaces or empty return an empty string
	if(( string::npos == startpos ) || ( string::npos == endpos))
	{
		str = "";
	}
	else
	{
		str = str.substr( startpos, endpos-startpos+1 );
	}
	return str;
}

string fileExtension(string input)
{
	if (input.length() == 0) {
		return "";
	}
	
	int i = input.length();
	
	while ( i >= 0 )
	{
		i--;
		if (input[i] == '.')
		{
			return input.substr(i + 1);
		}
	}
	
	return "";
}


string getFileName(string input)
{
	if (input.length() == 0) {
		return "";
	}
	
	int i = input.length();
	
	while ( i >= 0 )
	{
		i--;
		if (input[i] == '.')
		{
			return input.substr(0, i+1);
		}
	}
	
	return "";
}


