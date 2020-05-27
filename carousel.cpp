/* Scalable logging library implementing the Carousel algorithm
 */

#include "carousel.hpp"

#include <cmath>

namespace carousel {

Carousel::Carousel(const LogCallback& callback,
                   size_t memorySize,
                   size_t collectionTime,
                   size_t highThreshold,
                   size_t bloomFilterSize)
  : m_callback(callback)
  , m_bloom(bloomFilterSize)
  , m_memorySize(memorySize)
  , m_collectionTime(collectionTime)
  , m_highThreshold(highThreshold)
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

    if (m_nMatchingThisPhase > m_highThreshold) {
      // We've exceeded the number that can match during this phase => repartition
      repartition();
      return;
    }
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
  m_bloom.reset();
  m_currentPhase = (m_currentPhase + 1) % static_cast<size_t>(std::pow(2, m_k - 1));
  m_nThisPhase = 0;
  m_nMatchingThisPhase = 0;
}

void
Carousel::repartition()
{
  m_k++;
  m_kMask += std::pow(2, m_k - 1);
  m_currentPhase = 0;
  m_nThisPhase = 0;
  m_nMatchingThisPhase = 0;
}

} // namespace carousel
