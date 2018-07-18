CXX=c++

all: test

clean:
	rm -fv tests

test:
	${CXX} -std=c++11 -Wall -Ofast -DNO_EOSIO tests.cc -o tests
	./tests
