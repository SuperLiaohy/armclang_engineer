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
#include "Judge/referee_system.h"
#include "Judge/ui.hpp"
#include "OneStepGet/OneStepGet.hpp"
#include "RGBLED/RGBLED.hpp"
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
#endif

Buzzer buzzer(&htim12, TIM_CHANNEL_2);

Chassis chassis(&canPlus2, {Slope(15, 1), Slope(15, 1), Slope(0.1, 0), Slope(15, 1)}, chassis_dep::base_motor_default,
                chassis_dep::extend_motor_default);

// joint3的offset是不会变的，因为joint3是没有经过180°的，joint1也是一样
RoboArm roboArm(&canPlus1, 5, 65536, 10, 1, 65536, 6, 2, 65536, 6, 3, 65536, 6, 4, 65536, 10, 1, 1,
                Pid(1200, 0.01, 100, 4000, 8000, 0.0), Pid(2.5f, 0.00f, 0.3f, 4000.f, 10000.0f), 2,
                Pid(1200, 0.01, 100, 4000, 8000, 0.0), Pid(2.5f, 0.00f, 0.3f, 4000.f, 10000.0f), &hi2c1,
                {87.197998, -45.0833359 + 360 - 102.278336 + 5, -45.0833359 + 37.5383339 + 5, 135 + 27.9533329,
                 112.700996, 0, 0});

__attribute__((section(".RAM_D3"))) RGBLED Led(&hspi6);

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

OneStepGetControl OSG::mode   = OneStepGetControl::AUTO;
OneStepGetAUTO OSG::auto_mode = OneStepGetAUTO::NONE;
OSG one_step_gets(Pid(100, 0.0000, 20, 500, 5000, 0.0), Pid(1.5, 0, 2.3, 4000, 10000, 1), 4, Slope(0.6, 0),
                  Pid(15, 0, 4, 8000, 16000, 1.0), Pid(15, 0, 4, 8000, 16000, 1.0), 2, Slope {0.6, 0},
                  Pid(100, 0.0000, 20, 500, 5000, 0.0), Pid(1.5, 0, 2.3, 4000, 10000, 1), 3, Slope(0.6, 0),
                  Pid(15, 0, 4, 8000, 16000, 1.0), Pid(15, 0, 4, 8000, 16000, 1.0), 1, Slope(0.6, 0));

interact_dep::Actions anti_reset(interact_dep::action_status::Joints);
interact_dep::Actions get_right_y(interact_dep::action_status::Joints);

interact_dep::Actions get_silver_mine(interact_dep::action_status::Joints);
interact_dep::Actions get_silver_mine_z(Slope(0.5, 0.2, 220), interact_dep::action_status::CartesianZ, {400, 0, -88});
interact_dep::Actions put_silver_mine_right(interact_dep::action_status::Joints);
interact_dep::Actions put_silver_mine_left(interact_dep::action_status::Joints);

interact_dep::Actions exchange_left(interact_dep::action_status::Joints);
interact_dep::Actions exchange_right(interact_dep::action_status::Joints);

interact_dep::Actions reset1(interact_dep::action_status::Joints);
interact_dep::Actions reset2(interact_dep::action_status::Joints);
interact_dep::Actions silver_reset(interact_dep::action_status::Joints);
// interact_dep::Actions reset2(interact_dep::action_status::Joints);

// std::array<interact_dep::Actions, 2>reset = {get_silver_mine, get_silver_mine_z} ;
// std::array<uint32_t, 2>reset_time = {500, 2000};
// interact_dep::ActionsGroup reset_group={.actions_list = reset.data(), .time_list = reset_time.data(), .len = 2, .index = 0, .time_cnt = 0};

std::array<interact_dep::Actions, 5> get_silver_action = {get_silver_mine, get_silver_mine_z, put_silver_mine_left,
                                                          exchange_left, silver_reset};
std::array<uint32_t, 5> get_silver_time                = {2000, 2000, 2000, 200, 200};
std::array<interact_dep::ActionsGroup::exe, 6> get_silver_exe = {[]() {
                                                                     interact.sub_board.set_pump(1);
                                                                     interact.sub_board.set_valve3(1);
                                                                     interact.sub_board.set_valve5(0);
                                                                 },
                                                                 nullptr,
                                                                 []() {
                                                                     interact.sub_board.set_valve5(1);
                                                                     one_step_gets.left.X.status =
                                                                         OneStepGetXStatus::FRONT;

                                                                     one_step_gets.left.X.pos.step_set(0.35);
                                                                     one_step_gets.left.X.pos.target_set(350);
                                                                 },
                                                                 []() {
                                                                     interact.sub_board.set_valve3(0);
                                                                     one_step_gets.left.X.pos.target_set(0);
                                                                     one_step_gets.left.X.pos.step_set(0.6);
},
                                                                 []() {},
                                                                 []() {
                                                                     interact.robo_arm.mode = interact_dep::robo_mode::NONE;
                                                                 }

};

interact_dep::ActionsGroup get_silver_group = {.actions_list = get_silver_action.data(),
                                               .time_list    = get_silver_time.data(),
                                               .event_list   = nullptr,
                                               .exe_list     = get_silver_exe.data(),
                                               .len          = 5,
                                               .index        = 0,
                                               .time_cnt     = 0};

std::array<interact_dep::ActionsGroup::exe, 4> get_gold_exe = {
    []() {
        interact.sub_board.set_pump(1);
        interact.sub_board.set_valve4(1);
        interact.sub_board.set_valve5(1);
    },
    []() {
        one_step_gets.left.X.status = OneStepGetXStatus::FRONT;
        one_step_gets.left.X.pos.target_set(2000);
        one_step_gets.right.X.status = OneStepGetXStatus::FRONT;
        one_step_gets.right.X.pos.target_set(-2300);
    },
    []() {
        one_step_gets.left.Y.status = OneStepGetYStatus::UP;
        one_step_gets.left.Y.pos.target_set(300);
        one_step_gets.right.Y.status = OneStepGetYStatus::UP;
        one_step_gets.right.Y.pos.target_set(-300);
    },
    []() {
        one_step_gets.left.X.status = OneStepGetXStatus::BACK;
        one_step_gets.left.X.pos.target_set(0);
        one_step_gets.right.X.status = OneStepGetXStatus::BACK;
        one_step_gets.right.X.pos.target_set(0);
    }};

// OneStepGetControl one_step_get_control = OneStepGetControl::AUTO;
// OneStepGetAUTO one_step_get_auto = OneStepGetAUTO::NONE;
