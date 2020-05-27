/* Scalable logging library implementing the Carousel algorithm
 */

#include "carousel.hpp"

#include <cmath>

namespace carousel {

Carousel::Carousel(const LogCallback& callback,
                   size_t memorySize,
                   size_t collectionTime,
                   size_t highThreshold,
                   size_t lowThreshold,
                   size_t bloomFilterSize)
  : m_callback(callback)
  , m_bloom(bloomFilterSize)
  , m_memorySize(memorySize)
  , m_collectionTime(collectionTime)
  , m_highThreshold(highThreshold)
  , m_lowThreshold(lowThreshold)
{
  // TODO: Is this the correct way to determine the phase duration?
  m_phaseDuration = static_cast<double>(collectionTime) / static_cast<double>(memorySize);
}

void
Carousel::log(const std::string& key, const std::string& entry)
{
  size_t hash = std::hash<std::string>{}(key);

  // Check if entry matches the current phase
  if ((hash & m_kMask) == m_currentPhase) {

    // Check if likely (bloom filter) already stored this key this phase
    if (m_bloom.isEvidenced(key)) {
      // Skip since likely already logged this phase
      return;
    }

    m_bloom.add(key);
    m_nMatchingThisPhase++;

    // Call callback to log this entry
    m_callback(key, entry);
  }

  m_nThisPhase++;

  if (m_nThisPhase >= m_phaseDuration) {
    // Time to go to the next phase
    startNextPhase();
  }
}

void
Carousel::reset()
{
  m_bloom.reset();
  m_k = 1;
  m_kMask = 1;
  m_currentPhase = 0;
  m_nThisPhase = 0;
  m_nMatchingThisPhase = 0;
}

void
Carousel::startNextPhase()
{
  if (m_nMatchingThisPhase > m_highThreshold) {
    // We've exceeded the number that can match during this phase => repartition high
    repartitionHigh();
  }
  else if (m_nMatchingThisPhase < m_lowThreshold) {
    // We've not met the minimum number that should match during this phase => repartition low
    repartitionLow();
  }
  else {
    m_bloom.reset();
    m_currentPhase = (m_currentPhase + 1) % static_cast<size_t>(std::pow(2, m_k - 1));
    m_nThisPhase = 0;
    m_nMatchingThisPhase = 0;
  }
}

void
Carousel::repartitionHigh()
{
  m_bloom.reset();
  m_k++;
  m_kMask += std::pow(2, m_k - 1);
  m_currentPhase = 0;
  m_nThisPhase = 0;
  m_nMatchingThisPhase = 0;
}

void
Carousel::repartitionLow()
{
  m_bloom.reset();
  m_k--;
  m_kMask -= std::pow(2, m_k);
  m_currentPhase = 0;
  m_nThisPhase = 0;
  m_nMatchingThisPhase = 0;
}

} // namespace carousel
