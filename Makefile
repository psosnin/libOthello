CXX := clang++
CFLAGS := -O3 -std=c++20
INCLUDE := -Iinclude
PYINCLUDE := -I/usr/include/python3.9
GTEST := -lgtest
BOOST := -lboost_python3

TEST_SRC := $(wildcard test/*.cpp)
TEST := othello_test

LIB_SRC := $(wildcard src/*.cpp)
WRAPPER := src/boost/py_wrapper.cpp
LIB := lib/libothello.so
PYLIB := lib/pythello.so


.PHONY: all clean pythello test

all: pythello

pythello: $(PYLIB)
	rm -rf ../lib
	cp -rf lib ../lib

test: $(TEST)

$(PYLIB): $(LIB) $(LIB_SRC) $(WRAPPER)
	$(CXX) $(CFLAGS) -shared $(INCLUDE) $(PYINCLUDE) -fPIC $(WRAPPER) $(LIB) -o $(PYLIB) $(BOOST)

$(LIB): $(LIB_SRC) lib
	$(CXX) $(CFLAGS) -shared $(INCLUDE) -fPIC $(LIB_SRC) -o $(LIB)

$(TEST): $(TEST_SRC) $(LIB)
	$(CXX) $(CFLAGS) -g $(INCLUDE) $(GTEST) $(TEST_SRC) $(LIB) -o $(TEST)

lib: 
	@mkdir lib

clean: 
	rm -rf lib
	rm -f othello_test