/* Scalable logging library implementing the Carousel algorithm
 */

#ifndef CAROUSEL_CAROUSEL_HPP
#define CAROUSEL_CAROUSEL_HPP

#include "bloom.hpp"

#include <chrono>
#include <functional>
#include <string>

namespace carousel {

class Carousel
{
public:
  typedef std::function<void(const std::string&, const std::string&)> LogCallback;

public:
  /**
   * \brief Creates an instance of Carousel that outputs to the specified callback
   * \param memorySize Number of sources that can be logged
   * \param collectionTime Time logging will run for
   * \param highThreshold Upper bound on number of matching keys within a phase before repartitioning
   * \param lowThreshold Lower bound on number of matching keys within a phase before repartitioning
   * \param bloomFilterBits Number of bits to use in bloom filter. Greater values can increase accuracy
   */
  Carousel(const LogCallback& callback,
           size_t memorySize,
           std::chrono::milliseconds collectionTime,
           size_t bloomFilterBits = 5000);

  /**
   * \brief Submit the specified entry to Carousel
   * \param key Logging key
   * \param entry Entry for log for the given key
   *
   * Whether Carousel logs this entry depends upon whether it falls into the current phase
   */
  void
  log(const std::string& key, const std::string& entry);

  /**
   * \brief Reset Carousel
   */
  void
  reset();

private:
  void
  startNextPhase();

  void
  repartitionOverflow();

  void
  repartitionUnderflow();

  bool
  isBloomFilterOverflowed();

  bool
  isBloomFilterUnderflowed();

private:
  LogCallback m_callback;
  Bloom m_bloom;
  const double m_x = 2.3;

  size_t m_memorySize;
  std::chrono::milliseconds m_collectionTime;
  std::chrono::milliseconds m_phaseDuration;

  size_t m_k = 0;
  size_t m_kMask = 0;
  size_t m_v = 0;
  std::chrono::steady_clock::time_point m_phaseStartTime;
  size_t m_nMatchingThisPhase = 0;
};

} // namespace carousel

#endif // CAROUSEL_CAROUSEL_HPP
