#ifndef CAROUSEL_LOGGER_HPP
#define CAROUSEL_LOGGER_HPP

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>

namespace carousel {

class Logger
{
public:
  Logger(size_t memorySize,
         std::chrono::milliseconds collectionInterval);

  Logger(const Logger&) = delete; // non construction-copyable
  Logger& operator=(const Logger&) = delete; // non copyable

  /**
   * \brief Insert data into logging queue
   */
  void
  log(const std::string& key, const std::string& content);

  /**
   * \brief start processing in a separate thread
   */
  void
  run();

  /**
   * \brief stop processing
   */
  void
  stop();

  size_t
  numRecordedKeys();

private:
  void
  processLog();

  void
  thread();

private:
  size_t m_memorySize;
  std::chrono::milliseconds m_interval;
  std::chrono::steady_clock::time_point m_lastLog;

  std::deque<std::string> m_logging_queue;
  std::unordered_set<std::string> m_db;

  std::thread *m_log_thread;

  std::mutex m_queue_mutex;
  std::condition_variable m_queue_cond;
  std::mutex m_db_mutex;

  bool m_stop;
};

}

#endif // CAROUSEL_LOGGER_HPP
