CXX := clang++
CFLAGS := -O3 -std=c++20 -g
INCLUDE := -Iinclude
GTEST := -lgtest

TEST_SRC := $(wildcard tests/*.cpp)
TEST := bin/othello-test

LIB_SRC := $(wildcard src/utils/*.cpp)
LIB := lib/libothello.so

SRC := $(wildcard src/*.cpp)
EXECS := $(patsubst src/%.cpp, bin/%,  $(SRC))

.PHONY: all clean

all: $(EXECS) $(TEST)

lib/libothello.so: $(LIB_SRC)
	$(CXX) $(CFLAGS) -shared $(INCLUDE) -fPIC $(LIB_SRC) -o $(LIB)

$(EXECS): bin/% : src/%.cpp lib/libothello.so
	$(CXX) $(CFLAGS) $(INCLUDE) $(LIB) -o $@ $<

$(TEST): $(TEST_SRC) lib/libothello.so
	$(CXX) $(CFLAGS) $(INCLUDE) $(GTEST) $(TEST_SRC) $(LIB) -o $(TEST)

clean: 
	rm -f obj/*
	rm -f bin/*
	rm -f lib/*

