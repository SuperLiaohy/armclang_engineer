//
// Created by Administrator on 24-12-6.
//
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "usbd_cdc_if.h"
extern USBD_HandleTypeDef hUsbDeviceHS;
/** Received data over USB are stored in this buffer      */
extern uint8_t UserRxBufferHS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
extern uint8_t UserTxBufferHS[APP_TX_DATA_SIZE];
#ifdef __cplusplus
}
#endif

class SuperCDC {
public:
    SuperCDC() = default;
    [[gnu::always_inline]]inline void transmit(uint8_t *data, uint16_t len);
    void printf(const char *fmt, ...);
};

[[gnu::always_inline]]inline void SuperCDC::transmit(uint8_t *data, uint16_t len) {
    CDC_Transmit_HS(data, len);
}
