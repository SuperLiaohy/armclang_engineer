//
// Created by liaohy on 9/5/24.
//

#include "Pid.hpp"

#include <MyMath/MyMath.hpp>


float Pid::update(const float target, const float feedback) {
    this->target = target;

    err = target - feedback;

    item.p = err * parameters.p;
    item.i = limited(err * parameters.i + item.i, i.min, i.max);
    item.d = (err - last_err) * parameters.d;

    output = limited(item.p + item.i + item.d, out.min, out.max);

    last_err = err;

    return output;
}

float Pid::update(const float feedback) {
    err = target - feedback;

    item.p = err * parameters.p;
    item.i = limited(err * parameters.i + item.i, i.min, i.max);
    item.d = (err - last_err) * parameters.d;

    output = limited(item.p + item.i + item.d, out.min, out.max);

    last_err = err;

    return output;
}
