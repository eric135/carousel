SRC := $(wildcard *.cpp)
HDR := $(SRC:.cpp=.hpp)
OBJ := $(SRC:.cpp=.o)


CXXFLAGS := -Wall -Werror -fpic -std=c++11 -g -I.

# Users can adjust these variables to modify compilation or installation
# C++ compiler to use
CXX := g++
# Make to use
MAKE := make
# Prefix to install under $(PREFIX)/include $(PREFIX)/lib
PREFIX := /usr/local

all: libcarousel.so
	$(MAKE) -C frontend

clean:
	rm -f libcarousel.so *.o
	$(MAKE) -C frontend clean

install:
	mkdir -p $(PREFIX)/lib
	cp -v libcarousel.so $(PREFIX)/lib
	mkdir -p $(PREFIX)/include/carousel
	cp -v $(HDR) $(PREFIX)/include/carousel
	$(MAKE) -C frontend install

libcarousel.so: $(OBJ) $(HDR)
	$(CXX) -shared -o $@ $(OBJ)

%.o: %.cpp $(HDR)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.PHONY: all clean install
