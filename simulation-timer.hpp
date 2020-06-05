#ifndef CAROUSEL_SIMULATION_TIMER_HPP
#define CAROUSEL_SIMULATION_TIMER_HPP

#include <stdint.h>
#include "timer.hpp"

namespace carousel {

class SimulationTimer : public Timer<uint64_t, uint64_t>
{
public:
  SimulationTimer()
    : m_tick(0) {}

  inline uint64_t
  now() const
  {
    return m_tick;
  }

  inline uint64_t
  getPhase(uint64_t period, uint64_t interval) const
  {
    return period * interval;
  }

  inline void
  tick()
  {
    m_tick++;
  }

  inline void
  reset()
  {
    m_tick = 0;
  }
private:
  uint64_t m_tick;
};

}

#endif // CAROUSEL_SIMULATION_TIMER_HPP
