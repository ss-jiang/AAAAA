LDFLAGS=-pthread -lboost_system
CXXFLAGS=-Wall -Werror -std=c++0x
CP_LOC=config_parser/
GTEST_DIR=googletest/googletest
GMOCK_DIR=googletest/googlemock
TEST_FILES=session_test.cpp utils_test.cpp server_test.cpp session.cpp
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
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I ${GTEST_DIR} -isystem ${GMOCK_DIR}/include -I ${GMOCK_DIR} -pthread -c ${GMOCK_DIR}/src/gmock-all.cc

	#$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) -c $(GMOCK_DIR)/src/gmock_main.cc

	ar -rv libgtest.a gtest-all.o
	ar -rv libgmock.a gtest-all.o gmock-all.o
	#ar -rv gmock_main.a gmock_main.o gmock-all.o

	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread ${TEST_FILES} ${GTEST_DIR}/src/gtest_main.cc libgtest.a utils.o config_parser.o -o session_test -lboost_system
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread ${TEST_FILES} ${GTEST_DIR}/src/gtest_main.cc libgtest.a utils.o config_parser.o -o utils_test -lboost_system
	g++ -lboost_system -std=c++0x -isystem ${GMOCK_DIR}/include -isystem ${GTEST_DIR} -pthread ${TEST_FILES} libgmock.a libgtest.a utils.o config_parser.o -o server_test
clean:
	rm -f *.o web-server session_test utils_test server_test
