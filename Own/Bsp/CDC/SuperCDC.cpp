//
// Created by Administrator on 24-12-6.
//

#include "SuperCDC.h"

void cdc_printf(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    uint8_t buffer[256];
    vsnprintf((char *)buffer, sizeof(buffer), fmt, args);
    va_end(args);
    CDC_Transmit_HS(buffer, strlen((char *)buffer));
}
