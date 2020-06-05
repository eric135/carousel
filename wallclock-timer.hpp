#ifndef CAROUSEL_WALLCLOCK_TIMER_HPP
#define CAROUSEL_WALLCLOCK_TIMER_HPP

#include <chrono>
#include "timer.hpp"

namespace carousel {

class WallclockTimer : public Timer<std::chrono::steady_clock::time_point,
                                    std::chrono::milliseconds>
{
public:
  inline std::chrono::steady_clock::time_point
  now() const
  {
    return std::chrono::steady_clock::now();
  }

  inline std::chrono::milliseconds
  getPhase(size_t period, std::chrono::milliseconds interval) const
  {
    return std::chrono::milliseconds(period * interval.count());
  }
};

}

#endif // CAROUSEL_WALLCLOCK_TIMER_HPP
