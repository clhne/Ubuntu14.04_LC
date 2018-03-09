out:spimpl.o utils.o
	g++ spimpl.o utils.o -o sp
spimpl.o:spimpl.cpp
	g++ -c spimpl.cpp -o spimpl.o
utils.o:utils.cpp
	g++ -c utils.cpp -o utils.o
clean:
	rm -rf *.o out

