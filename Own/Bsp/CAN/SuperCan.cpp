//
// Created by liaohy on 9/5/24.
//

#include "SuperCan.hpp"


SuperCan::SuperCan(FDCAN_HandleTypeDef *_hcan, uint32_t _fifo, uint32_t _fifo_start) {
    this->hcan = _hcan;
    this->fifo = _fifo;
    this->fifo_start = _fifo_start;
}

void SuperCan::filter_config(uint16_t filter_number, filter_mode filterMode, uint32_t reg_1, uint32_t reg_2) {
    FDCAN_FilterTypeDef sFilterConfig;
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = filter_number;
    switch (filterMode) {
        case filter_mode::RANGE:
            sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
            break;
        case filter_mode::MASK:
            sFilterConfig.FilterType = FDCAN_FILTER_MASK;
            break;
        case filter_mode::LIST:
            sFilterConfig.FilterType = FDCAN_FILTER_DUAL;
            break;
        default:
            break;
    }
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = reg_1;
    sFilterConfig.FilterID2 = reg_2;

    if (HAL_FDCAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK) {
        Error_Handler();
    }
    HAL_FDCAN_ConfigGlobalFilter(hcan,FDCAN_REJECT,FDCAN_REJECT,FDCAN_REJECT_REMOTE,FDCAN_REJECT_REMOTE);
    HAL_FDCAN_ConfigFifoWatermark(hcan, FDCAN_CFG_RX_FIFO0, 1);
}

void SuperCan::start() {
    HAL_FDCAN_Start(hcan);
    HAL_FDCAN_ActivateNotification(hcan, fifo_start, 0);
}

void SuperCan::transmit_pdata(uint32_t id, uint8_t *data) {
    ++tx_cnt;
    tx_header.Identifier = id;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = 8;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker = 0;
    if (HAL_FDCAN_AddMessageToTxFifoQ(hcan, &tx_header, data) != HAL_OK) { return; }
}


void SuperCan::transmit(uint32_t id, int16_t data1, int16_t data2, int16_t data3, int16_t data4) {

    ++tx_cnt;

    tx_header.Identifier          = id;
    tx_header.IdType              = FDCAN_STANDARD_ID;
    tx_header.DataLength          = 8;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch       = FDCAN_BRS_OFF;
    tx_header.FDFormat            = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker       = 0;
    tx_data[0] = (data1 >> 8) & 0xff;
    tx_data[1] = (data1) & 0xff;
    tx_data[2] = (data2 >> 8) & 0xff;
    tx_data[3] = (data2) & 0xff;
    tx_data[4] = (data3 >> 8) & 0xff;
    tx_data[5] = (data3) & 0xff;
    tx_data[6] = (data4 >> 8) & 0xff;
    tx_data[7] = (data4) & 0xff;    if (HAL_FDCAN_AddMessageToTxFifoQ(hcan, &tx_header, tx_data) != HAL_OK) {
        return;
    }
}