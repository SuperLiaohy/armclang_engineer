//
// Created by lhy on 2024/9/17.
//
#pragma once


#ifdef __cplusplus
extern "C" {
#endif

void StartDefaultTask(void const *argument);
void OS_DJITask(void const *argument);
void OS_LedTask(void const *argument);
void OS_ErrorTask(void const *argument);
void OS_DebugTask(void const *argument);
void OS_ChassisTask(void const *argument);
void OS_RemoteCtrlTask(void const *argument);
void OS_IMUTask(void const *argument);
void OS_LKTask(void const *argument);
void OS_PCTask(void const *argument);
void OS_ImageTransTask(void const *argument);
void OS_JudgeTask(void const *argument);
void OS_KbTask(void const *argument);
void OS_SubBoardTask(void const *argument);
void OS_ArmInitTask(void const *argument);
void OS_BuzzerTask(void const *argument);
void OS_ArmTask(void const *argument);
void OS_OneStepGetTask(void const *argument);
#ifdef __cplusplus
}
#endif


