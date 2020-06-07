#include <functional>

#include "simlogger.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

SimLogger::SimLogger(const carousel::SimulationTimer& timer,
                     size_t memorySize,
                     uint64_t collectionInterval)
  : m_timer(timer)
  , m_memorySize(memorySize)
  , m_interval(collectionInterval)
  , m_lastLog(0)
{
}

void
SimLogger::logCallback(const std::string& key, const std::string& content)
{
  if (m_logging_queue.size() < m_memorySize) {
    m_logging_queue.push_back(key);
  }
}

void
SimLogger::processLog()
{
  uint64_t now = m_timer.now();
  if (!m_logging_queue.empty() && now - m_lastLog >= m_interval) {
    m_db.insert(m_logging_queue.front());
    m_logging_queue.pop_front();
    m_lastLog = now;
  }
}

NaiveLogger::NaiveLogger(const carousel::SimulationTimer& timer,
                         size_t memorySize,
                         uint64_t collectionInterval)
  : SimLogger(timer, memorySize, collectionInterval)
{
}

void
NaiveLogger::inputData(const std::string& key, const std::string& content)
{
  logCallback(key, content);
}

CarouselLogger::CarouselLogger(const carousel::SimulationTimer& timer,
                               size_t memorySize,
                               uint64_t collectionInterval)
  : SimLogger(timer, memorySize, collectionInterval)
  , m_carousel(bind(&CarouselLogger::logCallback, this, _1, _2),
               memorySize,
               timer,
               collectionInterval)
{
  m_carousel.reset();
}

void
CarouselLogger::inputData(const std::string& key, const std::string& content)
{
  m_carousel.log(key, content);
}
