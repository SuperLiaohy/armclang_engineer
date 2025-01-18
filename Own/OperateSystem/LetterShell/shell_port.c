/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"
#include "stm32h7xx_hal.h"
#include "usart.h"
//#include "cevent.h"
#include "log/log.h"

uint8_t cli_buffer[64] __attribute__((section(".RAM_D1")));
QueueHandle_t xRxedChars;

Shell shell;
char shellBuffer[512] __attribute__((section(".RAM_D1")));

static SemaphoreHandle_t shellMutex;

void uartLogWrite(char *data, short len);

Log uartLog = {
        .write = uartLogWrite,
        .active = 1,
        .level = LOG_DEBUG,
        .lock = NULL,
        .unlock = NULL,
};

void uartLogWrite(char *data, short len) {
    if (uartLog.shell) {
        shellWriteEndLine(uartLog.shell, data, len);
    }
}

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len) {
    HAL_UART_Transmit(&huart10, (uint8_t *) data, len, 0xffff);
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
//short userShellRead(char *data, unsigned short len)
//{
//    return HAL_UART_Receive(&huart10, (uint8_t *)data, len, 0);
//}

/**
 * @brief 用户shell上锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellLock(Shell *shell) {
    xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY);
    return 0;
}

/**
 * @brief 用户shell解锁
 * 
 * @param shell shell
 * 
 * @return int 0
 */
int userShellUnlock(Shell *shell) {
    xSemaphoreGiveRecursive(shellMutex);
    return 0;
}

/**
 * @brief 用户shell上锁
 *
 * @param log log
 *
 * @return int 0
 */
int userLogLock(struct log_def *log) {
    xSemaphoreTakeRecursive(shellMutex, portMAX_DELAY);
    return 0;
}

/**
 * @brief 用户log解锁
 *
 * @param log log
 *
 * @return int 0
 */
int userLogUnlock(struct log_def *log) {
    xSemaphoreGiveRecursive(shellMutex);
    return 0;
}

int cnt_shell;
SHELL_EXPORT_VAR(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_VAR_INT), cnt_shell, &cnt_shell, test);
int *cnt_point = &cnt_shell;
SHELL_EXPORT_VAR(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_VAR_INT), cnt_point, &cnt_point, cnt address);

void show(int cmd) {
    shellPrint(&shell, "show %d\r\n", cmd);
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE (SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN, show, show, show var);
void show_from_point(int cmd) {
    int* pointer = (int*)cmd;
    shellPrint(&shell, "show %d\r\n", *pointer);
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE (SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN, show_from_point, show_from_point, show var from point);
/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void) {
    xRxedChars = xQueueCreate(64, ( unsigned portBASE_TYPE ) sizeof( signed char ));

    HAL_UARTEx_ReceiveToIdle_DMA(&huart10, cli_buffer, 64);

    shellMutex = xSemaphoreCreateMutex();

    shell.write = userShellWrite;
//    shell.read = userShellRead;
    shell.lock = userShellLock;
    shell.unlock = userShellUnlock;

    logRegister(&uartLog, &shell);
    shellInit(&shell, shellBuffer, 512);
    osThreadDef(shellTask, shellTask, osPriorityNormal, 0, 512);

//    if (xTaskCreate(shellTask, "shell", 512, &shell, 5, NULL) != pdPASS) {
    if (osThreadCreate(osThread(shellTask), NULL)) {
        logError("shell task creat failed");
    } else {
        logInfo("shell task creat success");
    }
    uartLog.lock = userLogLock;
    uartLog.unlock = userLogUnlock;
}
//CEVENT_EXPORT(EVENT_INIT_STAGE2, userShellInit);

