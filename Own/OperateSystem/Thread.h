//
// Created by lhy on 2024/9/17.
//

#ifndef NEW_PRJ_THREAD_H
#define NEW_PRJ_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

void StartDefaultTask(void const *argument);
void OS_DJITask(void const *argument);
void OS_LedTask(void const *argument);
void OS_ErrorTask(void const *argument);
void OS_DetectTask(void const *argument);
void OS_ChassisTask(void const *argument);
void OS_RemoteCtrlTask(void const *argument);
void OS_IMUTask(void const *argument);
void OS_LKTask(void const *argument);
void OS_PCTask(void const *argument);
void OS_ImageTransTask(void const *argument);
void OS_JudgeTask(void const *argument);
void OS_TransmitTask(void const *argument);
void OS_KbTask(void const *argument);
void OS_UITask(void const *argument);
#ifdef __cplusplus
}
#endif


#endif //NEW_PRJ_THREAD_H
