//
// Created by Administrator on 25-1-6.
//

#include "roboarm_dep.hpp"
namespace roboarm_dep {
    void Differentiator::init() {
        left.clear();
        right.clear();
//        read_fram();
        left.total_position()  = 0;
        right.total_position() = 0;
    }

    void Differentiator::read_fram() {
        fram.read(0x0010,&index,1,1);
        float target[2];
        switch (index) {
            case 1:
                fram.read(0x0030, reinterpret_cast<uint8_t*>((target)),8,1);
                break;
            case 2:
                fram.read(0x0020, reinterpret_cast<uint8_t*>((target)),8,1);
                break;
            default:
                break;
        }
        left.total_position() = target[0];
        right.total_position() = target[1];
    }

    void Differentiator::write_fram() {
        float target[2] = {left.total_position(), right.total_position()};
        switch (index) {
            case 1:
                fram.read(0x0020, reinterpret_cast<uint8_t*>((target)),8,1);
                index = 2;
                fram.write(0x0010, &index,1,1);
                break;
            default:
                fram.write(0x0030, reinterpret_cast<uint8_t*>((target)),8,1);
                index = 1;
                fram.write(0x0010, &index,1,1);
                break;
        }
    }

    void Differentiator::update_relative_pos(float& pitch, float& roll) {
        pitch = (left.total_position() + right.total_position()) / 2;
        roll  = (1/gain) * (left.total_position() - right.total_position()) / 2;
    }

    MDH MDH::inv() {
        MDH tmp;
        rota.transpose(tmp.rota);
        return tmp;
    }

} // namespace roboarm_dep