FRONTEND_SRC := $(wildcard *.cpp)
FRONTEND_HDR := $(wildcard *.hpp) \
                $(wildcard ../*.hpp)
FRONTEND_OBJ := $(FRONTEND_SRC:.cpp=.o) \
                $(wildcard ../*.o)


CXXFLAGS := -I.. -Wall -Werror -fpic -std=c++11 -g

# Users can adjust these variables to modify compilation or installation
# C++ compiler to use
CXX := g++
# Prefix to install under $(PREFIX)/include $(PREFIX)/lib
PREFIX := /usr/local

all: carousel_test

clean:
	rm -f carousel_test *.o

install:
	mkdir -p $(PREFIX)/bin
	cp -v carousel_test $(PREFIX)/bin


carousel_test: $(FRONTEND_OBJ) $(FRONTEND_HDR)
	$(CXX) -o $@ $(FRONTEND_OBJ) -pthread

%.o: %.cpp $(HDR)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.PHONY: all clean install
