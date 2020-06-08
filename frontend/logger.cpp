#include <functional>
#include <iostream>

#include "logger.hpp"

namespace carousel
{

Logger::Logger(size_t memorySize,
               std::chrono::milliseconds collectionInterval)
  : m_memorySize(memorySize)
  , m_interval(collectionInterval)
{
}

void
Logger::log(const std::string& key, const std::string& content)
{
  std::unique_lock<std::mutex> lock(m_queue_mutex);
  bool was_empty = m_logging_queue.empty();
  if (m_logging_queue.size() < m_memorySize) {
    m_logging_queue.push_back(key);
    if (was_empty) {
      m_queue_cond.notify_one();
    }
  }
}

void
Logger::run()
{
  m_lastLog = std::chrono::steady_clock::now();
  m_stop = false;
  m_log_thread = new std::thread(std::bind(&Logger::thread, this));
}

void
Logger::stop()
{
  {
  std::unique_lock<std::mutex> lock(m_queue_mutex);
  m_stop = true;
  m_queue_cond.notify_one();
  }
  m_log_thread->join();
  delete m_log_thread;
}

size_t
Logger::numRecordedKeys()
{
  std::unique_lock<std::mutex> lock(m_queue_mutex);
  return m_db.size();
}

void
Logger::processLog()
{
  std::unique_lock<std::mutex> lock(m_queue_mutex);

  // because of the sleep, stop() may have been called before
  // entering this function
  if (m_stop) {
    return;
  }

  while (m_logging_queue.empty()) {
    m_queue_cond.wait(lock);
    if (m_stop) {
      return;
    }
  }
  m_lastLog = std::chrono::steady_clock::now();
  m_db.insert(m_logging_queue.front());
  m_logging_queue.pop_front();
}

void
Logger::thread()
{
  while (!m_stop) {
    std::this_thread::sleep_until(m_lastLog + m_interval);
    processLog();
  }
}

}
