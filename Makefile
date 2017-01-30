LDFLAGS=-pthread -lboost_system
CXXFLAGS=-Wall -Werror -std=c++0x
CP_LOC=config_parser/
GTEST_DIR=googletest/googletest
TEST_FILES=echo_test.cpp session.cpp

all: server.o session.o main.o config_parser.o
	g++ -o web-server main.o server.o session.o config_parser.o $(LDFLAGS) $(CXXFLAGS)

server.o: server.cpp server.h
	g++ -c server.cpp $(LDFLAGS) $(CXXFLAGS)

session.o: session.cpp session.h
	g++ -c session.cpp $(LDFLAGS) $(CXXFLAGS)

config_parser.o: $(CP_LOC)config_parser.cc $(CP_LOC)config_parser.h
	g++ -c $(CP_LOC)config_parser.cc -g $(CXXFLAGS)

main.o: main.cpp server.h session.h $(CP_LOC)config_parser.h
	g++ -c main.cpp $(LDFLAGS) $(CXXFLAGS)

.PHONY: clean, all, test

test:
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread ${TEST_FILES} ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o echo_test -lboost_system

clean:
	rm -f *.o web-server echo_test




