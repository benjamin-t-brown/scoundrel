#pragma once

namespace scoundrel {
namespace timer {

struct Timer {
  double duration = 0;
  double t = 0;
};

inline void start(Timer& timer, const double duration) {
  timer.t = 0;
  timer.duration = duration;
}

inline bool isComplete(const Timer& timer) { return timer.t >= timer.duration; }

inline bool isRunning(const Timer& timer) { return timer.t < timer.duration; }

inline void update(Timer& timer, const double dt) {
  if (isRunning(timer)) {
    timer.t += dt;
  }
}

} // namespace timer
} // namespace scoundrel