LDFLAGS=-pthread -lboost_system
CXXFLAGS=-Wall -Werror -std=c++0x
CP_LOC=config_parser/
GTEST_DIR=googletest/googletest
GMOCK_DIR=googletest/googlemock
SESSION_TEST=tests/session_test.cpp session.cpp
UTILS_TEST=tests/utils_test.cpp 
SERVER_TEST=tests/server_test.cpp
PARSER_TEST=tests/config_parser_test.cc
CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include

all: server.o session.o main.o config_parser.o utils.o
	g++ -o web-server main.o server.o session.o config_parser.o utils.o $(LDFLAGS) $(CXXFLAGS)

server.o: server.cpp server.h
	g++ -c server.cpp $(LDFLAGS) $(CXXFLAGS)

session.o: session.cpp session.h
	g++ -c session.cpp $(LDFLAGS) $(CXXFLAGS)

config_parser.o: $(CP_LOC)config_parser.cc $(CP_LOC)config_parser.h
	g++ -c $(CP_LOC)config_parser.cc -g $(CXXFLAGS)

utils.o: utils.cpp utils.h
	g++ -c utils.cpp $(LDFLAGS) $(CXXFLAGS)

main.o: main.cpp server.h session.h $(CP_LOC)config_parser.h utils.h
	g++ -c main.cpp $(LDFLAGS) $(CXXFLAGS)

.PHONY: clean, all, test

test: 
#server_test.cpp session_test.cpp server.cpp ${GTEST_DIR}/src/gtest-all.cc session.cpp ${GTEST_DIR}/src/gtest_main.cc ${GMOCK_DIR}/src/gmock-all.cc
	#g++ server_test.cpp session_test.cpp server.cpp -isystem ${GTEST_DIR}/include \
	#-I${GTEST_DIR} ${GTEST_DIR}/src/gtest-all.cc session.cpp ${GTEST_DIR}/src/gtest_main.cc
	#-isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} ${GMOCK_DIR}/src/gmock-all.cc \
	#std=c++0x -g -Wall -lboost_regex -lboost_system -lpthread -o mytest

	g++ -c $(CP_LOC)config_parser.cc -g $(CXXFLAGS)
	g++ -c utils.cpp $(LDFLAGS) $(CXXFLAGS)
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} -pthread -c ${GMOCK_DIR}/src/gmock-all.cc

	#$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) -c $(GMOCK_DIR)/src/gmock_main.cc

	ar -rv libgtest.a gtest-all.o
	ar -rv libgmock.a gtest-all.o gmock-all.o
	#ar -rv gmock_main.a gmock_main.o gmock-all.o

	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread ${SESSION_TEST} ${GTEST_DIR}/src/gtest_main.cc libgtest.a utils.o config_parser.o -o session_test -lboost_system
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread ${UTILS_TEST} ${GTEST_DIR}/src/gtest_main.cc libgtest.a utils.o config_parser.o -o utils_test -lboost_system
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread ${PARSER_TEST} ${GTEST_DIR}/src/gtest_main.cc libgtest.a utils.o config_parser.o -o config_parser_test -lboost_system
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread ${SERVER_TEST} ${GTEST_DIR}/src/gtest_main.cc libgtest.a utils.o config_parser.o -o server_test -lboost_system
	#g++ -lboost_system -std=c++0x -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -pthread ${SERVER_TEST} libgtest.a libgmock.a utils.o config_parser.o -o server_test
clean:
	rm -f *.o web-server session_test utils_test server_test
