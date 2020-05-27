/* Scalable logging library implementing the Carousel algorithm
 */

#ifndef CAROUSEL_BLOOM_HPP
#define CAROUSEL_BLOOM_HPP

#include <string>
#include <vector>

namespace carousel {

class Bloom
{
public:
  /**
   * \brief Creates a bloom filter with the given number of bits
   */
  Bloom(size_t nBits);

  /**
   * \brief Adds a key to the bloom filter
   */
  void
  add(const std::string& key);

  /**
   * \brief Checks whether the existence of the specified key is evidenced by the bloom filter
   */
  bool
  isEvidenced(const std::string& key) const;

  /**
   * \brief Resets all bits stored in the bloom filter
   */
  void
  reset();

private:
  size_t
  hash1(const std::string& key) const;

  size_t
  hash2(const std::string& key) const;

  size_t
  hash3(const std::string& key) const;

  size_t
  hash4(const std::string& key) const;

  size_t
  hash5(const std::string& key) const;

private:
  size_t m_nBits;
  std::vector<bool> m_bits;
};

} // namespace carousel

#endif // CAROUSEL_BLOOM_HPP
