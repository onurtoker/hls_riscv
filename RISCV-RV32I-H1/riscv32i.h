// RISCV-32I header file
// https://github.com/jameslzhu/riscv-card/blob/master/riscv-card.pdf

#ifndef RISCV32I_H_
#define RISCV32I_H_

#include <stdint.h>
#include <ap_int.h>

// Instruction width
#define ARCH 	32

// Memory depth
#define LOG_MEM_SIZE 		10
#define MEM_SIZE 			(1 << LOG_MEM_SIZE)

// Register file depth
#define LOG_REGFILE_SIZE	5
#define REGFILE_SIZE		(1 << LOG_REGFILE_SIZE)

// opcode width and mask
#define OPCODE_WIDTH 		7
#define OPCODE_MASK 		((1 << OPCODE_WIDTH) - 1)

// rd, rs1, rs2
#define RFA_WIDTH 			5

// func3_t, func7_t
#define FUNC3_WIDTH				3
#define FUNC3_MASK				((1 << FUNC3_WIDTH) - 1)
#define FUNC3_LOC				12
typedef ap_uint<FUNC3_WIDTH> 	func3_t;
#define FUNC7_WIDTH				7
#define FUNC7_MASK				((1 << FUNC7_WIDTH) - 1)
#define FUNC7_LOC				25
typedef ap_uint<FUNC7_WIDTH> 	func7_t;

// funcx_t
typedef ap_uint<FUNC7_WIDTH+FUNC3_WIDTH> 	funcx_t;

// Opcodes
#define OPCODE_R 	0x33
#define OPCODE_IA	0x13
#define OPCODE_IM   0x03
#define OPCODE_S	0x23
#define OPCODE_B	0x63
#define OPCODE_J	0x6F
#define OPCODE_IJ	0x67
#define OPCODE_U1	0x37
#define OPCODE_U2	0x17
#define OPCODE_E	0x73

// Useful macro
#define CONCAT(X,Y) ((X) << 3) + (Y)
// func3/func7 codes
#define FUNC3_ADD	0x00
#define FUNC7_ADD	0x00
#define FUNCX_ADD	CONCAT(FUNC7_ADD, FUNC3_ADD)

#define FUNC3_SUB	0x00
#define FUNC7_SUB	0x20
#define FUNCX_SUB	CONCAT(FUNC7_SUB, FUNC3_SUB)

#define FUNC3_XOR	0x40
#define FUNC7_XOR	0x00
#define FUNCX_XOR	CONCAT(FUNC7_XOR, FUNC3_XOR)

#define FUNC3_OR	0x60
#define FUNC7_OR	0x00
#define FUNCX_OR	CONCAT(FUNC7_OR, FUNC3_OR)

#define FUNC3_AND	0x70
#define FUNC7_AND	0x00
#define FUNCX_AND	CONCAT(FUNC7_AND, FUNC3_AND)

#define FUNC3_SLL	0x01
#define FUNC7_SLL	0x00
#define FUNCX_SLL	CONCAT(FUNC7_SLL, FUNC3_SLL)

#define FUNC3_SRL	0x05
#define FUNC7_SRL	0x00
#define FUNCX_SRL	CONCAT(FUNC7_SRL, FUNC3_SRL)

#define FUNC3_SRA	0x05
#define FUNC7_SRA	0x20
#define FUNCX_SRA	CONCAT(FUNC7_SRA, FUNC3_SRA)

#define FUNC3_SLT	0x02
#define FUNC7_SLT	0x00
#define FUNCX_SLT	CONCAT(FUNC7_SLT, FUNC3_SLT)

#define FUNC3_SLTU	0x03
#define FUNC7_SLTU	0x00
#define FUNCX_SLTU	CONCAT(FUNC7_SLTU, FUNC3_SLTU)

#define FUNC3_XB	0x00
#define FUNC3_XH	0x01
#define FUNC3_XW	0x02
#define FUNC3_XBU	0x04
#define FUNC3_XHU	0x05

#define FUNC_BEQ	0x00
#define FUNC_BNE	0x01
#define FUNC_BLT	0x04
#define FUNC_BGE	0x05
#define FUNC_BLTU	0x06
#define FUNC_BGEU	0x07

typedef ap_uint<ARCH>				arch_t;
// Memory address type (uint32_t)
typedef ap_uint<LOG_MEM_SIZE> 		addr_t;
// Register file index (uint5_t)
typedef ap_uint<LOG_REGFILE_SIZE> 	rfi_t;
// Opcode
typedef ap_uint<OPCODE_WIDTH> 		opcode_t;

// Write strobe
#define STRB_WIDTH					4
typedef ap_uint<STRB_WIDTH>			strb_t;

// Simple CPU
void cpu(arch_t*, volatile strb_t*);

#endif  // RISCV32I_H_
