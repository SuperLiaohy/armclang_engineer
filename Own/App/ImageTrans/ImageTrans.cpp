//
// Created by Administrator on 25-1-20.
//

#include "ImageTrans.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "stdlib.h"
#include "cmsis_os.h"
extern osThreadId IMAGEATRANS_TASHandle;
#ifdef __cplusplus
};
#endif


using namespace crc;

void ImageTrans::update() {
//    key_board.mouse = 0;
    auto buff = uartPlus.rx_buffer;

    if (buff[0] == image_trans_dep::SOF && verify_crc8_check_sum(buff, 5)) {
        xTaskResumeFromISR(IMAGEATRANS_TASHandle);
    }

}
