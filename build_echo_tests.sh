#!/bin/bash

GTEST_DIR=googletest/googletest
case `uname` in
  Linux)
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
    ar -rv libgtest.a gtest-all.o
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread echo_test.cpp session.cpp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o echo_test -lboost_system;;
  Darwin)
    clang++ -std=c++11 -stdlib=libc++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
    ar -rv libgtest.a gtest-all.o
    clang++ -std=c++11 -stdlib=libc++ -isystem ${GTEST_DIR}/include -pthread echo_test.cpp session.pp ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o echo_test -lboost_system ;;
  *)
    echo "Unknown operating system." ;;
esac
