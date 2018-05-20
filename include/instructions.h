#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>

// Instructions
void op_zero(uint16_t op);
void op_jp(uint16_t op);
void op_call(uint16_t op);
void op_se(uint16_t op);
void op_sne(uint16_t op);
void op_se_reg(uint16_t op);
void op_ld(uint16_t op);
void op_add(uint16_t op);
void op_vx_vy(uint16_t op);
void op_sne_vx_vy(uint16_t op);
void op_ld_i(uint16_t op);
void op_jp_nnn_v0(uint16_t op);
void op_rnd(uint16_t op);
void op_drw(uint16_t op);
void op_skp(uint16_t op);
void op_F(uint16_t op);

#endif
