//
// Created by Administrator on 24-10-3.
//

#include "Interact/Interact.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "cmsis_os.h"
extern QueueHandle_t xRxedChars;
extern uint8_t cli_buffer[];
extern osThreadId IMAGEATRANS_TASHandle;

#include "LetterShell/shell.h"
#include "LetterShell/shell_port.h"

#ifdef __cplusplus
}
#endif

#include "ThreadConfig.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    UNUSED(huart);
#if USING_UART_IT

    auto& serial = liaohy::serial;

    if (huart == serial.uart) {
        //    liaohy::serial.println("Hello, World!");
        serial.tx_buffer = serial.rx_buffer->read(reinterpret_cast<uint16_t&>(serial.len));
        serial.print();
        //    uint8_t ch[15] = {0};
        //    HAL_UART_Receive_IT(&huart1, (uint8_t *) ch, 15);

        serial.read_it(15);
        //        HAL_UART_Receive_IT(serial.uart, serial.rx_buffer->next(15)->data, 15);
    }

#endif
}

int16_t x, y;
static uint8_t cnt = 0;
extern uint8_t cmd;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    UNUSED(Size);
#if USING_UART_IDLE
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (huart == interact.remote_control.uartPlus.uart) {
        interact.remote_control.update();
        if (cnt++ > 5) {
            xEventGroupSetBitsFromISR(osEventGroup, REMOTE_CONTROL_START_EVENT, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    } else if (huart == interact.image_trans.uartPlus.uart) {

        interact.image_trans.update();
			xHigherPriorityTaskWoken = pdTRUE;
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

//    if (huart == uartPlus10.uart) {
//        for (int i = 0; i < Size; ++i) {
//            xQueueSendFromISR(xRxedChars, &cli_buffer[i], &xHigherPriorityTaskWoken);
//            //            shellHandler(&shell, cli_buffer[i]);
//        }
//    }

#endif
}
