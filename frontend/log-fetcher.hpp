#ifndef CAROUSEL_LOG_FETCHER_HPP
#define CAROUSEL_LOG_FETCHER_HPP

#include <string>
#include <vector>
#include <random>
#include <fstream>

namespace carousel
{

class LogFetcher
{
public:
  virtual ~LogFetcher() {}

  virtual bool
  prepare() { return true; }

  virtual const std::string
  fetch() = 0;
};

class RandomLogFetcher : public LogFetcher {
public:
  RandomLogFetcher(int keyRange);

  const std::string
  fetch();

private:
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_distribution;
  std::vector<std::string> m_keylist;
};

class DatasetLogFetcher : public LogFetcher {
public:
  DatasetLogFetcher(const char *fileName, int skip);

  bool
  prepare();

  const std::string
  fetch();

private:
  const char *m_fileName;
  int m_skip;
  std::ifstream m_ifs;
};

}

#endif //
