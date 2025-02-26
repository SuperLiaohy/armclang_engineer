//
// Created by Administrator on 24-10-22.
//

#include "lkMotor.hpp"

#if USING_LKMOTOR == 1

void lkMotor::enable() {
    uint8_t data[8] = {0x88, 0, 0, 0, 0, 0, 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::disable() {
    uint8_t data[8] = {0x81, 0, 0, 0, 0, 0, 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::close() {
    uint8_t data[8] = {0x80, 0, 0, 0, 0, 0, 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::voltageControl(int16_t target) {
    uint8_t data[8] = {0xa0, 0, 0, 0, *(uint8_t *) (&target), *((uint8_t *) (&target) + 1), 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::torqueControl(int16_t target) {
    uint8_t data[8] = {0xa1, 0, 0, 0, *(uint8_t *) (&target), *((uint8_t *) (&target) + 1), 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::speedControl(int32_t target) {
    uint8_t data[8] = {0xa2, 0, 0, 0, *(uint8_t *) (&target), *((uint8_t *) (&target) + 1),
                       *((uint8_t *) (&target) + 2), *((uint8_t *) (&target) + 3)};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::totalposition1Control(int32_t target) {
    uint8_t data[8] = {0xa3, 0, 0, 0, *(uint8_t *) (&target), *((uint8_t *) (&target) + 1),
                       *((uint8_t *) (&target) + 2), *((uint8_t *) (&target) + 3)};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::totalposition2Control(uint16_t speed, int32_t totalposition) {
    uint8_t data[8] = {0xa4, 0, *(uint8_t *) (&speed), *((uint8_t *) (&speed) + 1), *(uint8_t *) (&totalposition),
                       *((uint8_t *) (&totalposition) + 1), *((uint8_t *) (&totalposition) + 2),
                       *((uint8_t *) (&totalposition) + 3)};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::position1Control(uint8_t spin, uint32_t target) {
    uint8_t data[8] = {0xa5, spin, 0, 0, *(uint8_t *) (&target), *((uint8_t *) (&target) + 1),
                       *((uint8_t *) (&target) + 2), *((uint8_t *) (&target) + 3)};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::position2Control(uint8_t spin, uint16_t speed, uint32_t target) {
    uint8_t data[8] = {0xa6, spin, *(uint8_t *) (&speed), *((uint8_t *) (&speed) + 1), *(uint8_t *) (&target),
                       *((uint8_t *) (&target) + 1), *((uint8_t *) (&target) + 2), *((uint8_t *) (&target) + 3)};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::addposition1Control(int32_t target) {
    uint8_t data[8] = {0xa7, 0, 0, 0, *(uint8_t *) (&target), *((uint8_t *) (&target) + 1),
                       *((uint8_t *) (&target) + 2), *((uint8_t *) (&target) + 3)};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::addposition2Control(uint16_t speed, int32_t target) {
    uint8_t data[8] = {0xa8, 0, *(uint8_t *) (&speed), *((uint8_t *) (&speed) + 1), *(uint8_t *) (&target),
                       *((uint8_t *) (&target) + 1), *((uint8_t *) (&target) + 2), *((uint8_t *) (&target) + 3)};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::read_feedback() {
    uint8_t data[8] = {0x9c, 0, 0, 0, 0, 0, 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::read_encoder() {
    uint8_t data[8] = {0x90, 0, 0, 0, 0, 0, 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::readPid() {
    uint8_t data[8] = {0x30, 0, 0, 0, 0, 0, 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::writeRAMPid() {
    uint8_t data[8] = {0x31, 0, foc_pid.pos_p, foc_pid.pos_i, foc_pid.speed_p, foc_pid.speed_i, foc_pid.current_p,
                       foc_pid.current_i};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::writeROMPid() {
    uint8_t data[8] = {0x32, 0, foc_pid.pos_p, foc_pid.pos_i, foc_pid.speed_p, foc_pid.speed_i, foc_pid.current_p,
                       foc_pid.current_i};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::read_error() {
    uint8_t data[8] = {0x9a, 0, 0, 0, 0, 0, 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::clear_error() {
    uint8_t data[8] = {0x9b, 0, 0, 0, 0, 0, 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}

void lkMotor::read_totalposition() {
    uint8_t data[8] = {0x92, 0, 0, 0, 0, 0, 0, 0};
    superCan->send(id + Motor<lkMotor>::foc.TX_LOW_ID, data);
}


//float lkMotor::SelfDoublePid::update(float extern_input, float extern_target, float inter_input) {
//    return DoublePid::update(extern_input, extern_target, inter_input);
//}

void lkMotorBoard::init(float inter_p, float inter_i, float inter_d, float inter_maxI, float inter_maxOut,
                        float extern_p, float extern_i, float extern_d, float extern_maxI, float extern_maxOut) {
//    this->ctrlId = ctrlId;
    pid.set(inter_p, inter_i, inter_d, inter_maxI, inter_maxOut);
    doublePid.inter_pid.set(inter_p, inter_i, inter_d, inter_maxI, inter_maxOut);
    doublePid.extern_pid.set(extern_p, extern_i, extern_d, extern_maxI, extern_maxOut);
}

void lkMotorBoard::readData(const uint8_t *data) {

}

void lkMotorBoard::CalcTotalPos() {

}
float lkMotorBoard::SelfDoublePid::update(float extern_input, float extern_target, float inter_input) {
    float error = extern_target - extern_input;
    if (error > 32767) {
        error -= 32767;
    } else if (error < -32767) {
        error += 32767;
    }
    extern_input = extern_target - error;
    output = inter_pid.update(extern_pid.update(extern_target, extern_input), inter_input);
    return output;
}
#endif //USING_LKMOTOR
