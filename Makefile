CXX=c++

all: test

clean:
	rm -fv tests

test:
	${CXX} -Ofast -std=c++11 -DNO_EOSIO tests.cc -o tests
	./tests
