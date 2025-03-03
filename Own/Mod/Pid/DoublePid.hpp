//
// Created by lhy on 2024/9/9.
//
#pragma once


#include "Pid.hpp"


class DoublePid {
public:
    DoublePid(Pid inter_pid, Pid extern_pid) : output(0), inter_pid(inter_pid), extern_pid(extern_pid) {}

    virtual float update(float extern_input, float extern_target, float inter_input);

    float output;

    Pid inter_pid;
    Pid extern_pid;
protected:
};


