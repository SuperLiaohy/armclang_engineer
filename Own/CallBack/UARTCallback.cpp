//
// Created by Administrator on 24-10-3.
//

#include "Interact/Interact.hpp"
#include "Judge/ui.hpp"
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

static uint8_t cnt = 0;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    UNUSED(Size);
#if USING_UART_IDLE
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (huart == interact.remote_control.uartPlus.uart) {
        ++interact.remote_control.uartPlus.rx_cnt;
        interact.remote_control.update(interact.key_board);
        if (cnt++ > 5) {
            xEventGroupSetBitsFromISR(osEventGroup, REMOTE_CONTROL_START_EVENT, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    } else if (huart == interact.image_trans.uartPlus.uart) {
        using namespace crc;
        ++interact.image_trans.uartPlus.rx_cnt;
        if (interact.image_trans.update(Size)) {
            auto data                      = interact.image_trans.uartPlus.rx_buffer;
            uint16_t len                   = (data[2] << 8 | data[1]);
            interact.image_trans.rx_cmd_id = data[6] << 8 | data[5];
            if (verify_crc16_check_sum(data, len + 9)) {
                switch (interact.image_trans.rx_cmd_id) {
                    case 0x302:
                        interact.receive_custom(&data[7]);
                        break;
                    case 0x304:
                        ++interact.image_trans.rx_cnt;
                        break;
                    case 0x306:
                        break;
                    case 0x309:
                        break;
                    default:
                        break;
                }
            }
        }
        interact.image_trans.uartPlus.receive_dma_idle(100);
    } else if (huart == interact.sub_board.uartPlus.uart) {
        ++interact.sub_board.uartPlus.rx_cnt;
        interact.sub_board.get_feedback();
        interact.sub_board.start_receive();
    } else if (huart == ui.uartPlus.uart) {
        using namespace crc;
        ++ui.uartPlus.rx_cnt;
        for (int i = 0; i < Size - 9; ++i) {
            if (ui.uartPlus.rx_buffer[i]==0xA5) {
                auto data                      = &ui.uartPlus.rx_buffer[i];
                uint16_t len                   = (data[2] << 8 | data[1]);
                auto rx_cmd_id = data[6] << 8 | data[5];
                if (verify_crc16_check_sum(data, len + 9)) {
                    switch (rx_cmd_id) {
                        case 0x201:
                            ui.ui_frame->data_frame.sender_id = data[7];
                            switch (data[7]) {
                                case 2:
                                    ui.ui_frame->data_frame.receiver_id = 0x102;
                                    break;
                                case 102:
                                    ui.ui_frame->data_frame.receiver_id = 0x166;
                                    break;
                            }
                            break;
                        case 0x301:
                            ++ui.rx_cnt;
                            break;
                        case 0x306:
                            break;
                        case 0x309:
                            break;
                        default:
                            break;
                    }
                    i+=len-1;
                }
            }
        }
        ui.start_receive();
    }

    //    if (huart == uartPlus10.uart) {
    //        for (int i = 0; i < Size; ++i) {
    //            xQueueSendFromISR(xRxedChars, &cli_buffer[i], &xHigherPriorityTaskWoken);
    //            //            shellHandler(&shell, cli_buffer[i]);
    //        }
    //    }

#endif
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart) {
    if (huart == interact.sub_board.uartPlus.uart) {
        interact.sub_board.start_receive();
    } else if (huart == interact.image_trans.uartPlus.uart) {
        interact.image_trans.uartPlus.receive_dma_idle(100);
        ++interact.image_trans.uartPlus.err_cnt;
    } else if (huart == interact.remote_control.uartPlus.uart) {
        ++interact.remote_control.uartPlus.err_cnt;
    } else if (huart == ui.uartPlus.uart) {
        ++ui.uartPlus.err_cnt;
        ui.get_feedback();
        ui.start_receive();
    }
};
void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef* huart) {
    if (huart == interact.sub_board.uartPlus.uart) {
        interact.sub_board.start_receive();
    } else if (huart == interact.image_trans.uartPlus.uart) {
    } else if (huart == interact.remote_control.uartPlus.uart) {
    } else if (huart == ui.uartPlus.uart) {
        ++ui.uartPlus.err_cnt;
        ui.get_feedback();
        ui.start_receive();
    }
};
