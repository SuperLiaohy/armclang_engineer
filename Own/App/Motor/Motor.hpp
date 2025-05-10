//
// Created by Administrator on 2025/3/4.
//

#pragma once
#include "LKSingleMotor.hpp"
#include "M2006Diff.hpp"
#include "M3508.hpp"
#include "Motor.tpp"
template<motor_control motor>
class Motor : public motor{
public:
  template<typename... Args>
  explicit Motor(Args&&... args) : motor(std::forward<Args>(args)...) {};

};




