/* Scalable logging library implementing the Carousel algorithm
 */

#include "carousel.hpp"

namespace carousel {

Carousel::Carousel(const LogCallback& callback)
  : m_callback(callback)
{
}

void
Carousel::log(const std::string& key, const std::string& entry)
{
  // TODO
}

void
Carousel::reset()
{
  // TODO
}

} // namespace carousel
