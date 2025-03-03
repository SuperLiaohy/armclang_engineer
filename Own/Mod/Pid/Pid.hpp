//
// Created by liaohy on 9/5/24.
//
#pragma once

namespace pid_dep {
    struct Parameters {
        float p;
        float i;
        float d;
    };

    struct Limitation {
        float min;
        float max;
    };
}

struct pid_cfg {
    float p;
    float i;
    float d;
    float maxI;
    float maxOut;
    float dead_zone;

    constexpr pid_cfg(float p, float i, float d, float maxI, float maxOut, float dead_zone = 0)
        : p(p), i(i), d(d), maxI(maxI), maxOut(maxOut), dead_zone(dead_zone) {
    }

    constexpr pid_cfg(const pid_cfg &cfg) = default;
};

class Pid {
private:
    using Parameters = pid_dep::Parameters;
    using Limitation = pid_dep::Limitation;

public:
    Pid() = default;

    Pid(const float p, const float i, const float d, const float max_i, const float max_out, const float dead_zone = 0)
        : parameters{p, i, d},
          item{0, 0, 0},
          i{-max_i, max_i},
          out{-max_out, max_out},
          err(0),
          last_err(0),
          output(0),
          dead_zone(dead_zone),
          target(0) {
    }

    explicit Pid(const pid_cfg &cfg)
        : parameters{cfg.p, cfg.i, cfg.d},
          item{0, 0, 0},
          i{-cfg.maxI, cfg.maxI},
          out{-cfg.maxOut, cfg.maxOut},
          err(0),
          last_err(0),
          output(0),
          dead_zone(cfg.dead_zone),
          target(0) {
    }

    Parameters parameters;
    Parameters item;

    Limitation i;
    Limitation out;

    float err;
    float last_err;

    float output;

    float dead_zone;
    float target;

    float update(float target, float feedback);

    float update(float feedback);

    void set(const float p, const float i, const float d, const float max_i, const float max_out) {
        this->parameters.p = p;
        this->parameters.i = i;
        this->parameters.d = d;
        this->i.min = -max_i;
        this->i.max = max_i;
        this->out.min = -max_out;
        this->out.max = max_out;
    }

    void clear() {
        err = 0;
        last_err = 0;
        item.p = 0;
        item.d = 0;
        item.i = 0;
        output = 0;
    }
};
