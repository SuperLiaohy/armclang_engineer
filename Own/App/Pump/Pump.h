//
// Created by Administrator on 24-10-26.
//

#ifndef DM_42_PUMP_H
#define DM_42_PUMP_H
#include "Uart/SuperUart.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

#ifdef __cplusplus
}
#endif
namespace pump_dep {
    enum STATUS {
        OPEN,
        CLOSE,
    };

    enum FETCHING {
        FETCH,
        NO_FETCH,
    };
    struct Storge {
        STATUS valve;
        FETCHING fetch;

        void open() {valve = OPEN;}
        void close() {valve = CLOSE;}
    };
}
class Pump {
public:
    pump_dep::STATUS relays;

    pump_dep::Storge storge[5];

    pump_dep::STATUS positive_valve;

    void open(uint8_t num) {
        switch (num) {
            case 0:
                relays = pump_dep::OPEN;
                break;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                storge[num-1].open();
                break;
            case 6:
                positive_valve = pump_dep::OPEN;
                break;
        }
    }

    void close(uint8_t num) {
        switch (num) {
            case 0:
                relays = pump_dep::CLOSE;
                break;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                storge[num-1].close();
                break;
            case 6:
                positive_valve = pump_dep::CLOSE;
                break;
        }    }

private:
    SuperUart* uartPlus;
};

#endif //DM_42_PUMP_H
