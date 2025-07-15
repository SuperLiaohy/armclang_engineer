//
// Created by Administrator on 24-11-2.
//
#pragma once

#include <array>

#ifdef __cplusplus
extern "C" {
#endif
#include "cmsis_os.h"
#include "tim.h"
#ifdef __cplusplus
}
#endif

#include "RingBuffer/RingBuffer.h"

class Buzzer {
public:
    enum NoteFrequency : uint16_t {
        LC        = 262,
        LD        = 294,
        LE        = 330,
        LF        = 349,
        LG        = 392,
        LA        = 440,
        LAP_LBN   = 466,
        LB        = 494,
        MC        = 523,
        MD        = 587,
        ME        = 659,
        MEP       = 740,
        MF        = 698,
        MG        = 784,
        MA        = 880,
        MAP_MBN   = 932,
        MB        = 988,
        HC        = 1046,
        HD        = 1175,
        HE        = 1318,
        HF        = 1397,
        HG        = 1568,
        HA        = 1760,
        HB        = 1976,
        MUSIC_EOF = 40000,
    };

    static constexpr std::array<uint16_t, 8> error_music = {HC, 20000, HC, 20000, HC, 20000, HC, 20000};
    // 定义旋律，长度为24个音符
    static constexpr std::array<uint16_t, 24> melody = {
        // 第一部分：上升旋律，欢快开场
        MC, ME, MG, MA, // C E G A
        MA, MG, ME, MC, // A G E C（回落）

        // 第二部分：高音重复，增强响度
        HC, HE, HG, HA, // C E G A（高八度）
        HA, HG, HE, HC, // A G E C（回落）

        // 第三部分：结尾，简洁明亮
        MG, MA, MB, HA, // G A B A
        MG, ME, MC, MC  // G E C C（结束）
    };
    static constexpr std::array<uint16_t, 96> Romance = {
        ME, ME, ME, ME, MD, MC, MC,    LB,    LA, LA, MC, ME, MA, MA, MA,    MA,   MG, MD,      MF,      ME,
        MD, MD, ME, MF, ME, MF, ME,    MEP,   MF, ME, ME, MD, MC, MC, LB,    LA,   LB, LB,      LB,      LB,
        MC, LB, LA, LA, LA, LA, 20000, 20000, ME, ME, ME, ME, MD, MC, MC,    LB,   LB, LB,      LAP_LBN, LB,
        MA, MA, MA, MA, MB, MA, MA,    MG,    MG, MG, MA, MB, HC, HC, HC,    HC,   MB, MAP_MBN, MA,      MA,
        MA, MA, ME, MD, ME, ME, ME,    ME,    MF, MD, MC, MC, MC, MC, 20000, 20000};

public:
    Buzzer(TIM_HandleTypeDef* htim, uint16_t Channel)
        : htim(htim)
        , Channel(Channel) {};

    void Start();
    void SetFreq(uint16_t freq, uint16_t pulse = 300);
    void Stop();

    template<uint16_t size, auto delay = osDelay> void StartMusic(const std::array<uint16_t, size>& music);
    template<uint16_t size> void PushMusic(const std::array<uint16_t, size>& music);
    template<auto delay = osDelay> bool StartMusic();

    void CleanMusic() {music_buffer.reset();};

private:
    TIM_HandleTypeDef* htim;
    uint16_t Channel;
    RingBuffer<100, uint16_t> music_buffer;
};

template<uint16_t size, auto delay> void Buzzer::StartMusic(const std::array<uint16_t, size>& music) {
    for (auto item: music) {
        SetFreq(item);
        delay(100);
    }
    SetFreq(20000, 20);
}

template<uint16_t size> void Buzzer::PushMusic(const std::array<uint16_t, size>& music) {
    music_buffer.write_data(music.data(), size);
    uint16_t end[2] = {20000, MUSIC_EOF};
    music_buffer.write_data(end, 2);
}

template<auto delay> bool Buzzer::StartMusic() {
    if (!music_buffer.is_empty()) {
        auto music = music_buffer.get();
        music_buffer.add_read();
        SetFreq(music);
        delay(100);
        return true;
    }
    return false;
}

extern Buzzer buzzer;
