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

W25Q64 w25q64(&hospi2);

__weak SuperCan canPlus1(&hfdcan1, FDCAN_RX_FIFO0, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
__weak SuperCan canPlus2(&hfdcan2, FDCAN_RX_FIFO0, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
__weak SuperCan canPlus3(&hfdcan3, FDCAN_RX_FIFO0, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);

#if USING_LKMOTOR == 1
#endif

#if USING_DMMOTOR == 1
#endif

#if USING_GM6020 == 1
#endif

#if USING_M3508 == 1
#endif

#if USING_M2006 == 1
#endif

#if USING_CHASSIS == 1
#endif

Buzzer buzzer(&htim12, TIM_CHANNEL_2);

Chassis chassis(&canPlus2, {Slope(15, 1), Slope(15, 1), Slope(0.1, 0), Slope(15, 1)}, chassis_dep::base_motor_default,
                chassis_dep::extend_motor_default);

// joint3的offset是不会变的，因为joint3是没有经过180°的，joint1也是一样
RoboArm roboArm(&canPlus1, 5, 65536, 10, 1, 65536, 6, 2, 65536, 6, 3, 65536, 6, 4, 65536, 10, 
                Pid(400, 0.002, 0.8, 800, 1200, 0), Pid(0.45, 0.010, 0.00, 550, 550, 1.0),
                7, 65536, 10,
                6, 65536, 10,
                {87.197998, -45.0833359 + 360 - 102.278336 + 5, -45.0833359 + 37.5383339 + 5, 135 + 27.9533329,
                 360-13.6999512, 310.715 - 360, 0});

__attribute__((section(".RAM_D3"))) RGBLED Led(&hspi6);

Imu imu(IMU_MEASURE::MEASURE_DISABLE);

Interact interact(0xFF, 0xFE, &huart5, &huart10, &huart3);

SuperGPIO power_5v(GPIOC, GPIO_PIN_15);
SuperGPIO power_24v_right(GPIOC, GPIO_PIN_14);
SuperGPIO power_24v_left(GPIOC, GPIO_PIN_13);

UI ui(102, 0x0166, &huart7);

OneStepGetControl OSG::mode   = OneStepGetControl::AUTO;
OSG one_step_gets(Pid(100, 0.0000, 20, 500, 9000, 0.0), Pid(1.5, 0, 2.3, 4000, 7000, 1),
                  4, Slope(1.3, 0), false,4000,-4000,
                  Pid(20, 0, 4, 8000, 16000, 1.0), Pid(20, 0, 4, 8000, 16000, 1.0),
                  2, Slope {0.6, 0}, true,4000,-4000,
                  Pid(100, 0.0000, 20, 500, 9000, 0.0), Pid(1.5, 0.00, 2.3, 4000, 7000, 1),
                  3, Slope(1.3, 0), true,4000,-4000,
                  Pid(20, 0, 4, 8000, 16000, 1.0), Pid(20, 0, 4, 8000, 16000, 1.0),
                  1, Slope(0.6, 0), false,4000,-4000);

interact_dep::Actions get_silver_mine({0,37.604,115.54184,0,27.570,0}, {480, 720, 360, 360});
interact_dep::Actions get_silver_mine_z(Slope(0.4, 0.15, 310), interact_dep::action_status::CartesianZ_z);

interact_dep::Actions exchange_left({-17.9960938, 36.7366142, 35.3361511, -89.4694138, -89.9465207, 52.6248474});
interact_dep::Actions exchange_right({17.9960938,36.7366142,35.3361511,89.4694138,-89.9465207,-52.6248474});

interact_dep::Actions reset1({0, -55, 145, 0, 0, 0}, {720,720,360,720});
interact_dep::Actions reset2({0, -16.3, 144.8, 0, 0, 0}, {720, 720, 360, 720});

interact_dep::Actions arm_get_gold({0, 48.4501343, 70.3479385, 0.00540079346, -28.7980728, 0}, {480, 720, 900, 720});
interact_dep::Actions arm_get_gold_z(Slope(0.1, 0.15, 120), interact_dep::action_status::CartesianX_z);

// **************************************************************************************************** //
// OK

std::array<interact_dep::Actions, 2> get_second_silver_action        = {
     get_silver_mine,
     get_silver_mine_z};
// std::array<interact_dep::Actions, 2> get_second_silver_action        = {
//     interact_dep::Actions({0, 37.604, 115.54184, 0, 27.570, 0},{480,720,360,360}),
//     interact_dep::Actions(Slope(0.4, 0.15, 310), interact_dep::action_status::CartesianZ_z)
// };
std::array<uint32_t, 2> get_second_silver_time                       = {3000, 2000};
std::array<interact_dep::ActionsGroup::exe, 3> get_second_silver_exe = {
    []() {
        interact.sub_board.set_pump(1);
        interact.sub_board.set_main_valve(1);
    },
    nullptr,
    []() { interact.robo_arm.mode = interact_dep::robo_mode::NONE; }
};
std::array<interact_dep::ActionsGroup::event, 2> get_second_silver_event = {
    []()->bool {
        return interact.sub_board.custom_frame_rx.s.valve3 < 250;
    },
    nullptr
};
interact_dep::ActionsGroup get_second_silver_group = {.actions_list = get_second_silver_action.data(),
                                                      .time_list    = get_second_silver_time.data(),
//                                                      .event_list   = get_second_silver_event.data(),
	                                                    .event_list   = nullptr,
                                                      .exe_list     = get_second_silver_exe.data(),
                                                      .len          = 2,
                                                      .index        = 0,
                                                      .time_cnt     = 0};

// **************************************************************************************************** //
// !!!

std::array<uint32_t, 1> put_down_silver_time = {100};
std::array<interact_dep::Actions, 1> put_down_silver_action        = {reset2};
std::array<interact_dep::ActionsGroup::exe, 2> put_down_silver_exe = {
    []() {
        interact.sub_board.set_pump(0);
        interact.sub_board.set_lf_valve(0);
        interact.sub_board.set_rf_valve(0);
//        one_step_gets.Xleft.set_state(translation::state::MOVE,1000);
//        one_step_gets.Xright.set_state(translation::state::MOVE,-1000);
    },
    []() { interact.robo_arm.mode = interact_dep::robo_mode::NONE; }};

interact_dep::ActionsGroup put_down_silver_group = {.actions_list = put_down_silver_action.data(),
                                                      .time_list    = put_down_silver_time.data(),
                                                      .event_list   = nullptr,
                                                      .exe_list     = put_down_silver_exe.data(),
                                                      .len          = 1,
                                                      .index        = 0,
                                                      .time_cnt     = 0};

// **************************************************************************************************** //
// OK

std::array<uint32_t, 1> put_down_gold_time = {100};
std::array<interact_dep::Actions, 1> put_down_gold_action        = {reset2};
std::array<interact_dep::ActionsGroup::exe, 2> put_down_gold_exe = {
    []() {
        interact.sub_board.set_pump(0);
        interact.sub_board.set_lf_valve(0);
        interact.sub_board.set_rf_valve(0);
//        one_step_gets.Xleft.set_state(translation::state::MOVE, 1000);
//        one_step_gets.Xright.set_state(translation::state::MOVE, -1000);
//        one_step_gets.Yleft.set_state(translation::state::MOVE, 1450);
//        one_step_gets.Yright.set_state(translation::state::MOVE, -1450);
    },
    []() { interact.robo_arm.mode = interact_dep::robo_mode::NONE; }};

interact_dep::ActionsGroup put_down_gold_group = {.actions_list = put_down_gold_action.data(),
                                                      .time_list    = put_down_gold_time.data(),
                                                      .event_list   = nullptr,
                                                      .exe_list     = put_down_gold_exe.data(),
                                                      .len          = 1,
                                                      .index        = 0,
                                                      .time_cnt     = 0};

// **************************************************************************************************** //


std::array<uint32_t, 4> get_gold_time = {10, 4000, 2000, 4000};
std::array<interact_dep::Actions, 4> get_gold_action        = {reset1, reset1, reset1, reset1};
std::array<interact_dep::ActionsGroup::exe, 5> get_gold_exe = {
    []() {
        interact.sub_board.set_pump(1);
        interact.sub_board.set_rf_valve(1);
        interact.sub_board.set_lf_valve(1);
    },
    []() {
        one_step_gets.Xleft.set_state(translation::state::MOVE, osg::xl_max);
//        one_step_gets.Yleft.set_state(translation::state::MOVE, 200);
        one_step_gets.Xright.set_state(translation::state::MOVE, osg::xr_max);
//        one_step_gets.Yright.set_state(translation::state::MOVE, -200);
    },
    []() {
        one_step_gets.Yleft.set_state(translation::state::MOVE, osg::yl_max); //255
        one_step_gets.Yright.set_state(translation::state::MOVE, osg::yr_max); //-345
    },
    []() {
        one_step_gets.Xleft.set_state(translation::state::MOVE, 0);
        one_step_gets.Xright.set_state(translation::state::MOVE, 0);
    },
    []() { interact.robo_arm.mode = interact_dep::robo_mode::NONE; }};

std::array<interact_dep::ActionsGroup::event, 4> get_gold_event = {
    []()-> bool {
        return interact.sub_board.custom_frame_rx.s.valve5 < 250  && interact.sub_board.custom_frame_rx.s.valve1 < 250;
    },
    nullptr,
    nullptr,
    nullptr};

interact_dep::ActionsGroup get_gold_group = {.actions_list = get_gold_action.data(),
                                                      .time_list    = get_gold_time.data(),
                                                      .event_list   = get_gold_event.data(),
                                                      .exe_list     = get_gold_exe.data(),
                                                      .len          = 4,
                                                      .index        = 0,
                                                      .time_cnt     = 0};

// **************************************************************************************************** //


std::array<uint32_t, 4> get_right_gold_time = {10, 4000, 2000, 4000};
std::array<interact_dep::Actions, 4> get_right_gold_action        = {
    reset1,
    reset1,
    reset1,
    reset1};
std::array<interact_dep::ActionsGroup::exe, 5> get_right_gold_exe = {
    []() {
        interact.sub_board.set_pump(1);
        interact.sub_board.set_rf_valve(1);
    },
    []() {
        one_step_gets.Xright.set_state(translation::state::MOVE, osg::xr_max);
//        one_step_gets.Yright.set_state(translation::state::MOVE, -200);
    },
    []() {
        one_step_gets.Yright.set_state(translation::state::MOVE, osg::yr_max); //-345
    },
    []() {
        one_step_gets.Xright.set_state(translation::state::MOVE, 0);
    },
    []() { interact.robo_arm.mode = interact_dep::robo_mode::NONE; }};

interact_dep::ActionsGroup get_right_gold_group = {.actions_list = get_right_gold_action.data(),
                                                      .time_list    = get_right_gold_time.data(),
                                                      .event_list   = nullptr,
                                                      .exe_list     = get_right_gold_exe.data(),
                                                      .len          = 4,
                                                      .index        = 0,
                                                      .time_cnt     = 0};

// **************************************************************************************************** //


std::array<uint32_t, 4> get_left_gold_time = {10, 4000, 2000, 4000};
std::array<interact_dep::Actions, 4> get_left_gold_action        = {reset1, reset1, reset1, reset1};
std::array<interact_dep::ActionsGroup::exe, 5> get_left_gold_exe = {
    []() {
        interact.sub_board.set_pump(1);
        interact.sub_board.set_lf_valve(1);
    },
    []() {
        one_step_gets.Xleft.set_state(translation::state::MOVE, osg::xl_max);
//        one_step_gets.Yleft.set_state(translation::state::MOVE, 200);
    },
    []() {
        one_step_gets.Yleft.set_state(translation::state::MOVE, osg::yl_max); //255
    },
    []() {
        one_step_gets.Xleft.set_state(translation::state::MOVE, 0);
    },
    []() { interact.robo_arm.mode = interact_dep::robo_mode::NONE; }};

interact_dep::ActionsGroup get_left_gold_group = {.actions_list = get_left_gold_action.data(),
                                                      .time_list    = get_left_gold_time.data(),
                                                      .event_list   = nullptr,
                                                      .exe_list     = get_left_gold_exe.data(),
                                                      .len          = 4,
                                                      .index        = 0,
                                                      .time_cnt     = 0};


// **************************************************************************************************** //


