//
// Created by Administrator on 25-1-6.
//

#include "RoboArmDep.h"
void Differentiator::init() {
    float left_dPos  = 0;
    float right_dPos = 0;
    uint32_t cnt       = 0;
    while (1) {

        canPlus1.write(-1500, 1500, 0, 0);
        canPlus1.send(Motor<M2006>::foc.TX_LOW_ID);

        //            int32_t left_dPos = 0;
        HAL_Delay(1);
        left_dPos = left.feed_back.Data.position - left.feed_back.Data.lastPosition;
        if (left_dPos > 180) {
            left_dPos -= 360;
        } else if (left_dPos < -180) {
            left_dPos += 360;
        }
        //            int32_t right_dPos = 0;
        right_dPos = right.feed_back.Data.position - right.feed_back.Data.lastPosition;
        if (right_dPos > 180) {
            right_dPos -= 360;
        } else if (right_dPos < -180) {
            right_dPos += 360;
        }

        if (isInRange<float>(left_dPos, -0.01, 0.01) && isInRange<float>(right_dPos, -0.01, 0.01)) {
            ++cnt;
            if (cnt > 50) {
                HAL_Delay(10);
                canPlus1.write(0, 0, 0, 0);
                canPlus1.send(Motor<M2006>::foc.TX_LOW_ID);
                HAL_Delay(100);
//                offset.l = left.feed_back.totalPosition - left.motor.firstPosition;
//                offset.r = right.feed_back.totalPosition - right.motor.firstPosition;

                left.feed_back.totalPosition  = -90;
                right.feed_back.totalPosition = 90;
                break;
            }
        } else {
            cnt = 0;
        }
    }
}
void Differentiator::update_relative_pos(float& pitch, float& roll) {
    pitch = (left.feed_back.totalPosition - right.feed_back.totalPosition) / 2;
    roll  = (left.feed_back.totalPosition + right.feed_back.totalPosition) / 2;
}

MDH MDH::inv() {
    MDH tmp;
    rota.transpose(tmp.rota);
//    tmp.trans =
    return tmp;
}