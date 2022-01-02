`timescale 1ns / 1ps

module my_cpu_tb();

localparam T=10;

logic clk, reset, start;
logic done, idle, ready, d_we, my_dst_vld, my_imm_vld;
logic[5:0] i_addr, d_addr;
logic [31:0] i_val, d_val_i, d_val_o, my_dst, my_imm;

cpu U1(
        .ap_clk(clk),
        .ap_rst(reset),
        .ap_start(start),
        .ap_done(done),
        .ap_idle(idle),
        .ap_ready(ready),
        .i_mem_V_address0(i_addr),
        .i_mem_V_q0(i_val),
        .d_mem_V_address0(d_addr),
        .d_mem_V_we0(d_we),
        .d_mem_V_d0(d_val_i),
        .d_mem_V_q0(d_val_o)
);

i_mem U2(
        .clk(clk),   
        .we(1'b0),   
        .a(i_addr),   
        .dout(i_val)    
);

d_mem U3(
        .clk(clk),   
        .we(d_we),   
        .a(d_addr),
        .din(d_val_i),
        .dout(d_val_o)    
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
