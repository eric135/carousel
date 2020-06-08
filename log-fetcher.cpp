#include "log-fetcher.hpp"

namespace carousel
{

RandomLogFetcher::RandomLogFetcher(int keyRange)
  : m_distribution(0, keyRange - 1)
  , m_keylist(keyRange)
{
  for (int i = 0; i < keyRange; i++) {
    m_keylist[i] = std::to_string(i);
  }
}

const std::string&
RandomLogFetcher::fetch()
{
  int x = m_distribution(m_generator);
  return m_keylist[x];
}

}
