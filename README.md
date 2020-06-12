# Carousel scalable logging library for C++

This is an implementation of the Carousel scalable logging algorithm in C++.
This algorithm was proposed by Lam, et al. in "Carousel: Scalable Logging for Intrusion Prevention Systems" (NSDI 2010).

## Dependencies

To build and use this library, one needs a compiler compatible with ISO-standardized C++11 and a make tool compatible with GNU-style Makefiles.
No external libraries are required.

## Building the library

This library can be built and installed by running the following commands:

```
make
sudo make install
```

By default, we assume that use is using GCC and that the library should be installed under `/usr/local`.
These settings can be changed by editing the `CXX` and `PREFIX` variables in Makefile, respectively.

## Using the library

To include this library, in your source code, include `carousel/carousel.hpp`.

To link with this library, specify `-lcarousel` in your LDFLAGS.

## Using the frontend test program

This repository also contains a test frontend as a simple demonstration the Carousel algorithm. It is located in the `frontend` folder. It can either use randomly generated data (default) or datasets provided in the `test-data` folder (use `-d` argument). Refer to `./frontend/carousel_test --help` for detailed usage.