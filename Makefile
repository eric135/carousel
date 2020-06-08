HDR := $(wildcard *.hpp)

LIBCAROUSEL_SRC := carousel.cpp bloom.cpp
LIBCAROUSEL_HDR := $(LIBCAROUSEL_SRC:.cpp=.hpp)
LIBCAROUSEL_OBJ := $(LIBCAROUSEL_SRC:.cpp=.o)

CAROUSEL_TEST_SRC := carousel_test.cpp \
                     logger.cpp \
                     log-fetcher.cpp
CAROUSEL_TEST_HDR := logger.hpp log-fetcher.hpp
CAROUSEL_TEST_OBJ := $(CAROUSEL_TEST_SRC:.cpp=.o)


CXXFLAGS := -Wall -Werror -fpic -std=c++11 -g

# Users can adjust these variables to modify compilation or installation
# C++ compiler to use
CXX := g++
# Prefix to install under $(PREFIX)/include $(PREFIX)/lib
PREFIX := /usr/local

all: libcarousel.so carousel_test

clean:
	rm -f carousel_test libcarousel.so *.o

install:
	mkdir -p $(PREFIX)/include/carousel
	cp -v $(LIBCAROUSEL_HDR) $(PREFIX)/include/carousel
	mkdir -p $(PREFIX)/lib
	cp -v libcarousel.so $(PREFIX)/lib

libcarousel.so: $(LIBCAROUSEL_OBJ) $(LIBCAROUSEL_HDR)
	$(CXX) -shared -o $@ $(LIBCAROUSEL_OBJ)

carousel_test: $(LIBCAROUSEL_OBJ) $(CAROUSEL_TEST_OBJ) $(CAROUSEL_TEST_HDR)
	$(CXX) -o $@ $(CAROUSEL_TEST_OBJ) $(LIBCAROUSEL_OBJ) -pthread

%.o: %.cpp $(HDR)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.PHONY: all clean install
