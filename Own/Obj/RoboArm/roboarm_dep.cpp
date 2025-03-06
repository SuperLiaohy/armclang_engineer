//
// Created by Administrator on 25-1-6.
//

#include "roboarm_dep.hpp"
namespace roboarm_dep {
    void Differentiator::init() {
        float left_dPos    = 0;
        float right_dPos   = 0;
        uint32_t cnt       = 0;
        float limit_min    = 0.1;
        float limit_max    = 4000;
        uint32_t time      = 0;
        float limit_time   = 150;
        float left_target  = -0;
        float right_target = 0;
        // left.feed_back.totalPosition = 0;
        // right.feed_back.totalPosition = 0;
        while (1) {
            ++time;
            left_target  -= 1.2;
            right_target += 1.2;

            left.motor.set_position(left_target);
            right.motor.set_position(right_target);

            // left.motor.doublePid.update(left.feed_back.totalPosition, left_target, left.feed_back.data.speed);
            // right.motor.doublePid.update(right.feed_back.totalPosition, right_target, right.feed_back.data.speed);

            if (time > limit_time) {
                limited(left.motor.output(), -limit_max + 2000.f * (time - limit_time) / limit_time, limit_max - 2000.f * (time - limit_time) / limit_time);
                limited(right.motor.output(), -limit_max + 2000.f * (time - limit_time) / limit_time, limit_max - 2000.f * (time - limit_time) / limit_time);
            } else {
                limited(left.motor.output(), -limit_max, limit_max);
                limited(right.motor.output(), -limit_max, limit_max);
            }

            canPlus1.write(static_cast<int16_t>(left.motor.output()), static_cast<int16_t>(right.motor.output()), 0, 0);
            canPlus1.send(M2006::foc.TX_LOW_ID);
            left_dPos  = left.motor.dpos();
            right_dPos = right.motor.dpos();
            if (time > limit_time * 2) {
                canPlus1.write(0, 0, 0, 0);
                canPlus1.send(M2006::foc.TX_LOW_ID);
                HAL_Delay(50);
                left.motor.clear();
                right.motor.clear();
                left.motor.total_position() = -90;
                right.motor.total_position() = 90;
                break;
            }
            if ((isInRange<float>(left_dPos, -limit_min, limit_min) && isInRange<float>(right_dPos, limit_min, limit_min))) {
                ++cnt;
                if (cnt > 10) {

                    canPlus1.write(0, 0, 0, 0);
                    canPlus1.send(M2006::foc.TX_LOW_ID);
                    HAL_Delay(50);
                    left.motor.clear();
                    right.motor.clear();
                    left.motor.total_position() = -90;
                    right.motor.total_position() = 90;
                    break;
                }
            } else {
                cnt = 0;
            }
            osDelay(1);
        }
    }

    void Differentiator::update_relative_pos(float& pitch, float& roll) {
        pitch = (left.motor.total_position() - right.motor.total_position()) / 2;
        roll  = (left.motor.total_position() + right.motor.total_position()) / 2;
    }

    MDH MDH::inv() {
        MDH tmp;
        rota.transpose(tmp.rota);
        //    tmp.trans =
        return tmp;
    }

} // namespace roboarm_dep