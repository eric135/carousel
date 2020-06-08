#include <iostream>
#include <sstream>

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

const std::string
RandomLogFetcher::fetch()
{
  int x = m_distribution(m_generator);
  return m_keylist[x];
}

DatasetLogFetcher::DatasetLogFetcher(const char *fileName, int skip)
  : m_fileName(fileName)
  , m_skip(skip)
{
}

bool
DatasetLogFetcher::prepare()
{
  m_ifs.open(m_fileName);
  if (!m_ifs.is_open()) {
    std::cerr << "Cannot open file: " << m_fileName << std::endl;
    return false;
  }

  for (int i = 0; i < m_skip; i++) {
    std::string s;
    std::getline(m_ifs, s);
  }

  if (!m_ifs.good()) {
    std::cerr << "File status is not correct after initialization" << std::endl;
    return false;
  }

  return true;
}

const std::string
DatasetLogFetcher::fetch()
{
  std::string line;
  std::string s;

  std::getline(m_ifs, line);
  std::istringstream iss(line);

  iss >> s;
  iss >> s;
  iss >> s;

  return s;
}

}
