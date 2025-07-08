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
                // Pid(),Pid()
                7, 65536, 10,
                6, 65536, 10,
                {87.197998, -45.0833359 + 360 - 102.278336 + 5, -45.0833359 + 37.5383339 + 5, 135 + 27.9533329,
                 360-5.45471478, 310.715 - 360, 0});

__attribute__((section(".RAM_D3"))) RGBLED Led(&hspi6);

Imu imu(IMU_MEASURE::MEASURE_DISABLE);

Interact interact(0xFF, 0xFE, &huart5, &huart10, &huart3);

SuperGPIO power_5v(GPIOC, GPIO_PIN_15);
SuperGPIO power_24v_right(GPIOC, GPIO_PIN_14);
SuperGPIO power_24v_left(GPIOC, GPIO_PIN_13);

UI ui(102, 0x0166, &huart7);

OneStepGetControl OSG::mode   = OneStepGetControl::AUTO;
OneStepGetAUTO OSG::auto_mode = OneStepGetAUTO::NONE;
OSG one_step_gets(Pid(100, 0.0000, 20, 500, 9000, 0.0), Pid(1.5, 0, 2.3, 4000, 10000, 1), 4, Slope(1.2, 0),
                  Pid(15, 0, 4, 8000, 16000, 1.0), Pid(15, 0, 4, 8000, 16000, 1.0), 2, Slope {1.0, 0},
                  Pid(100, 0.0000, 20, 500, 9000, 0.0), Pid(1.5, 0, 2.3, 4000, 10000, 1), 3, Slope(3, 0),
                  Pid(15, 0, 4, 8000, 16000, 1.0), Pid(15, 0, 4, 8000, 16000, 1.0), 1, Slope(1.0, 0));

interact_dep::Actions anti_reset({-3.45, -16.612, -84.722, 0.043, 0, 0});

interact_dep::Actions get_silver_mine({0,37.604,115.54184,0,27.570,0}, {480, 720, 360, 360});
interact_dep::Actions get_silver_mine_z(Slope(0.4, 0.15, 310), interact_dep::action_status::CartesianZ_z);
interact_dep::Actions put_silver_mine_left({-51.8910751, 0.283977509, 91.6136169, 8.03382111, 90, -55.05}, {720, 180, 180, 720});

interact_dep::Actions exchange_left({-17.9960938, 36.7366142, 35.3361511, -89.4694138, -89.9465207, 52.6248474});
interact_dep::Actions exchange_right({17.9960938,36.7366142,35.3361511,89.4694138,-89.9465207,-52.6248474});

interact_dep::Actions reset1({0, -55, 145, 0, 0, 0}, {720,720,360,720});
interact_dep::Actions reset2({0, -16.3, 144.8, 0, 0, 0}, {720, 720, 360, 720});
interact_dep::Actions silver_reset({0, -55, 135, 0, 0, 0}, {720, 180, 360, 360});
interact_dep::Actions get_silver_from_left({-27.51478,10.4793777,126.511063,95.8899307,-90,-7.86681938}, {90,180,180,720});

interact_dep::Actions arm_get_gold({0, 48.4501343, 70.3479385, 0.00540079346, -28.7980728, 0}, {480, 720, 900, 720});
interact_dep::Actions arm_get_gold_z(Slope(0.1, 0.15, 120), interact_dep::action_status::CartesianX_z);

// interact_dep::Actions put_down_silver({0,55,116.5,0,-78,0});    //
// interact_dep::Actions get_silver2_mine({-36.8980751,47.0325127,101.838203,0,31.1292877,0});     //
// interact_dep::Actions get_right_y({-8.48937225,-17.2843819,-102.246674,83.5459137,89.6870422,42.9085999});   //
// interact_dep::Actions put_silver_mine_right({16.20, 14.4343681, 122.649994, 101.64, 90.78, 38.22});      //

// **************************************************************************************************** //


//std::array<interact_dep::Actions, 6> get_silver_action = {get_silver_mine,get_silver_mine_z, put_silver_mine_left,put_silver_mine_left, exchange_left, silver_reset};
std::array<interact_dep::Actions, 6> get_silver_action = {
    interact_dep::Actions({0, 37.604, 115.54184, 0, 27.570, 0},{480,720,360,360}),
    interact_dep::Actions(Slope(0.4, 0.15, 310), interact_dep::action_status::CartesianZ_z),
    interact_dep::Actions({-51.8910751, 0.283977509, 91.6136169, 8.03382111, 90, -55.05}, {720, 180, 180, 720}),
    interact_dep::Actions({-51.8910751, 0.283977509, 91.6136169, 8.03382111, 90, -55.05}, {720, 180, 180, 720}),
    interact_dep::Actions({-17.9960938, 36.7366142, 35.3361511, -89.4694138, -89.9465207, 52.6248474}),
    interact_dep::Actions({0, -55, 135, 0, 0, 0}, {720, 180, 360, 360})
};
std::array<uint32_t, 6> get_silver_time                = {3000, 2000, 2000, 500, 200, 1000};
std::array<interact_dep::ActionsGroup::exe, 7> get_silver_exe = {
    []() {
        interact.sub_board.set_pump(1);
        interact.sub_board.set_main_valve(1);
        interact.sub_board.set_lf_valve(0);
        one_step_gets.reset();
    },
    nullptr,
    []() {
        interact.sub_board.set_lf_valve(1);
        one_step_gets.left.X.status = OneStepGetXStatus::FRONT;
        one_step_gets.left.X.pos.step_set(0.09);
        one_step_gets.left.X.pos.target_set(200);
        one_step_gets.left.Y.status = OneStepGetYStatus::UP;
        one_step_gets.left.Y.pos.target_set(750);
    },
    []() {
        interact.sub_board.set_main_valve(0);
        one_step_gets.left.X.status = OneStepGetXStatus::BACK;
        one_step_gets.left.X.pos.target_set(0);
        one_step_gets.left.X.pos.step_set(0.6);
    },
    []() {},
    []() {},
    []() { interact.robo_arm.mode = interact_dep::robo_mode::NONE; }
};

std::array<interact_dep::ActionsGroup::event, 6> get_silver_event = {
    []()->bool {
        return interact.sub_board.custom_frame_rx.s.valve3 < 400;
    },
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};
interact_dep::ActionsGroup get_silver_group = {.actions_list = get_silver_action.data(),
                                               .time_list    = get_silver_time.data(),
                                               .event_list   = nullptr,
                                               .exe_list     = get_silver_exe.data(),
                                               .len          = 6,
                                               .index        = 0,
                                               .time_cnt     = 0};

// **************************************************************************************************** //



// std::array<interact_dep::Actions, 2> get_second_silver_action        = {
//     get_silver_mine,
//     get_silver_mine_z};
std::array<interact_dep::Actions, 2> get_second_silver_action        = {
    interact_dep::Actions({0, 37.604, 115.54184, 0, 27.570, 0},{480,720,360,360}),
    interact_dep::Actions(Slope(0.4, 0.15, 310), interact_dep::action_status::CartesianZ_z)
};
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
                                                      .event_list   = get_second_silver_event.data(),
                                                      .exe_list     = get_second_silver_exe.data(),
                                                      .len          = 2,
                                                      .index        = 0,
                                                      .time_cnt     = 0};

// **************************************************************************************************** //


// std::array<interact_dep::Actions, 2> get_silver_from_left_actions        = {get_silver_from_left, reset1};
std::array<interact_dep::Actions, 2> get_silver_from_left_actions        = {
    interact_dep::Actions({-27.51478, 10.4793777, 126.511063, 95.8899307, -90, -7.86681938},{90,180,180,720}),
    interact_dep::Actions({0, -55, 145, 0, 0, 0},{720,720,360,720})
};
std::array<uint32_t, 2> get_silver_from_time                       = {4000, 1000};
std::array<interact_dep::ActionsGroup::exe, 3> get_silver_from_exe = {
    []() {
        interact.sub_board.set_pump(1);
        interact.sub_board.set_main_valve(1);
    },
     []() {
         interact.sub_board.set_lf_valve(0);
    },
   []() {
       interact.robo_arm.mode = interact_dep::robo_mode::NONE;
    }

};

interact_dep::ActionsGroup get_silver_from_left_group = {.actions_list = get_silver_from_left_actions.data(),
                                                      .time_list    = get_silver_from_time.data(),
                                                      .event_list   = nullptr,
                                                      .exe_list     = get_silver_from_exe.data(),
                                                      .len          = 2,
                                                      .index        = 0,
                                                      .time_cnt     = 0};

// **************************************************************************************************** //


std::array<uint32_t, 1> put_down_silver_time = {100};
std::array<interact_dep::Actions, 1> put_down_silver_action        = {reset2};
std::array<interact_dep::ActionsGroup::exe, 2> put_down_silver_exe = {
    []() {
        interact.sub_board.set_pump(0);
        interact.sub_board.set_lb_valve(0);
        interact.sub_board.set_rb_valve(0);
        one_step_gets.left.X.status = OneStepGetXStatus::FRONT;

        one_step_gets.left.X.pos.target_set(1000);
        one_step_gets.right.X.status = OneStepGetXStatus::FRONT;

        one_step_gets.right.X.pos.target_set(-1000);
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


std::array<uint32_t, 1> put_down_gold_time = {100};
std::array<interact_dep::Actions, 1> put_down_gold_action        = {reset2};
std::array<interact_dep::ActionsGroup::exe, 2> put_down_gold_exe = {
    []() {
        interact.sub_board.set_pump(0);
        interact.sub_board.set_lf_valve(0);
        interact.sub_board.set_rf_valve(0);
        one_step_gets.left.X.status = OneStepGetXStatus::FRONT;

        one_step_gets.left.X.pos.target_set(1000);
        one_step_gets.right.X.status = OneStepGetXStatus::FRONT;

        one_step_gets.right.X.pos.target_set(-1000);
        one_step_gets.left.Y.status = OneStepGetYStatus::UP;
        one_step_gets.left.Y.pos.target_set(1450);
        one_step_gets.right.Y.status = OneStepGetYStatus::UP;
        one_step_gets.right.Y.pos.target_set(-1450);
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
        one_step_gets.left.X.status = OneStepGetXStatus::FRONT;
        one_step_gets.left.X.pos.target_set(2000);
        one_step_gets.left.Y.status = OneStepGetYStatus::UP;
        one_step_gets.left.Y.pos.target_set(200);
        one_step_gets.right.X.status = OneStepGetXStatus::FRONT;
        one_step_gets.right.X.pos.target_set(-2500);
        one_step_gets.right.Y.status = OneStepGetYStatus::UP;
        one_step_gets.right.Y.pos.target_set(-200);
    },
    []() {
        one_step_gets.left.Y.status = OneStepGetYStatus::UP;
        one_step_gets.left.Y.pos.target_set(285 + 200); //255
        one_step_gets.right.Y.status = OneStepGetYStatus::UP;
        one_step_gets.right.Y.pos.target_set(-320 - 200); //-345
    },
    []() {
        one_step_gets.left.X.status = OneStepGetXStatus::BACK;
        one_step_gets.left.X.pos.target_set(0);
        one_step_gets.right.X.status = OneStepGetXStatus::BACK;
        one_step_gets.right.X.pos.target_set(0);
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
        one_step_gets.right.X.status = OneStepGetXStatus::FRONT;
        one_step_gets.right.X.pos.target_set(-2500);
        one_step_gets.right.Y.status = OneStepGetYStatus::UP;
        one_step_gets.right.Y.pos.target_set(-200);
    },
    []() {
        one_step_gets.right.Y.status = OneStepGetYStatus::UP;
        one_step_gets.right.Y.pos.target_set(-420 - 200); //-345
    },
    []() {
        one_step_gets.right.X.status = OneStepGetXStatus::BACK;
        one_step_gets.right.X.pos.target_set(0);
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
        one_step_gets.left.X.status = OneStepGetXStatus::FRONT;
        one_step_gets.left.X.pos.target_set(2000);
        one_step_gets.left.Y.status = OneStepGetYStatus::UP;
        one_step_gets.left.Y.pos.target_set(200);
    },
    []() {
        one_step_gets.left.Y.status = OneStepGetYStatus::UP;
        one_step_gets.left.Y.pos.target_set(285 + 200); //255
    },
    []() {
        one_step_gets.left.X.status = OneStepGetXStatus::BACK;
        one_step_gets.left.X.pos.target_set(0);
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


std::array<uint32_t, 1> ready_silver2_time = {100};
std::array<interact_dep::Actions, 1> ready_silver2_action        = {anti_reset};
std::array<interact_dep::ActionsGroup::exe, 2> ready_silver2_exe = {
    []() {
        interact.polarity = interact_dep::chassis_polarity::ANTI;

        one_step_gets.left.X.status = OneStepGetXStatus::BACK;
        one_step_gets.left.X.pos.target_set(-2000);
        one_step_gets.left.Y.status = OneStepGetYStatus::UP;
        one_step_gets.left.Y.pos.target_set(1450);
        one_step_gets.right.X.status = OneStepGetXStatus::BACK;
        one_step_gets.right.X.pos.target_set(2300);
        one_step_gets.right.Y.status = OneStepGetYStatus::UP;
        one_step_gets.right.Y.pos.target_set(-1450);
    },
    []() { interact.robo_arm.mode = interact_dep::robo_mode::NONE; }};

interact_dep::ActionsGroup ready_silver2_group = {.actions_list = ready_silver2_action.data(),
                                                      .time_list    = ready_silver2_time.data(),
                                                      .event_list   = nullptr,
                                                      .exe_list     = ready_silver2_exe.data(),
                                                      .len          = 1,
                                                      .index        = 0,
                                                      .time_cnt     = 0};

// **************************************************************************************************** //



std::array<uint32_t, 4> get_silver2_time = {2500, 2500, 1500, 500};
std::array<interact_dep::Actions, 4> get_silver2_action        = {anti_reset, anti_reset, anti_reset, reset1};
std::array<interact_dep::ActionsGroup::exe, 5> get_silver2_exe = {
    []() {
        interact.polarity = interact_dep::chassis_polarity::ANTI;

        interact.sub_board.set_pump(1);
        interact.sub_board.set_lb_valve(1);
        interact.sub_board.set_rb_valve(1);
        one_step_gets.left.Y.status = OneStepGetYStatus::DOWN;
        one_step_gets.left.Y.pos.target_set(-1450);
        one_step_gets.right.Y.status = OneStepGetYStatus::DOWN;
        one_step_gets.right.Y.pos.target_set(1450);
    },
    []() {
        one_step_gets.left.Y.status = OneStepGetYStatus::UP;
        one_step_gets.left.Y.pos.target_set(1450);
        one_step_gets.right.Y.status = OneStepGetYStatus::UP;
        one_step_gets.right.Y.pos.target_set(-1450);
    },
    []() {

    },
    []() {
        interact.polarity = interact_dep::chassis_polarity::NONE;
    },
    []() { interact.robo_arm.mode = interact_dep::robo_mode::NONE; }};

interact_dep::ActionsGroup get_silver2_group = {.actions_list = get_silver2_action.data(),
                                                      .time_list    = get_silver2_time.data(),
                                                      .event_list   = nullptr,
                                                      .exe_list     = get_silver2_exe.data(),
                                                      .len          = 4,
                                                      .index        = 0,
                                                      .time_cnt     = 0};

// **************************************************************************************************** //


