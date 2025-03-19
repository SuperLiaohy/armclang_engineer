//
// Created by Administrator on 24-12-6.
//

#include "CDC/SuperCDC.hpp"

// #include <Interact/Interact.hpp>
#include "Interact/Interact.hpp"

// extern Interact interact;

extern "C" void CDC_ReceiveCallBack(uint8_t *buf, uint32_t *len) {
//    CDC_Transmit_HS(buf, *len);
     interact.receive_cdc(buf);
}