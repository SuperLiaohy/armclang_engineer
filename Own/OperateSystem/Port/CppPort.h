//
// Created by lhy on 2024/9/19.
//

#ifndef OWN_CPP_PORT_C_H
#define OWN_CPP_PORT_C_H

#ifdef __cplusplus
extern "C" {
#endif


void start_c(void);

void flash_init_c(void);
void flash_write_c(void);
void flash_read_c(void);

void hard_fault_reset();
#ifdef __cplusplus
}
#endif

#endif //OWN_CPP_PORT_C_H
