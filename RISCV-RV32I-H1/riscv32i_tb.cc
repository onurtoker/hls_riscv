// RISCV-32I design in HLS
// https://github.com/jameslzhu/riscv-card/blob/master/riscv-card.pdf

#include "riscv32i.h"
#include <stdio.h>
#include <stdint.h>

int main(void) {
	arch_t mem[MEM_SIZE];
	ap_uint<4> wstrb;
	strb_t* pstrb = &wstrb;

	// Initialize the memory
	FILE* fptr = fopen("../../../../mem.txt", "r");
	int i = 0;
	while(fscanf(fptr,"%x", mem[i++]) != EOF);
	for (; i < MEM_SIZE; mem[i++] = 0);
	fclose(fptr);

	// Invoke the CPU
	cpu(mem, pstrb);

	// Memory dump
	fptr = fopen("../../../../dump.txt", "w");
	for (int i = 0; i < MEM_SIZE; i++)
	  fprintf(fptr, "mem[%04x] = %08x\n", i, (uint32_t) mem[i]);
	fclose(fptr);

	// End
	return 0;

}
