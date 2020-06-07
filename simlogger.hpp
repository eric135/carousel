#ifndef CAROUSEL_SIMLOGGER_HPP
#define CAROUSEL_SIMLOGGER_HPP

#include <deque>
#include <unordered_set>

#include "carousel.hpp"
#include "simulation-timer.hpp"

extern template class carousel::Carousel<uint64_t,uint64_t>;
typedef carousel::Carousel<uint64_t,uint64_t> SimCarousel;

class SimLogger
{
public:
  SimLogger(const carousel::SimulationTimer& timer,
            size_t memorySize,
            uint64_t collectionInterval);

  void
  logCallback(const std::string& key, const std::string& content);

  void
  processLog();

  virtual void
  inputData(const std::string& key, const std::string& content) = 0;

  inline size_t
  recorded_keys() const
  {
    return m_db.size();
  }

private:
  const carousel::SimulationTimer& m_timer;
  std::deque<std::string> m_logging_queue;
  std::unordered_set<std::string> m_db;
  size_t m_memorySize;
  uint64_t m_interval;
  uint64_t m_lastLog;
};

class NaiveLogger : public SimLogger
{
public:
  NaiveLogger(const carousel::SimulationTimer& timer,
              size_t memorySize,
              uint64_t collectionInterval);

  void
  inputData(const std::string& key, const std::string& content);
};

class CarouselLogger : public SimLogger
{
public:
  CarouselLogger(const carousel::SimulationTimer& timer,
                 size_t memorySize,
                 uint64_t collectionInterval);

  void
  inputData(const std::string& key, const std::string& content);

private:
  SimCarousel m_carousel;
};

#endif // CAROUSEL_SIMLOGGER_HPP
