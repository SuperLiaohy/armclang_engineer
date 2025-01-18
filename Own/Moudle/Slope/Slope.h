//
// Created by lhy on 2024/9/15.
//

#ifndef NEW_PRJ_SLOPE_H
#define NEW_PRJ_SLOPE_H

struct slope_cfg {
    float step;
    float dead_zone;
};

class Slope {
public:
    Slope(float step, float dead_zone)
        : target(0)
        , step(step)
        , value(0)
        , dead_zone(dead_zone) {}

    explicit Slope(const slope_cfg& cfg)
        : target(0)
        , step(cfg.step)
        , value(0)
        , dead_zone(cfg.dead_zone) {}

    [[gnu::always_inline]] inline void target_set(float input);

    [[gnu::always_inline]] inline void step_set(float input);

    [[gnu::always_inline]] [[nodiscard]] inline float get() const;

    float update();

private:
    float target;
    float step;
    float value;
    float dead_zone;
};

[[gnu::always_inline]] inline void Slope::target_set(float input) { target = input; }
[[gnu::always_inline]] inline void Slope::step_set(float input) { step = input; }
[[gnu::always_inline]] [[nodiscard]] inline float Slope::get() const { return value; }

#endif //NEW_PRJ_SLOPE_H
