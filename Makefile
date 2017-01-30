LDFLAGS=-pthread -lboost_system
CXXFLAGS=-Wall -Werror -std=c++0x
CP_LOC=config_parser/

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
	./build_echo_tests.sh

clean:
	rm *.o web-server
