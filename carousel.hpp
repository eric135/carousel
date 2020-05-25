/* Scalable logging library implementing the Carousel algorithm
 */

#ifndef CAROUSEL_CAROUSEL_HPP
#define CAROUSEL_CAROUSEL_HPP

#include <functional>
#include <optional>
#include <ostream>

namespace carousel {

class Carousel
{
public:
  typedef std::function<void(const std::string&)> LogCallback;

public:
  /**
   * \brief Creates an instance of Carousel that outputs to the specified callback
   */
  Carousel(const LogCallback& callback);

  /**
   * \brief Submit the specified entry to Carousel
   * \param key Logging key
   * \param entry Entry for log for the given key
   */
  void
  log(const std::string& key, const std::string& entry);

  /**
   * \brief Resets Carousel
   */
  void
  reset();

private:
  LogCallback m_callback;
};

} // namespace carousel

#endif // CAROUSEL_CAROUSEL_HPP
