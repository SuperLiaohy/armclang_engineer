//
// Created by Administrator on 24-12-6.
//

#ifndef DM_42_SUPERCDC_H
#define DM_42_SUPERCDC_H

#ifdef __cplusplus
extern "C" {
#endif
#include "string.h"
#include "stdarg.h"
#include "usbd_cdc_if.h"
extern USBD_HandleTypeDef hUsbDeviceHS;
/** Received data over USB are stored in this buffer      */
extern uint8_t UserRxBufferHS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
extern uint8_t UserTxBufferHS[APP_TX_DATA_SIZE];
#ifdef __cplusplus
}
#endif

void cdc_printf(const char *fmt, ...);

#endif //DM_42_SUPERCDC_H
