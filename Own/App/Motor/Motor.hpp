//
// Created by Administrator on 2025/3/4.
//

#pragma once
#include "Motor.tpp"
#include "M3508.hpp"
#include "M2006.hpp"
#include "lkMotor.hpp"
template<motor_control motor>
class Motor : public motor{
public:
  template<typename... Args>
  explicit Motor(Args&&... args) : motor(std::forward<Args>(args)...) {};

};




