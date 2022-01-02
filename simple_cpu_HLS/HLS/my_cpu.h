// Adapted from
// https://gitlab.cs.washington.edu/cse599s/hls-tutorials/-/tree/master/part3

#ifndef MY_CPU_H_
#define MY_CPU_H_

#include <stdint.h>
#include <ap_int.h>

// Instruction width
#define INSN_W 32
// Data width
#define DATA_W 32

// Instruction depth
#define LOG_INSN_SIZE 10
#define INSN_SIZE (1 << LOG_INSN_SIZE)
// Data depth
#define LOG_DATA_SIZE 10
#define DATA_SIZE (1 << LOG_DATA_SIZE)
// Register file depth
#define LOG_REGFILE_SIZE 4
#define REGFILE_SIZE (1 << LOG_REGFILE_SIZE)

// Opcode width
#define OPCODE_W 3
// Immediate width
#define IMM_W 16

// Register Locations
#define R0  0
#define R1  1
#define R2  2
#define R3  3
#define R4  4
#define R5  5
#define R6  6
#define R7  7
#define R8  8
#define R9  9
#define R10 10
#define R11 11
#define R12 12
#define R13 13
#define R14 14
#define R15 15

// Opcodes
#define OPCODE_FINISH 0
#define OPCODE_WR_IMM 1
#define OPCODE_LOAD   2
#define OPCODE_STORE  3
#define OPCODE_BEQ    4
#define OPCODE_BNE    5
#define OPCODE_ADD    6

typedef ap_uint<INSN_W> insn_T;

// Data width
typedef ap_int<DATA_W> data_T;

// Simple CPU
void cpu(insn_T i_mem[INSN_SIZE], data_T d_mem[DATA_SIZE]);

#endif  // MY_CPU_H_
