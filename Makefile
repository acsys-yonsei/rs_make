all: rs
rs: rs.o
	g++ -o rs rs.o
rs.o: rs_make.cpp
	g++ -std=c++11 -c -o rs.o rs_make.cpp

clean:
	rm rs.o rs