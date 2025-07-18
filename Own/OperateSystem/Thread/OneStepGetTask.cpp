//
// Created by Administrator on 25-5-2.
//
#include "Interact/Interact.hpp"
#include "Motor/M2006Diff.hpp"
#include "Motor/Param/M2006Diff.hpp"
#include "OneStepGet/OneStepGet.hpp"
void OneStepGetTask() {
    one_step_gets.rotate.set_target(180);
    uint32_t time = 0;
    while (1) {
        if (time++ % 3) {
            one_step_gets.rotate.move();
//            one_step_gets.rotate.read();
        }
        one_step_gets.Yright.state_handle();
        one_step_gets.Yleft.state_handle();
        one_step_gets.Xright.state_handle();
        one_step_gets.Xleft.state_handle();
        one_step_gets.rotate_move.state_handle();

        one_step_gets.Yright.move_handle();
        one_step_gets.Yleft.move_handle();
        one_step_gets.Xright.move_handle();
        one_step_gets.Xleft.move_handle();
        one_step_gets.rotate_move.move_handle();

//        canPlus3.transmit(0x200,
//                          one_step_gets.Yright.Motor.speed_output(),
//                          one_step_gets.Yleft.Motor.speed_output(),
//                          one_step_gets.Xright.Motor.speed_output(),
//                          one_step_gets.Xleft.Motor.speed_output());
        canPlus3.transmit(0x200,
//                          0,
                          one_step_gets.Yright.Motor.speed_output(),
//0,
                          one_step_gets.Yleft.Motor.speed_output(),
//0,
                          one_step_gets.Xright.Motor.speed_output(),
                          one_step_gets.Xleft.Motor.speed_output());
        canPlus3.transmit(M2006::foc.TX_HIGH_ID,0,one_step_gets.rotate_move.Motor.speed_output(),0,0);
        osDelay(1);
    }
}