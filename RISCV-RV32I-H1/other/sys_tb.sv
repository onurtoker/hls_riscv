`timescale 1ns / 1ps

module sys_tb();

localparam T=10;

logic clk, reset, start;
logic done, idle, ready, we;
logic[9:0] addr;
logic [31:0] val_i, val_o;
logic [3:0] wstrb;
logic ce, vld;

cpu U1(
        .ap_clk(clk),
        .ap_rst(reset),
        .ap_start(start),
        .ap_idle(idle),
        .ap_ready(ready),
        .mem_V_address0(addr),
        .mem_V_ce0(ce),
        .mem_V_we0(we),
        .mem_V_d0(val_i),
        .mem_V_q0(val_o),
        .pstrb_V(wstrb)    
);

//mem U3(
//        .clk(clk),   
//        .we(we),   
//        .addr(addr),
//        .din(val_i),
//        .dout(val_o),
//        .wstrb(wstrb)    
//);

SRAM U4 (
  .clka(clk),    // input wire clka
  .wea({4{we}} & wstrb),   // input wire [3 : 0] wea
  .addra(addr),  // input wire [9 : 0] addra
  .dina(val_i),  // input wire [31 : 0] dina
  .douta(val_o)  // output wire [31 : 0] douta
);

initial clk = 0;
always #(T/2) clk = ~clk;

initial
begin
    reset = 1; 
    #(T);
    reset = 0; 
    #(T);
    start = 1;
    #(T);
    start = 0;
    wait(idle==1);
    $display("DONE\n");
    $stop;    
end

endmodule
