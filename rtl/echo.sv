module echo (
    input  logic         clk,         // Clock  for sync
    input  logic         rst,         // Reset active high 
    input  logic [15:0][511:0] echo_data_in,  // Bus get data from vproc_core (v0–v15)
    output logic [15:0][511:0] echo_data_out  // Bus return data to vproc_core (write t v16–v31)
);

    always_comb begin
        echo_data_out = echo_data_in;        
    end
endmodule

