//
// Created by lhy on 2024/9/9.
//
#pragma once


#include "pid.hpp"


class DoublePid {
public:
    DoublePid(PID inter_pid, PID extern_pid) : output(0), inter_pid(inter_pid), extern_pid(extern_pid) {}

    virtual float update(float extern_input, float extern_target, float inter_input);

    float output;

    PID inter_pid;
    PID extern_pid;
protected:
};


