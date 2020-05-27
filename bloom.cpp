/* Scalable logging library implementing the Carousel algorithm
 */

#include "bloom.hpp"

#include <functional>

namespace carousel {

Bloom::Bloom(size_t nBits)
  : m_nBits(nBits)
  , m_bits(nBits, false)
{
}

void
Bloom::add(const std::string& key)
{
  m_bits[hash1(key)] = true;
  m_bits[hash2(key)] = true;
  m_bits[hash3(key)] = true;
  m_bits[hash4(key)] = true;
  m_bits[hash5(key)] = true;
}

bool
Bloom::isEvidenced(const std::string& key) const
{
  return m_bits[hash1(key)] &&
         m_bits[hash2(key)] &&
         m_bits[hash3(key)] &&
         m_bits[hash4(key)] &&
         m_bits[hash5(key)];
}

void
Bloom::reset()
{
  m_bits.clear();
  m_bits.assign(m_nBits, false);
}

size_t
Bloom::hash1(const std::string& key) const
{
  return std::hash<std::string>{}(key) % m_nBits;
}

size_t
Bloom::hash2(const std::string& key) const
{
  size_t sum = 0;
  for (char c : key) {
    sum += static_cast<size_t>(c);
  }
  return sum % m_nBits;
}

size_t
Bloom::hash3(const std::string& key) const
{
  size_t sum = 0;
  for (size_t i = 0; i < key.size(); i += 2) {
    sum += static_cast<size_t>(key[i]);
  }
  return sum % m_nBits;
}

size_t
Bloom::hash4(const std::string& key) const
{
  size_t sum = 0;
  for (size_t i = 1; i < key.size(); i += 2) {
    sum += static_cast<size_t>(key[i]);
  }
  return sum % m_nBits;
}

size_t
Bloom::hash5(const std::string& key) const
{
  size_t sum = 0;
  for (size_t i = key.size() / 2; i < key.size(); i++) {
    sum += static_cast<size_t>(key[i]);
  }
  return sum % m_nBits;
}

} // namespace carousel
