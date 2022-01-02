// Adapted from
// https://gitlab.cs.washington.edu/cse599s/hls-tutorials/-/tree/master/part3

#include "my_cpu.h"

// Instruction memory address
typedef ap_uint<LOG_INSN_SIZE> insn_idx_T;
// Data memory address
typedef ap_uint<LOG_DATA_SIZE> data_idx_T;
// Register file index
typedef ap_uint<REGFILE_SIZE> rf_idx_T;

// Opcode type
typedef ap_uint<OPCODE_W> opcode_T;
// Immediate type
typedef ap_int<IMM_W> imm_T;

void cpu(insn_T i_mem[INSN_SIZE], data_T d_mem[DATA_SIZE]) {

  // Register file
  data_T reg_file[REGFILE_SIZE];

  // Program counter, new_pc, opcode
  insn_idx_T pc = 0;
  insn_idx_T new_pc = 0;
  opcode_T opcode = 0;

  // Finish flag (indicates program completion)
  bool finish = false;

  OUTER_LOOP: while (!finish) {

	  // Now execute program until we encounter a FINISH or BRANCH opcode
	  PROGRAM_LOOP: while (!finish) {
#pragma HLS PIPELINE II=3
		// Fetch

		insn_T insn = i_mem[pc];

		pc += 1;

		// Decode instruction fields
		opcode_T opcode = insn & 0x7; //insn.generic.opcode;
		rf_idx_T dst = 0;
		rf_idx_T src_0 = 0;
		rf_idx_T src_1 = 0;
		new_pc = 0;
		imm_T imm = 0;
		if (opcode == OPCODE_FINISH) {
		  finish = true;
		} else if (opcode == OPCODE_WR_IMM) {
		  dst = (insn >> 3) & 0xF; 		//insn.immediate.dst;
		  imm = (insn >> 7) & 0xFFFF; 	//insn.immediate.imm;
		} else if (opcode == OPCODE_LOAD) {
		  dst = (insn >> 3) & 0xF; 		//insn.memory.reg;
		  src_0 = (insn >> 7) & 0xF; 	//insn.memory.addr;
		} else if (opcode == OPCODE_STORE) {
		  src_0 = (insn >> 3) & 0xF; 	//insn.memory.addr;
		  src_1 = (insn >> 7) & 0xF; 	//insn.memory.reg;
		} else if (opcode == OPCODE_BEQ || opcode == OPCODE_BNE) {
			src_0 = (insn >> 3) & 0xF;	//insn.branch.src_0;
			src_1 = (insn >> 7) & 0xF; 	//insn.branch.src_1;
			new_pc = (insn >> 11) & 0x3FF; //insn.branch.new_pc;
		} else if (opcode == OPCODE_ADD) {
		  dst = (insn >> 3) & 0xF; 		//insn.binary.dst;
		  src_0 = (insn >> 7) & 0xF; 	//insn.binary.src_0;
		  src_1 = (insn >> 11) & 0xF;	//insn.binary.src_1;
		} else {
		  finish = true;
		}

		// Execute
		data_T arg_0 = reg_file[src_0];
		data_T arg_1 = reg_file[src_1];
		data_T res = 0;
		if (opcode == OPCODE_WR_IMM) {
		  res = (data_T) imm;
		} else if (opcode == OPCODE_ADD) {
		  res = (data_T) arg_0 + arg_1;
		}

		// Branch handling code (If branch is taken, exit the pipeline)
	    if ((opcode == OPCODE_BEQ) && (arg_0 == arg_1))
	    	//pc = new_pc;
	    	finish = true;
	    if ((opcode == OPCODE_BNE) && (arg_0 != arg_1))
	    	//pc = new_pc;
	    	finish = true;

		// Memory load/store
		if (opcode == OPCODE_LOAD) {
		  res = d_mem[arg_0];
		} else if (opcode == OPCODE_STORE) {
		  d_mem[arg_0] = arg_1;
		}

		// Write Back
		if (opcode == OPCODE_WR_IMM  || opcode == OPCODE_LOAD || opcode == OPCODE_ADD) {
		  reg_file[dst] = res;
		}

	  } // PROGRAM_LOOP

	  if (opcode == OPCODE_FINISH)
		  break;
	  else
		  pc = new_pc;

  } // OUTER_LOOP


}
