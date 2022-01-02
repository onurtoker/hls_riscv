// Adapted from
// https://gitlab.cs.washington.edu/cse599s/hls-tutorials/-/tree/master/part3

#include <stdio.h>
#include <stdint.h>

#include "my_cpu.h"

#define VECTOR_LEN 4
#define USE_BRANCH true

// BEGIN Instruction Table
insn_T getFinishInsn() {return 0x00000000;}
insn_T getWriteImmediateInsn(int dst, int imm) {return OPCODE_WR_IMM + (dst << 3) + (imm << 7);}
insn_T getMemoryInsn(int opcode, int reg, int addr) {return opcode + (reg << 3) + (addr << 7);}
insn_T getBranchInsn(int opcode, int src_0, int src_1, int new_pc) {return opcode + (src_0 << 3) + (src_1 << 7) + (new_pc << 11); }
insn_T getBinaryInsn(int opcode, int dst, int src_0, int src_1) {return opcode + (dst << 3) + (src_0 << 7) + (src_1 << 11); }
// END Instruction Table

int main(void) {

  insn_T i_mem[INSN_SIZE];
  data_T d_mem[DATA_SIZE];

  for (int i = 0; i < DATA_SIZE; i++)
	  i_mem[i] = getFinishInsn();

  for (int i = 0; i < INSN_SIZE; i++)
	  d_mem[i] = i; //0xDEADBEEF;

  // Initialize
  int addr_A = 0;
  int addr_B = addr_A + VECTOR_LEN;
  int addr_C = addr_B + VECTOR_LEN;

  // Assembly Program
#if USE_BRANCH == false
  for (int i = 0; i < VECTOR_LEN; i++) {
    i_mem[7*i+0] = getWriteImmediateInsn(R0, addr_A+i);   // Write A element address in R0
    i_mem[7*i+1] = getWriteImmediateInsn(R1, addr_B+i);   // Write B element address in R1
    i_mem[7*i+2] = getWriteImmediateInsn(R2, addr_C+i);   // Write C element address in R2
    i_mem[7*i+3] = getMemoryInsn(OPCODE_LOAD, R3, R0);    // Load A element into R3
    i_mem[7*i+4] = getMemoryInsn(OPCODE_LOAD, R4, R1);    // Load B element into R4
    i_mem[7*i+5] = getBinaryInsn(OPCODE_ADD, R5, R3, R4); // R5 <- R3 + R4
    i_mem[7*i+6] = getMemoryInsn(OPCODE_STORE, R2, R5);   // Store R5 into C
  }
#else
  // Loop Init
  i_mem[0] = getWriteImmediateInsn(R0, 0);            // loop index
  i_mem[1] = getWriteImmediateInsn(R1, 1);            // loop increment
  i_mem[2] = getWriteImmediateInsn(R2, VECTOR_LEN);   // loop bound
  i_mem[3] = getWriteImmediateInsn(R4, addr_A);       // A base address
  i_mem[4] = getWriteImmediateInsn(R5, addr_B);       // B base address
  i_mem[5] = getWriteImmediateInsn(R6, addr_C);       // C base address
  // Loop Body
  i_mem[6] = getMemoryInsn(OPCODE_LOAD, R7, R4);
  i_mem[7] = getMemoryInsn(OPCODE_LOAD, R8, R5);
  i_mem[8] = getBinaryInsn(OPCODE_ADD, R9, R7, R8);
  i_mem[9] = getMemoryInsn(OPCODE_STORE, R6, R9);
  i_mem[10] = getBinaryInsn(OPCODE_ADD, R4, R4, R1);
  i_mem[11] = getBinaryInsn(OPCODE_ADD, R5, R5, R1);
  i_mem[12] = getBinaryInsn(OPCODE_ADD, R6, R6, R1);
  // Loop Index Increment & Condition
  i_mem[13] = getBinaryInsn(OPCODE_ADD, R0, R0, R1); // increment loop index
  i_mem[14] = getBranchInsn(OPCODE_BNE, R0, R2, 6); // branch if not equal
#endif  // USE_BRANCH

  FILE *ifptr = fopen("i_mem.txt", "w");
  FILE *dfptr = fopen("d_mem.txt", "w");

  // Dump instruction memory
  for (int i = 0; i < 7 * VECTOR_LEN + 1; i++) {
	  printf("i_mem[%04x] = %08x\n", i, *((uint32_t*) (i_mem + i)));
	  fprintf(ifptr, "%08x\n", *((uint32_t*) (i_mem + i)));
  }

  // Dump data memory
  for (int i = 0; i < 3 * VECTOR_LEN; i++) {
	  printf("d_mem[%04x] = %08x\n", i, (uint32_t) d_mem[i]);
	  fprintf(dfptr, "%08x\n", (int) d_mem[i]);
  }

  fclose(ifptr);
  fclose(dfptr);

  // Invoke the CPU
  cpu(i_mem, d_mem);

  // Dump data memory
  for (int i = 0; i < 3 * VECTOR_LEN; i++)
	  printf("d_mem[%04x] = %08x\n", i, (int) d_mem[i]);

  // End
  return 0;
}
