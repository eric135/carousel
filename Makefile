SRC := $(wildcard *.cpp)
HDR := $(wildcard *.hpp)
OBJ := $(SRC:.cpp=.o)
CXXFLAGS := -Wall -Werror -fpic -std=c++11 -g

# Users can adjust these variables to modify compilation or installation
# C++ compiler to use
CXX := g++
# Prefix to install under $(PREFIX)/include $(PREFIX)/lib
PREFIX := /usr/local

all: carousel

clean:
	rm -f libcarousel.so *.o

install:
	mkdir -p $(PREFIX)/include/carousel
	cp -v *.hpp $(PREFIX)/include/carousel
	mkdir -p $(PREFIX)/lib
	cp -v libcarousel.so $(PREFIX)/lib

carousel: $(OBJ) $(HDR)
	$(CXX) -o simpletest $(OBJ)

%.o: %.cpp $(HDR)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.PHONY: all clean install
