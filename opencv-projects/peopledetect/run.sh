rm CMakeCache.txt peopledetect cmake_install.cmake Makefile
rm -rf CMakeFiles
cmake .
make
./peopledetect man.jpg
