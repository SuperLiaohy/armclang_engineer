//
// Created by Administrator on 25-1-10.
//
#include "CDC/SuperCDC.hpp"
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "Motor/M2006Diff.hpp"
#include "Motor/Motor.hpp"
#include "Motor/lkMotor.hpp"
#include "RoboArm/RoboArm.hpp"
#include "ThreadConfig.h"

uint8_t CPU_RunInfo[512];
void PCTask() {
    uint32_t cnt = 0;
    interact.pc.tx_frame.head = interact.pc.head;
    interact.pc.tx_frame.tail = interact.pc.tail;
    interact.pc.tx_frame.cmd = 0xA5;
    while (1) {
        if (cnt++%10==0) {
            interact.transmit_relative_pos(roboArm.relative_pos);
                   // memset(CPU_RunInfo, 0, 512);
                   // vTaskList((char*)&CPU_RunInfo); //获取任务运行时间信息
                   // SuperCDC::printf("---------------------------------------------\r\n");
                   // SuperCDC::printf("任务名       任务状态     优先级     剩余栈     任务序号\r\n");
                   // SuperCDC::printf("%s", CPU_RunInfo);
                   // SuperCDC::printf("---------------------------------------------\r\n");
                   // memset(CPU_RunInfo, 0, 512);
                   // vTaskGetRunTimeStats((char*)&CPU_RunInfo);
                   // SuperCDC::printf("任务名         运行计数     使用率\r\n");
                   // SuperCDC::printf("%s", CPU_RunInfo);
                   // SuperCDC::printf("---------------------------------------------\r\n\n");
                   // osDelay(1000); /* 延时500个tick */
        }
        osDelay(1);
    }
}

