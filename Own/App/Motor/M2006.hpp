/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-24 17:30:31
 * @FilePath: /DM/Own/App/Motor/M2006.hpp
 * @Description: DJI 2006电机 使用的C610的FOC
 */
#pragma once

#include "Control/PosPidControl.hpp"
#include "Param/M2006.hpp"
#define USING_M2006 1
#if USING_M2006 == 1

class M2006Pos : public PosPidControl<M2006> {
public:
    template<typename... Args>
    explicit M2006Pos(Args&&... args)
        : PosPidControl<M2006>(std::forward<Args>(args)...) {};

    void clear() {
        this->position.clear();
        this->speed.clear();
    }

    [[nodiscard]] float& total_position() { return this->feedback.total_position; }

    bool get_feedback(uint16_t id, uint8_t* data) {
        if (id == this->rx_id) {
            PosPidControl<M2006>::get_feedback(data);
            this->total_cnt();
            return true;
        }
        return false;
    };
};

#endif //USING_M2006
