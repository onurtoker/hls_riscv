// Instruction memory
// 64 locations, each location 32 bits

module i_mem(clk, we, a, din, dout);
 
input clk;   
input we;   
input  [5:0] a;   
input  [31:0] din;   
output [31:0] dout;   
logic    [31:0] ram [0:63];   
logic    [31:0] read_a; 
 
always @(posedge clk) 
begin   
    if (we)   
        ram[a] <= din;
     
    read_a <= a;   
end   

assign dout = ram[read_a];

// For simulation ONLY
initial 
begin
    $readmemh("C:/Users/onur/Desktop/MyWork/vivado/FPGA_course/HLS/project_cpu/i_mem.txt", ram);
end
   
endmodule 