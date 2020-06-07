/* Scalable logging library implementing the Carousel algorithm
 */

#include "carousel.hpp"

#include <cmath>

namespace carousel {

Carousel::Carousel(const LogCallback& callback,
                   size_t memorySize,
                   std::chrono::milliseconds collectionInterval)
  : m_callback(callback)
  , m_bloom(memorySize * 10)
  , m_memorySize(memorySize)
  , m_collectionInterval(collectionInterval)
  , m_phaseDuration(std::chrono::milliseconds(memorySize * collectionInterval.count()))
{
}

void
Carousel::log(const std::string& key, const std::string& entry)
{
  if (std::chrono::steady_clock::now() >= m_phaseStartTime + m_phaseDuration) {
    // Time to go to the next phase
    startNextPhase();
  }

  size_t hash = std::hash<std::string>{}(key);

  // Check if key matches the current phase
  if ((hash & m_kMask) == m_v) {
    // Check if likely (bloom filter) already stored this key this phase
    if (m_bloom.isEvidenced(key)) {
      // Skip since likely already logged this phase
      return;
    }

    m_bloom.add(key);
    m_nMatchingThisPhase++;

    // Check for bloom filter overflow
    if (isBloomFilterOverflowed()) {
      repartitionOverflow();
    }

    // Call callback to log this key+entry
    m_callback(key, entry);
  }
}

void
Carousel::reset()
{
  m_bloom.reset();
  m_k = 0;
  m_kMask = 0;
  m_v = 0;
  m_phaseStartTime = std::chrono::steady_clock::now();
}

void
Carousel::startNextPhase()
{
  // Check for bloom filter underflow
  if (isBloomFilterUnderflowed()) {
    repartitionUnderflow();
  }

  m_bloom.reset();
  m_v = (m_v + 1) % static_cast<size_t>(std::pow(2, m_k));
  m_phaseStartTime = std::chrono::steady_clock::now();
  m_nMatchingThisPhase = 0;
}

void
Carousel::repartitionOverflow()
{
  m_bloom.reset();
  m_k++;
  m_kMask += std::pow(2, m_k - 1);
  m_v = (m_v + 1) % static_cast<size_t>(std::pow(2, m_k));
  m_phaseStartTime = std::chrono::steady_clock::now();
}

void
Carousel::repartitionUnderflow()
{
  m_k--;
}

bool
Carousel::isBloomFilterOverflowed()
{
  return m_nMatchingThisPhase > m_memorySize;
}

bool
Carousel::isBloomFilterUnderflowed()
{
  return static_cast<double>(m_nMatchingThisPhase) < (static_cast<double>(m_memorySize) / m_x);
}

} // namespace carousel
