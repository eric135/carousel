#ifndef CAROUSEL_TIMER_HPP
#define CAROUSEL_TIMER_HPP

namespace carousel {

template<class T, class I>
class Timer
{
public:
  virtual T
  now() const = 0;

  virtual I
  getPhase(size_t period, I interval) const = 0;
};

}

#endif // CAROUSEL_TIMER_HPP
