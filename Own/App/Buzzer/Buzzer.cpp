//
// Created by Administrator on 24-11-2.
//

#include "Buzzer.hpp"


#define  LC 		262
#define  LD 		294
#define  LE 		330
#define  LF 		349
#define  LG 		392
#define  LA 		440
#define  LAP_LBN	466
#define  LB 		494
#define  MC			523
#define  MD			587
#define  ME			659
#define  MEP		740
#define  MF			698
#define  MG			784
#define  MA			880
#define  MAP_MBN	932
#define  MB			988
#define  HC			1046
#define  HD			1175
#define  HE			1318
#define  HF			1397
#define  HG			1568
#define  HA			1760
#define  HB			1976

void Buzzer::StartMusic(uint16_t *music, uint16_t size) {
    for (int i = 0; i < size; i++) {
        SetFreq(music[i]);
        osDelay(150);
        SetFreq(20000,20);
        osDelay(150);
    }
}


uint16_t Romance[96]=
        {
                ME,ME,ME, ME,MD,MC, MC,LB,LA, LA,MC,ME, MA,MA,MA,
                MA,MG,MD, MF,ME,MD, MD,ME,MF, ME,MF,ME, MEP,MF,ME,
                ME,MD,MC, MC,LB,LA, LB,LB,LB, LB,MC,LB, LA,LA,LA, LA,20000,20000,
                ME,ME,ME, ME,MD,MC, MC,LB,LB, LB,LAP_LBN,LB, MA,MA,MA,
                MA,MB,MA, MA,MG,MG, MG,MA,MB, HC,HC,HC, HC,MB,MAP_MBN,
                MA,MA,MA, MA,ME,MD, ME,ME,ME, ME,MF,MD, MC,MC,MC, MC,20000,20000
        };

uint16_t error_music[8] = {HC, 20000, HC, 20000, HC, 20000, HC, 20000};