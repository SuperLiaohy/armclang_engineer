//
// Created by Administrator on 24-12-6.
//

#include "CDC/SuperCDC.h"
#include "Uart/SuperUart.h"
#include "Interact/Interact.h"
extern "C" void CDC_ReceiveCallBack(uint8_t *buf, uint32_t *len) {
//    CDC_Transmit_HS(buf, *len);
    interact.receive_cdc(buf);
}