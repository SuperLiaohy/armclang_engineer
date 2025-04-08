//
// Created by lhy on 2024/9/9.
//
#pragma once

#include "Control/SpeedPidControl.hpp"
#include "Param/M3508.hpp"

#define USING_M3508 1
#if USING_M3508 == 1

class M3508Speed : public SpeedPidControl<M3508> {
public:
    template<typename... Args>
    explicit M3508Speed(Args&&... args)
        : SpeedPidControl<M3508>(std::forward<Args>(args)...) {};

    bool get_feedback(uint16_t id, uint8_t* data) {
        if (id == this->rx_id) {
            SpeedPidControl<M3508>::get_feedback(data);
            return true;
        }
        return false;
    };
};

#endif //USING_M3508
