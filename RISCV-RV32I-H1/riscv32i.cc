// RISCV-32I design in HLS
// https://github.com/jameslzhu/riscv-card/blob/master/riscv-card.pdf

#define HLS_DEBUG 1

#include "riscv32i.h"
#include <stdio.h>
#include <stdint.h>

// Useful macro
#define wstrb (*pstrb)

void cpu(arch_t mem[MEM_SIZE], volatile strb_t* pstrb) {

#pragma HLS INTERFACE ap_none port=pstrb
#pragma HLS RESOURCE variable=mem core=RAM_1P_BRAM

  // Register file
  arch_t reg_file[REGFILE_SIZE];

  // Reset all registers
  for (int i = 0; i < REGFILE_SIZE; i++)
	  reg_file[i] = 0;

  // Program counter (RESET vector)
  arch_t pc = 0;

  // Now execute program until we encounter "finish"
  PROGRAM_LOOP: while (true) {

	//--------------------------------------------------------------------------------------------------------
	// Fetch
	//--------------------------------------------------------------------------------------------------------
	arch_t insn = mem[pc >> 2];
	//wstrb = 0;

	//--------------------------------------------------------------------------------------------------------
	// Decode
	//--------------------------------------------------------------------------------------------------------
	// rd, rs1, rs2
	opcode_t opcode = insn(6,0);
	rfi_t rd = insn(11,7);
	rfi_t rs1 = insn(19,15);
	rfi_t rs2 = insn(24,20);
	func3_t func3 = insn(14,12);
	func7_t func7 = insn(31,25);
	func7 = (opcode == OPCODE_R) ? (func7) : ((func7_t) 0x0);
	funcx_t funcx = (((funcx_t) func7) << 3) | ((funcx_t) func3);	// funcx = {func7, func3}

	// Generate the immediate value
	arch_t imm, immI, immS, immB, immJ, immU;
	// immI
	immI = (((ap_int<ARCH>) insn) >> 20);
	// immS
	immS = ( immI(31,5),insn(11,8), insn(7,7));
	// immB
	immB = ( immS(31,12), insn(7,7), immS(10,1), ((ap_uint<1>) 0) );
	// immJ
	immJ = ( insn(31,31), insn(31,31), insn(31,31), insn(31,31),
		 insn(31,31), insn(31,31), insn(31,31), insn(31,31),
		 insn(31,31), insn(31,31), insn(31,31), insn(31,31),
 		 insn(19,12), insn(20,20), insn(30,21), ((ap_uint<1>) 0) );
	// immU
	immU = ( insn(31,12), ((ap_uint<12>) 0) );

	imm = 0;
	switch (opcode) {
		case OPCODE_IA:
		case OPCODE_IM:
		case OPCODE_IJ:
						imm = immI;
						break;

		case OPCODE_S:
						imm = immS;
						break;

		case OPCODE_B:
						imm = immB;
						break;

		case OPCODE_J:
						imm = immJ;
						break;

		case OPCODE_U1:
		case OPCODE_U2:
						imm = immU;
						break;
	}


	// ALU
	arch_t src1 = reg_file[rs1];
	arch_t src2 = ((opcode != OPCODE_R) & (opcode != OPCODE_S)) ? imm : reg_file[rs2];
	arch_t res = 0;

	// Memory load/store
	arch_t addr = src1 + imm;
	arch_t val = 0;

	// Jump
	arch_t res_j = 0;
	arch_t res_b = pc + imm;
	arch_t res_n = pc + 4;

	// U type
	arch_t imm12 = imm << 12;

	//--------------------------------------------------------------------------------------------------------
    // Execute
	//--------------------------------------------------------------------------------------------------------
    switch (opcode) {
		case OPCODE_E:
						printf("ECALL at PC = %08x\n", (uint32_t) pc);
						return;

		case OPCODE_R:
		case OPCODE_IA:
						switch(funcx) {
							case FUNCX_ADD:
											res = src1 + src2;
											break;
							case FUNCX_SUB:
											res = src1 - src2;
											break;
							case FUNCX_XOR:
											res = src1 ^ src2;
											break;
							case FUNCX_OR:
											res = src1 | src2;
											break;
							case FUNCX_AND:
											res = src1 & src2;
											break;
							case FUNCX_SLL:
											res = src1 << src2;
											break;
							case FUNCX_SRL:
											res = src1 >> src2;
											break;
							case FUNCX_SRA:
											res = ((ap_int<ARCH>) src1) >> src2;
											break;
							case FUNCX_SLT:
											res = (((ap_int<ARCH>) src1) < ((ap_int<ARCH>) src2)) ? 1 : 0;
											break;
							case FUNCX_SLTU:
											res = (((ap_uint<ARCH>) src1) < ((ap_uint<ARCH>) src2)) ? 1 : 0;
											break;
						}
						break;

		case OPCODE_IM:
						if (addr & 0x3) {
							printf("Address fault at PC = %08x\n", (uint32_t) pc);
							return;
						}
						val = mem[addr >> 2];
						switch (func3) {
							case FUNC3_XB:
											res = (ap_int<ARCH>) ((ap_int<8>) ( val(7,0) ));
											break;
							case FUNC3_XH:
											res = (ap_int<ARCH>) ((ap_int<16>) ( val(15,0) ));
											break;
							case FUNC3_XBU:
											res = val(7,0);
											break;
							case FUNC3_XHU:
											res = val(15,0);
											break;
							case FUNC3_XW:
											res = val;
											break;
						}
						break;

		case OPCODE_S:
						switch (func3) {
							case FUNC3_XB:
											wstrb=0x1;
											break;
							case FUNC3_XH:
											wstrb=0x3;
											break;
							case FUNC3_XW:
											wstrb=0xf;
											break;

						}
						res = src2;
						break;

		case OPCODE_B:
						switch (func3) {
							case FUNC_BEQ:
											res_j = (src1 == src2) ? res_b : res_n;
											break;
							case FUNC_BNE:
											res_j = (src1 != src2) ? res_b : res_n;
											break;
							case FUNC_BLT:
											res_j = (((ap_int<ARCH>) src1) < ((ap_int<ARCH>) src2)) ? res_b : res_n;
											break;
							case FUNC_BGE:
											res_j = (((ap_int<ARCH>) src1) >= ((ap_int<ARCH>) src2)) ? res_b : res_n;
											break;
							case FUNC_BLTU:
											res_j = (((ap_uint<ARCH>) src1) < ((ap_uint<ARCH>) src2)) ? res_b : res_n;
											break;
							case FUNC_BGEU:
											res_j = (((ap_uint<ARCH>) src1) >= ((ap_uint<ARCH>) src2)) ? res_b : res_n;
											break;

						}
						break;

		case OPCODE_J:
						res = res_n;
						res_j = res_b;
						break;

		case OPCODE_IJ:
						res = res_n;
						res_j = src1 + imm;
						break;

		case OPCODE_U1:
						res = imm12;
						break;

		case OPCODE_U2:
						res = pc + imm12;
						break;

		default:
						printf("Illegal instruction at PC = %08x\n", (uint32_t) pc);
						return;
    }

    // Write back to reg_file or memory or PC
    if (opcode == OPCODE_S) {
		if (addr(1,0)) {
			printf("Address fault at PC = %08x\n", (uint32_t) pc);
		}
		else {
			mem[addr >> 2] = res;
		}
    }
    else if ((opcode != OPCODE_B) && (rd != 0))
    	reg_file[rd] = res;

    // Branch handling
    if ((opcode == OPCODE_B) || (opcode == OPCODE_J) || (opcode == OPCODE_IJ)){
    	if (pc(1,0)) {
    		printf("Address fault at PC = %08x\n", (uint32_t) pc);
    		return;	// address fault
    	}
    	else
    		pc = res_j;
    }
    else
    	pc = res_n;
  }

#if HLS_DEBUG
  printf("PC = %08x\n", (uint32_t) pc);
  for (int i = 0; i < REGFILE_SIZE; i++)
	  printf("R[%02d] = %08x\n", i, (uint32_t) reg_file[i]);
#endif

}
