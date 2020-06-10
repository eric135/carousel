#include <functional>
#include <iostream>
#include <random>

#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "carousel.hpp"
#include "logger.hpp"
#include "log-fetcher.hpp"

using carousel::Carousel;
using carousel::Logger;
using carousel::LogFetcher;
using carousel::RandomLogFetcher;
using carousel::DatasetLogFetcher;

using std::placeholders::_1;
using std::placeholders::_2;

struct Options {
  int memorySize = 200;
  int logInterval = 10;
  int keyRange = 3000;
  int logPerTick = 3;
  int outputInterval = 200;
  int totalIteration = 50000;
  bool original = true;
  char *dataset = nullptr;
  int datasetSkip = 0;

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
      {"enhanced", no_argument, nullptr, 'e'},
      {"dataset", required_argument, nullptr, 'd'},
      {"dataset-skip", required_argument, nullptr, 'S'},
      {"help", no_argument, nullptr, 'h'},
      {nullptr, 0, nullptr, 0},
    };

    while ((ch = getopt_long(argc, argv,
                             "m:i:k:r:o:T:ed:S:h",
                             optlist, NULL)) != -1) {
      switch(ch) {
      case 'm': memorySize = atoi(optarg); break;
      case 'i': logInterval = atoi(optarg); break;
      case 'k': keyRange = atoi(optarg); break;
      case 'r': logPerTick = atoi(optarg); break;
      case 'o': outputInterval = atoi(optarg); break;
      case 'T': totalIteration = atoi(optarg); break;
      case 'e': original = false; break;
      case 'd': dataset = strdup(optarg); break;
      case 'S': datasetSkip = atoi(optarg); break;
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
    std::cerr << "-d, --dataset\tUse dataset file (Otherwise the random data generator will be used" << std::endl;
    std::cerr << "-e, --enhanced\tUse enhanced behavior, without wrapping v without 2^k (default: disabled)" << std::endl;
    std::cerr << "-S, --dataset-skip\tSkip number of lines in the dataset (default: 0)" << std::endl;
    std::cerr << "-h, --help\tThis help message" << std::endl;
  }
};

int main(int argc, char *argv[])
{
  Options o;
  if (o.parseArg(argc, argv)) {
    return 1;
  }

  Logger c(o.memorySize, std::chrono::milliseconds(o.logInterval));
  Logger n(o.memorySize, std::chrono::milliseconds(o.logInterval));
  Carousel carousel(std::bind(&Logger::log, &c, _1, _2),
                    o.memorySize,
                    std::chrono::milliseconds(o.logInterval),
                    o.original);

  std::shared_ptr<LogFetcher> fetcher;

  if (o.dataset != nullptr) {
    fetcher = std::make_shared<DatasetLogFetcher>(o.dataset, o.datasetSkip);
  } else {
    fetcher = std::make_shared<RandomLogFetcher>(o.keyRange);
  }

  if (!fetcher->prepare()) {
    return 1;
  }

  std::chrono::steady_clock::time_point log_time = std::chrono::steady_clock::now();
  c.run();
  n.run();
  for (int iter = 0; iter < o.totalIteration; iter++) {
    for (int i = 0; i < o.logPerTick; i++) {
      const std::string& k = fetcher->fetch();
      carousel.log(k, k);
      n.log(k, k);
    }

    if (iter % o.outputInterval == 0) {
      std::cout << iter << ":\tNaive: " << n.numRecordedKeys()
                << "\tCarousel: " << c.numRecordedKeys() << std::endl;
    }

    log_time += std::chrono::milliseconds(1);
    std::this_thread::sleep_until(log_time);
  }

  c.stop();
  n.stop();
  return 0;
}
