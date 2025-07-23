//
// Created by Administrator on 25-1-21.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"

uint16_t cmd_id = 0;
uint16_t cmd    = 0;

void ImageTransTask() {
    using namespace crc;
    auto &image_trans = interact.image_trans;

    image_trans.set_map_back(0);
    uint32_t time = 0;
    ImageTrans::frame_template frame{};
    uint8_t buffer[21];

    while (1) {
        ++time;
        image_trans.get_custom_feedback(roboArm.relative_pos);
        do {
            switch (image_trans.read_state()) {
                case ImageTrans::State::WAIT_ENOUGH_HEADER: {
                    if (image_trans.rb.size() > 4)
                        image_trans.set_state(ImageTrans::State::WAIT_FRAME_SOF);
                }
                    break;
                case ImageTrans::State::WAIT_FRAME_SOF: {
                    if (image_trans.rb.get() == ImageTrans::SOF) {
                        image_trans.rb.get_data(reinterpret_cast<uint8_t *>(&frame.header), 5);
                        if (verify_crc8_check_sum(reinterpret_cast<uint8_t *>(&frame.header), 5))
                            image_trans.set_state(ImageTrans::State::WAIT_FULL_FRAME);
                        else
                            image_trans.set_state(ImageTrans::State::ERROR_CRC8);
                    } else if (image_trans.rb.get() == 0xA9 && image_trans.rb[1] == 0x53)
                        image_trans.set_state(ImageTrans::State::RC_FRAME);
                    else {
                        image_trans.rb.add_read();
                        if (image_trans.rb.size() <= 4)
                            image_trans.set_state(ImageTrans::State::WAIT_ENOUGH_HEADER);
                    }
                }
                    break;
                case ImageTrans::State::WAIT_FULL_FRAME: {
                    if (image_trans.rb.size() >= frame.header.data_length + 4) {
                        image_trans.rb.get_data(reinterpret_cast<uint8_t *>(&frame.cmd_id),
                                                frame.header.data_length + 4);
                        if (verify_crc8_check_sum(reinterpret_cast<uint8_t *>(&frame), frame.header.data_length + 9)) {
                            switch (frame.cmd_id) {
                                case 0x302:
                                    interact.receive_custom(frame.data);
                                    ++image_trans.cnt;
                                    break;
                                case 0x304:
//                                image_trans.update_keyboard(interact.key_board);

                                    break;
                                case 0x306:
                                    break;
                                case 0x309:
                                    break;
                                default:
                                    break;
                            }
                            image_trans.set_state(ImageTrans::State::GET_FULL_FRAME);
                        } else
                            image_trans.set_state(ImageTrans::State::ERROR_CRC16);
                    }
                }
                    break;
                case ImageTrans::State::ERROR_CRC8:
                case ImageTrans::State::ERROR_CRC16:
                case ImageTrans::State::GET_FULL_FRAME:
                    image_trans.set_state(ImageTrans::State::WAIT_ENOUGH_HEADER);
                    break;
                case ImageTrans::State::RC_FRAME:
                    if (image_trans.rb.size() > 21) {
                        image_trans.rb.get_data(buffer, 21);
                        if (verify_crc16_check_sum(buffer, 21)) {
                            image_trans.update_rc_keyboard(buffer, interact.key_board);
                            image_trans.set_state(ImageTrans::State::WAIT_ENOUGH_HEADER);
                        } else {
                            image_trans.set_state(ImageTrans::State::ERROR_CRC16);
                        }
                    }

                    break;
                default:
                    image_trans.set_state(ImageTrans::State::WAIT_ENOUGH_HEADER);
                    break;
            }
        } while (image_trans.is_change());

        if (time % 100) {
            if (interact.robo_arm.mode == interact_dep::robo_mode::CUSTOM) {
                image_trans.set_map_back(0);
            } else {
                image_trans.set_map_back(1);
            }
            image_trans.transmit_custom_frame();
        }

        osDelay(1);
    }
}
