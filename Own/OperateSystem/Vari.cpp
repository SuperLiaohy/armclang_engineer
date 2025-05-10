//
// Created by Administrator on 24-10-3.
//

#include "Buzzer/Buzzer.hpp"
#include "CAN/SuperCan.hpp"
#include "CDC/SuperCDC.hpp"
#include "Chassis/Chassis.hpp"
#include "GPIO/SuperGPIO.hpp"
#include "Heap/CustomHeap.hpp"
#include "ImageTrans/ImageTrans.hpp"
#include "Interact/Interact.hpp"
#include "Judge/referee_system.h"
#include "Judge/ui.hpp"
#include "Motor/Motor.hpp"
#include "OneStepGet/OneStepGet.hpp"
#include "Pump/Pump.hpp"
#include "RGBLED/RGBLED.hpp"
#include "RemoteControl/RemoteControl.hpp"
#include "RoboArm/RoboArm.hpp"
#include "W25Q64/W25Q64.hpp"
#include <GPIO/SuperGPIO.hpp>
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
template<> const FOC Motor<GM6020>::foc = {0x204, 0x1FF, 0x2FF};
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
Chassis chassis(&canPlus2, chassis_dep::move_default, chassis_dep::base_motor_default,
                chassis_dep::extend_motor_default);
#endif

// joint3的offset是不会变的，因为joint3是没有经过180°的，joint1也是一样
RoboArm roboArm(&canPlus1, 5, 65536, 10, 1, 65536, 6, 2, 65536, 6, 3, 65536, 6, 4, 65536, 10, 1, 1,
                Pid(600, 0.01, 100, 0, 8000, 0.0), Pid(2.5f, 0.00f, 0.3f, 4000.f, 10000.0f), 2,
                Pid(600, 0.01, 100, 0, 8000, 0.0), Pid(2.5f, 0.00f, 0.3f, 4000.f, 10000.0f), &hi2c1,
                {87.197998, -45.0833359 + 360 - 102.278336 + 5, -45.0833359 + 37.5383339 + 5, 135 + 27.9533329, 112.700996, 0,
                 0});

__attribute__((section(".RAM_D3"))) RGBLED Led(&hspi6);

Buzzer buzzer(&htim12, TIM_CHANNEL_2);

Imu imu(IMU_MEASURE::MEASURE_DISABLE);

Interact interact(0xFF, 0xFE, &huart5, &huart10, &huart3);

//Key keyList[KEY_NUM];
//KeyBoard key_board;

SuperGPIO power_5v(GPIOC, GPIO_PIN_15);
SuperGPIO power_24v_right(GPIOC, GPIO_PIN_14);
SuperGPIO power_24v_left(GPIOC, GPIO_PIN_13);

UI ui(102, 0x0166, &huart7);

// OneStepGet one_step_get_left(Pid(1.5, 0, 2.3, 4000, 10000, 1), Pid(50, 0.0001, 20, 500, 3000, 0.0), 4,
//     Pid(15, 0, 4, 8000, 16000, 1), Pid(15, 0, 0, 8000, 16000, 1), 2);
// OneStepGet one_step_get_right(Pid(1.5, 0, 2.3, 4000, 10000, 1),Pid(50, 0.0001, 20, 500, 3000, 0.0), 3,
//     Pid(15, 0, 4, 8000, 16000, 1), Pid(15, 0, 4, 8000, 16000, 1), 1);

OneStepGetControl OSG::mode = OneStepGetControl::AUTO;
OneStepGetAUTO OSG::auto_mode = OneStepGetAUTO::NONE;
OSG one_step_gets(
    Pid(100, 0.0000, 20, 500, 5000, 0.0), Pid(1.5, 0, 2.3, 4000, 10000, 1), 4, slope_cfg{.dead_zone = 0,.step = 0.6},
    Pid(15, 0, 4, 8000, 16000, 1.0), Pid(15, 0, 4, 8000, 16000, 1.0), 2, slope_cfg{.dead_zone = 0,.step = 0.6},
    Pid(100, 0.0000, 20, 500, 5000, 0.0), Pid(1.5, 0, 2.3, 4000, 10000, 1), 3, slope_cfg{.dead_zone = 0,.step = 0.6},
    Pid(15, 0, 4, 8000, 16000, 1.0), Pid(15, 0, 4, 8000, 16000, 1.0), 1, slope_cfg{.dead_zone = 0,.step = 0.6});

interact_dep::Actions anti_reset(1);
interact_dep::Actions get_right_y(1);

// OneStepGetControl one_step_get_control = OneStepGetControl::AUTO;
// OneStepGetAUTO one_step_get_auto = OneStepGetAUTO::NONE;
