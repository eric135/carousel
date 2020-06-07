/* Scalable logging library implementing the Carousel algorithm
 */

#include "carousel.hpp"

#include <cmath>

namespace carousel {

template class Carousel<uint64_t, uint64_t>;
template class Carousel<std::chrono::steady_clock::time_point,
                        std::chrono::milliseconds>;

template<class T, class I>
Carousel<T,I>::Carousel(const LogCallback& callback,
                        size_t memorySize,
                        const Timer<T,I>& timer,
                        I collectionInterval)
  : m_callback(callback)
  , m_bloom(memorySize * 10)
  , m_timer(timer)
  , m_memorySize(memorySize)
  , m_collectionInterval(collectionInterval)
  , m_phaseDuration(timer.getPhase(memorySize, collectionInterval))
{
}

template<class T, class I>
void
Carousel<T,I>::log(const std::string& key, const std::string& entry)
{
  if (m_timer.now() >= m_phaseStartTime + m_phaseDuration) {
    // Time to go to the next phase
    startNextPhase();
  }

  size_t hash = std::hash<std::string>{}(key);

  // Check if key matches the current phase
  if ((hash & m_kMask) == (m_v & m_kMask)) {
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

template<class T, class I>
void
Carousel<T,I>::reset()
{
  m_bloom.reset();
  m_k = 0;
  m_kMask = 0;
  m_v = 0;
  m_phaseStartTime = m_timer.now();
}

template<class T, class I>
void
Carousel<T,I>::startNextPhase()
{
  // Check for bloom filter underflow
  if (isBloomFilterUnderflowed()) {
    repartitionUnderflow();
  }

  m_bloom.reset();
  m_v++;
  m_phaseStartTime = m_timer.now();
  m_nMatchingThisPhase = 0;
}

template<class T, class I>
void
Carousel<T,I>::repartitionOverflow()
{
  m_bloom.reset();
  m_k++;
  m_kMask = std::pow(2, m_k) - 1;
  m_v++;
  m_phaseStartTime = m_timer.now();
  m_nMatchingThisPhase = 0;
}

template<class T, class I>
void
Carousel<T,I>::repartitionUnderflow()
{
  if (m_k > 0) {
    m_k--;
    m_kMask = std::pow(2, m_k) - 1;
  }
}

template<class T, class I>
bool
Carousel<T,I>::isBloomFilterOverflowed()
{
  return m_nMatchingThisPhase > m_memorySize;
}

template<class T, class I>
bool
Carousel<T,I>::isBloomFilterUnderflowed()
{
  return static_cast<double>(m_nMatchingThisPhase) < (static_cast<double>(m_memorySize) / m_x);
}

} // namespace carousel
