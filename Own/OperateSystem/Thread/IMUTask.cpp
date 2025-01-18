//
// Created by liaohy on 24-11-16.
//
//#include <sys/stat.h>

#include "CppTask.h"

#include "Imu/Imu.h"
#include "W25Q64/W25Q64.h"
#include "ThreadConfig.h"
#define TEMP_CTRL_ENABLE 0
static uint8_t imu_flag = 0;
static uint32_t count = 0;
static uint32_t correct_times = 0;
static uint32_t temp_ticks = 0;

void IMUTask() {
    imu.init();
    imu.update_raw_data();
    imu.mahony_start();
    imu.update_offset(++correct_times, imu_flag);
    while (1) {
        if (count % 10 == 0) {
#if TEMP_CTRL_ENABLE
            imu.update_temp_ctrl();
#elif TEMP_CTRL_ENABLE == 0
            if (imu_flag == 0)
                imu_flag = 1;
#endif
            if (imu_flag == 0 && imu.is_temp_dead_condition(0.5)) {
                imu_flag = imu.is_temp_const(++temp_ticks);
            } else if (!imu.is_temp_dead_condition(0.5)) {
                temp_ticks = 0;
            }
        }
        ++count;
        IMUHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelay(1);
        if (count % 1 == 0) {
            imu.update_raw_data();
            switch (imu_flag) {
                case 1:
                    switch (imu.offset.restart_measure) {
                        case MEASURE_DISABLE:
                            w25q64.raed_buffer(IMU_OFFSET_ADDRESS, (uint8_t *) &imu.offset.correct, 12);
                            imu_flag = 2;
                            break;
                        case MEASURE_ENABLE:
                            imu.update_offset(++correct_times, imu_flag);
                            if (imu_flag == 2) {
                                w25q64.erase_sector_4(IMU_OFFSET_ADDRESS);
                                w25q64.write_buffer(IMU_OFFSET_ADDRESS, (uint8_t *) &imu.offset.correct, 12);
                            }
                            break;
                    }
                    break;
                case 2:
                    imu.update();
                    xEventGroupSetBits(osEventGroup, IMU_EVENT);
                    break;
                default:
                    break;
            }
        }
    }
}
