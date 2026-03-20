#pragma once

#include <cmath>

namespace program {
namespace cardanim {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct CardAnim {
  enum Type { NONE, BUMP, SHAKE, NUDGE };
  Type type = NONE;
  double t = 0;
  double duration = 0;
  double intensity = 1.0;
  double nudgeX = 0;
  double nudgeY = 0;
};

inline void
startBump(CardAnim& anim, double durationMs, double intensity = 1.0) {
  anim.type = CardAnim::BUMP;
  anim.t = 0;
  anim.duration = durationMs;
  anim.intensity = intensity;
}

inline void
startShake(CardAnim& anim, double durationMs, double intensity = 1.0) {
  anim.type = CardAnim::SHAKE;
  anim.t = 0;
  anim.duration = durationMs;
  anim.intensity = intensity;
}

inline void
startNudge(CardAnim& anim, double durationMs, double targetX, double targetY) {
  anim.type = CardAnim::NUDGE;
  anim.t = 0;
  anim.duration = durationMs;
  anim.intensity = 1.0;
  anim.nudgeX = targetX;
  anim.nudgeY = targetY;
}

inline void updateCardAnim(CardAnim& anim, int dt) {
  if (anim.type == CardAnim::NONE) {
    return;
  }
  anim.t += dt;
  if (anim.t >= anim.duration) {
    anim.type = CardAnim::NONE;
    anim.t = 0;
    anim.duration = 0;
  }
}

inline double getAnimScale(const CardAnim& anim) {
  if (anim.type != CardAnim::BUMP || anim.duration <= 0) {
    return 1.0;
  }
  double progress = anim.t / anim.duration;
  return 1.0 + anim.intensity * 0.2 * std::sin(M_PI * progress);
}

inline double getAnimRotation(const CardAnim& anim) {
  if (anim.type != CardAnim::SHAKE || anim.duration <= 0) {
    return 0.0;
  }
  double progress = anim.t / anim.duration;
  double decay = 1.0 - progress;
  return anim.intensity * 8.0 * std::sin(anim.t * 0.05) * decay;
}

inline double getAnimOffsetX(const CardAnim& anim) {
  if (anim.type != CardAnim::NUDGE || anim.duration <= 0) {
    return 0.0;
  }
  double progress = anim.t / anim.duration;
  return anim.nudgeX * std::sin(M_PI * progress);
}

inline double getAnimOffsetY(const CardAnim& anim) {
  if (anim.type != CardAnim::NUDGE || anim.duration <= 0) {
    return 0.0;
  }
  double progress = anim.t / anim.duration;
  return anim.nudgeY * std::sin(M_PI * progress);
}

} // namespace cardanim
} // namespace program
