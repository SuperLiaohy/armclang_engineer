//
// Created by Administrator on 25-1-6.
//

#include "roboarm_dep.hpp"
namespace roboarm_dep {
    void Differentiator::init() {
        left.clear();
        right.clear();
        left.total_position() = 0;
        right.total_position() = 0;
    }

    void Differentiator::update_relative_pos(float& pitch, float& roll) {
        pitch = (left.total_position() + right.total_position()) / 2;
        roll  = (left.total_position() - right.total_position()) / 2;
    }

    MDH MDH::inv() {
        MDH tmp;
        rota.transpose(tmp.rota);
        return tmp;
    }

} // namespace roboarm_dep