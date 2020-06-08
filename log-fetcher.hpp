#ifndef CAROUSEL_LOG_FETCHER_HPP
#define CAROUSEL_LOG_FETCHER_HPP

#include <string>
#include <vector>
#include <random>

namespace carousel
{

class LogFetcher
{
public:
  virtual ~LogFetcher() {}

  virtual bool
  prepare() { return true; }

  virtual const std::string&
  fetch() = 0;
};

class RandomLogFetcher : public LogFetcher {
public:
  RandomLogFetcher(int keyRange);

  const std::string&
  fetch();

private:
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_distribution;
  std::vector<std::string> m_keylist;
};

}

#endif //
