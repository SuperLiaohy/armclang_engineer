//
// Created by Administrator on 24-10-3.
//

#include "Buzzer/Buzzer.hpp"
#include "CAN/SuperCan.hpp"
#include "CDC/SuperCDC.hpp"
#include "Chassis/Chassis.hpp"
#include "GPIO/SuperGPIO.hpp"
#include "Heap/CustomHeap.hpp"
#include "Interact/Interact.hpp"
// #include "Motor/GM6020.hpp"
// #include "Motor/M2006.hpp"
// #include "Motor/M3508.hpp"
// #include "Motor/Motor.hpp"
// #include "Motor/dmMotor.hpp"
// #include "Motor/lkMotor.hpp"
#include "ImageTrans/ImageTrans.hpp"
#include "Judge/referee_system.h"
#include "Judge/ui.hpp"
#include "Motor/Motor.hpp"
#include "Pump/Pump.hpp"
#include "RGBLED/RGBLED.hpp"
#include "RemoteControl/RemoteControl.hpp"
#include "RoboArm/RoboArm.hpp"
#include "W25Q64/W25Q64.hpp"
#include <Imu/Imu.hpp>

__attribute__((section(".DTCMRAM"))) uint64_t DTCMUsed[8 * 1024 / 8];
__attribute__((section(".RAM_D1"))) uint64_t D1Used[8 * 1024 / 8];
__attribute__((section(".RAM_D2"))) uint64_t D2Used[8 * 1024 / 8];
__attribute__((section(".RAM_D3"))) uint64_t D3Used[8 * 1024 / 8];

__attribute__((section(".DTCMRAM"))) CustomHeap DTCMHeap(DTCMUsed, sizeof(DTCMUsed));
__attribute__((section(".RAM_D1"))) CustomHeap D1Heap(D1Used, sizeof(D1Used));
__attribute__((section(".RAM_D2"))) CustomHeap D2Heap(D2Used, sizeof(D2Used));
__attribute__((section(".RAM_D3"))) CustomHeap D3Heap(D3Used, sizeof(D3Used));

// DetectManager Detect::detectManager;

W25Q64 w25q64(&hospi2);

__weak SuperCan canPlus1(&hfdcan1, FDCAN_RX_FIFO0, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
__weak SuperCan canPlus2(&hfdcan2, FDCAN_RX_FIFO0, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
__weak SuperCan canPlus3(&hfdcan3, FDCAN_RX_FIFO0, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);

#if USING_LKMOTOR == 1
// template<>
// const FOC Motor<lkMotor>::foc = {0x140, 0x140, 0x140};
// template<>
// const FOC Motor<lkMotorBoard>::foc = {0x140, 0x280, 0x280};
// Motor<lkMotorBoard> test_motor(1, 65536);
#endif

#if USING_DMMOTOR == 1
// template<>
// const FOC Motor<dmMotor>::foc = {0, 0x140, 0x140};
// Motor<dmMotor> link1(1, 8192, 1000);
#endif

#if USING_GM6020 == 1
template<>
const FOC Motor<GM6020>::foc = {0x204, 0x1FF, 0x2FF};
Motor<GM6020> gm6020(1, 8192, 1000);
#endif

#if USING_M3508 == 1
// template<>
// const FOC Motor<M3508>::foc = {0x200, 0x200, 0x1FF};

#endif

#if USING_M2006 == 1
// template<>

#endif
#if USING_CHASSIS == 1
Chassis chassis(
    &canPlus2,
    chassis_dep::move_default,
    chassis_dep::base_motor_default,
    chassis_dep::extend_motor_default);
#endif

RoboArm roboArm(&canPlus1,
                5, 65536, 10,
                1, 65536, 6,
                2, 65536, 6,
                3, 65536, 6,
                4, 65536, 10,
                0.7,
                1, Pid(40 * 1.1, 0.05 , 10, 1600 / 20, 4000), Pid(2.f, 0.01f, 0.2f, 2000.f, 10000.0f),
                2, Pid(40 * 1.1, 0.05 , 10 , 1600 / 20, 4000), Pid(2.f, 0.01f, 0.2f, 2000.f, 10000.0f),
                {360 - 66.7289963, -45.0833359 + 360 - 102.278336, -45.0833359 + 37.5383339, 135 + 27.9533329, 125.261002, -90, 90});

__attribute__((section(".RAM_D3"))) RGBLED Led(&hspi6);

Buzzer buzzer(&htim12, TIM_CHANNEL_2);

Imu imu(MEASURE_DISABLE);

//ImageTrans image_trans(&huart10);

Interact interact(0xFF, 0xFE, &huart5, &huart10, &huart3);

//Key keyList[KEY_NUM];
//KeyBoard key_board;

Pump<SuperGPIO<GPIOA_BASE, OUTPUT, GPIO_PIN_0>> pump;
SuperGPIO<GPIOC_BASE, OUTPUT, GPIO_PIN_15> power_5v;
SuperGPIO<GPIOC_BASE, OUTPUT, GPIO_PIN_14> power_24v_right;
SuperGPIO<GPIOC_BASE, OUTPUT, GPIO_PIN_13> power_24v_left;

interact_dep::Actions test_actions(4);

interact_dep::Actions stretch(2);
interact_dep::Actions turn_up(1);

UI ui(102, 0x0166);
