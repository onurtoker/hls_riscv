// System memory
// 4KB RAM (1024 dword)

module mem(clk, we, addr, din, dout, wstrb);
 
input clk;   
input we;   
input  [9:0] addr;   
input  [31:0] din;   
output [31:0] dout;   
input [3:0] wstrb; 
logic    [31:0] ram [0:1023];   
logic    [9:0] read_addr; 
 
always @(posedge clk) 
begin
    if (we) begin
        if (wstrb[0]) ram[addr][ 7: 0] <= din[ 7: 0];
        if (wstrb[1]) ram[addr][15: 8] <= din[15: 8];
        if (wstrb[2]) ram[addr][23:16] <= din[23:16];
        if (wstrb[3]) ram[addr][31:24] <= din[31:24];
        /* add memory-mapped IO here */
        // Simple output device, print the LSB to console 
        if (addr == 255)
            $write("%x,", din[7:0]);    
    end

    read_addr <= addr;
end
assign dout = ram[read_addr];

//For simulation ONLY
initial 
begin
    $readmemh("C:/Users/onur/Desktop/MyWork/vivado/RISCV32I_HLS/mem.txt", ram);
end

//initial begin
//    ram[0] = 32'h 3fc00093; //       li      x1,1020
//    ram[1] = 32'h 0000a023; //       sw      x0,0(x1)
//    ram[2] = 32'h 0000a103; // loop: lw      x2,0(x1)
//    ram[3] = 32'h 00110113; //       addi    x2,x2,1
//    ram[4] = 32'h 0020a023; //       sw      x2,0(x1)
//    ram[5] = 32'h ff5ff06f; //       j       <loop>
//end
   
endmodule 
