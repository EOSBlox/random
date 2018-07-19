CXX=c++
FLAGS=-std=c++14 -Wall -Ofast -DNO_EOSIO

all: tests speed

clean:
	rm -f tests speed

tests: tests.cc Random.h
	${CXX} ${FLAGS} tests.cc -o tests

speed: speed.cc Random.h
	${CXX} ${FLAGS} speed.cc -o speed
