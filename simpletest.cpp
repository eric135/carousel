#include <iostream>
#include <string>
#include <random>

#include <getopt.h>
#include <stdlib.h>

#include "carousel.hpp"
#include "simlogger.hpp"

struct Options {
  int memorySize = 5;
  int logInterval = 50;
  int keyRange = 100;
  int logPerTick = 10;
  int outputInterval = 200;
  int totalIteration = 10000;

  int parseArg(int argc, char *argv[])
  {
    int ch;
    static struct option optlist[] = {
      {"memory", required_argument, nullptr, 'm'},
      {"interval", required_argument, nullptr, 'i'},
      {"key", required_argument, nullptr, 'k'},
      {"lograte", required_argument, nullptr, 'r'},
      {"output", required_argument, nullptr, 'o'},
      {"iteration", required_argument, nullptr, 'T'},
      {"help", no_argument, nullptr, 'h'},
      {nullptr, 0, nullptr, 0},
    };

    while ((ch = getopt_long(argc, argv,
                             "m:i:k:r:o:T:h",
                             optlist, NULL)) != -1) {
      switch(ch) {
      case 'm': memorySize = atoi(optarg); break;
      case 'i': logInterval = atoi(optarg); break;
      case 'k': keyRange = atoi(optarg); break;
      case 'r': logPerTick = atoi(optarg); break;
      case 'o': outputInterval = atoi(optarg); break;
      case 'T': totalIteration = atoi(optarg); break;
      case 'h': printHelp(); return 1;
      default:
        std::cerr << "Unrecognized argument" << std::endl;
        printHelp();
        return 1;
      }
    }
    return 0;
  }

private:
  void printHelp()
  {
    std::cerr << "simpletest [OPTIONS]\n" << std::endl;
    std::cerr << "-m, --memory\tBuffer size of logger (default: 5)" << std::endl;
    std::cerr << "-i, --interval\tTicks between logger process a log (default: 50)" << std::endl;
    std::cerr << "-k, --key\tNumber of keys (default: 100)" << std::endl;
    std::cerr << "-r, --lograte\tNumbers of log generated per tick (default: 10)" << std::endl;
    std::cerr << "-o, --output\tNumbers of ticks per output (default: 200)" << std::endl;
    std::cerr << "-T, --iteration\tTotal numbers of iteration to run (default: 10000)" << std::endl;
    std::cerr << "-h, --help\tThis help message" << std::endl;
  }
};

int main(int argc, char* argv[])
{
  Options o;
  if (o.parseArg(argc, argv)) {
    return 1;
  }

  std::vector<std::string> keylist(o.keyRange);
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, o.keyRange - 1);
  carousel::SimulationTimer timer;
  CarouselLogger c(timer, o.memorySize, o.logInterval);
  NaiveLogger n(timer, o.memorySize, o.logInterval);

  for (int i = 0; i < o.keyRange; i++) {
    keylist[i] = std::to_string(i);
  }

  for (int iter = 0; iter < o.totalIteration; iter++) {  
    timer.tick();

    for (int t = 0; t < o.logPerTick; t++) {
      int x = distribution(generator);
      const std::string& k = keylist[x];
      c.inputData(k, k);
      n.inputData(k, k);
    }

    c.processLog();
    n.processLog();

    if (iter % o.outputInterval == 0) {
      std::cout << iter << ":\tNaive: " << n.recorded_keys()
                << "\tCarousel: " << c.recorded_keys() << std::endl;
    }
  }

  return 0;
}
