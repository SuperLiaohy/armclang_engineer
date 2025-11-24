//
// Created by Administrator on 24-10-3.
//

#include "Buzzer/Buzzer.hpp"
#include "CAN/SuperCan.hpp"
#include "CDC/SuperCDC.hpp"
#include "GPIO/SuperGPIO.hpp"
#include "Heap/CustomHeap.hpp"
#include "Interact/Interact.hpp"
#include "RGBLED/RGBLED.hpp"
// #include "W25Q64/W25Q64.hpp"
#include <Imu/Imu.hpp>

__attribute__((section(".DTCMRAM"))) uint64_t DTCMUsed[8 * 1024 / 8];
__attribute__((section(".RAM_D1"))) uint64_t D1Used[8 * 1024 / 8];
__attribute__((section(".RAM_D2"))) uint64_t D2Used[8 * 1024 / 8];
__attribute__((section(".RAM_D3"))) uint64_t D3Used[8 * 1024 / 8];

__attribute__((section(".DTCMRAM"))) CustomHeap DTCMHeap(DTCMUsed, sizeof(DTCMUsed));
__attribute__((section(".RAM_D1"))) CustomHeap D1Heap(D1Used, sizeof(D1Used));
__attribute__((section(".RAM_D2"))) CustomHeap D2Heap(D2Used, sizeof(D2Used));
__attribute__((section(".RAM_D3"))) CustomHeap D3Heap(D3Used, sizeof(D3Used));

SuperCan canPlus1(&hfdcan1, FDCAN_RX_FIFO0, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
SuperCan canPlus2(&hfdcan2, FDCAN_RX_FIFO0, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
SuperCan canPlus3(&hfdcan3, FDCAN_RX_FIFO0, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);

Buzzer buzzer(&htim12, TIM_CHANNEL_2);

__attribute__((section(".RAM_D3")))
RGBLED Led(&hspi6);

Imu imu(IMU_MEASURE::MEASURE_ENABLE);

Interact interact{};

SuperGPIO power_5v(GPIOC, GPIO_PIN_15);
SuperGPIO power_24v_right(GPIOC, GPIO_PIN_14);
SuperGPIO power_24v_left(GPIOC, GPIO_PIN_13);
