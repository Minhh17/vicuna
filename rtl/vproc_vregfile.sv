// Copyright TU Wien
// Licensed under the Solderpad Hardware License v2.1, see LICENSE.txt for details
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1


module vproc_vregfile #(
        parameter vproc_pkg::vreg_type                   VREG_TYPE              = vproc_pkg::VREG_GENERIC,
        parameter int unsigned                           VREG_W                 = 512,  // vector register width in bits
        parameter int unsigned                           MAX_PORT_W             = 512,  // max port width in bits
        parameter int unsigned                           MAX_ADDR_W             = 128,  // max addr width in bits
        parameter int unsigned                           PORT_RD_CNT            = 1,    // number of read ports
        parameter int unsigned                           PORT_RD_W[PORT_RD_CNT] = '{512}, // read port widths
        parameter int unsigned                           PORT_WR_CNT            = 1,    // number of write ports
        parameter int unsigned                           PORT_WR_W[PORT_WR_CNT] = '{512}  // write port widths
    )(
        input  logic                                     clk_i,
        input  logic                                     async_rst_ni,
        input  logic                                     sync_rst_ni,

        input  logic [PORT_WR_CNT-1:0][MAX_ADDR_W  -1:0] wr_addr_i,
        input  logic [PORT_WR_CNT-1:0][MAX_PORT_W  -1:0] wr_data_i,
        input  logic [PORT_WR_CNT-1:0][MAX_PORT_W/8-1:0] wr_be_i,
        input  logic [PORT_WR_CNT-1:0]                   wr_we_i,

        input  logic [PORT_RD_CNT-1:0][MAX_ADDR_W  -1:0] rd_addr_i,
        output logic [PORT_RD_CNT-1:0][MAX_PORT_W  -1:0] rd_data_o
    );

    ///////////////////////////////////////////////////////////////////////////
    //
    //                        XOR-BASED MULTI-PORTED RAM
    //
    // For reference, see Laforest et al.'s "Composing Multi-Ported Memories on
    // FPGAs", DOI: http://dx.doi.org/10.1145/2629629, or following summary:
    // https://tomverbeure.github.io/2019/08/03/Multiport-Memories.html
    //
    // Following table shows the read address assignment for each RAM block,
    // with ARx corresponding to the address requested by read port x (RPx) and
    // AWy is the address specified by write port y (WPy).  The table also
    // features internal read ports (IPz) for loopback data required during
    // writes (each write port must read from all other columns to compose the
    // actual XORed write data).
    //
    //  n = PORT_RD_CNT (number of read ports)
    //  m = PORT_WR_CNT (number of write ports)
    //
    //              |      n read ports       | m-1 internal read ports |
    //
    //                RP0   RP1         RPn-1   IP0   IP1         IPm-2
    // -----        +-----+-----+     +-------+-----+-----+     +-------+
    //          WP0 | AR0 | AR1 | ... | ARn-1 | AW1 | AW2 | ... | AWm-1 |
    //              +-----+-----+     +-------+-----+-----+     +-------+
    //   m      WP1 | AR0 | AR1 | ... | ARn-1 | AW0 | AW2 | ... | AWm-1 |
    // write        +-----+-----+     +-------+-----+-----+     +-------+
    // ports          ...   ...          ...    ...   ...          ...
    //              +-----+-----+     +-------+-----+-----+     +-------+
    //          WPm | AR0 | AR1 | ... | ARn-1 | AW0 | AW1 | ... | AWm-2 |
    // -----        +-----+-----+     +-------+-----+-----+     +-------+
    //
    // There is one row for each write port. The read ports have an individual
    // column each, which is used to read data from every row.  Note that the
    // first n columns are for the n read ports, followed by m-1 columns for
    // the m write ports (one column less than write ports) which a write port
    // requires to read back data from all other rows.  We save one internal
    // read port by removing the redundant diagonal where WPy == IPy.
    //

    localparam int unsigned PORT_RD_CNT_TOTAL = PORT_RD_CNT + PORT_WR_CNT - 1;
    logic [31:0][VREG_W-1:0] ram_asic;

    // read address assignment
    logic [PORT_RD_CNT_TOTAL-1:0][PORT_WR_CNT-1:0][MAX_ADDR_W-1:0] rd_addr;
    always_comb begin
        for (int i = 0; i < PORT_RD_CNT; i++) begin
            for (int j = 0; j < PORT_WR_CNT; j++) begin
                rd_addr[i][j] = rd_addr_i[i];
            end
        end
        for (int i = 0; i < PORT_WR_CNT - 1; i++) begin
            for (int j = 0; j < PORT_WR_CNT; j++) begin
                rd_addr[PORT_RD_CNT + i][j] = (i < j) ? wr_addr_i[i] : wr_addr_i[i+1];
            end
        end
    end

    // RAM instantiation
    logic [PORT_RD_CNT_TOTAL-1:0][PORT_WR_CNT-1:0][MAX_PORT_W-1:0] rd_data;
    generate
        for (genvar gw = 0; gw < PORT_WR_CNT; gw++) begin

            // compose write data
            logic [MAX_PORT_W-1:0] wr_data;
            always_comb begin
                wr_data = wr_data_i[gw];
                if (VREG_TYPE != vproc_pkg::VREG_ASIC) begin
                    for (int i = 0; i < PORT_WR_CNT - 1; i++) begin
                        wr_data = wr_data ^ rd_data[PORT_RD_CNT + gw - ((i < gw) ? 1 : 0)][i + ((i < gw) ? 0 : 1)];
                    end
                end
            end

            case (VREG_TYPE)

                vproc_pkg::VREG_GENERIC: begin
                    for (genvar gr = 0; gr < PORT_RD_CNT_TOTAL; gr++) begin
                        logic [VREG_W-1:0] ram[32];
                        always_ff @(posedge clk_i) begin
                            for (int i = 0; i < MAX_PORT_W / 8; i++) begin
                                if (wr_we_i[gw] & wr_be_i[gw][i]) begin
                                    ram[wr_addr_i[gw]][i*8 +: 8] <= wr_data[i*8 +: 8];  // write each byte continous into RAM at addr input
                                end
                            end
                        end
                        assign rd_data[gr][gw] = ram[rd_addr[gr][gw]];
                    end
                end

                vproc_pkg::VREG_XLNX_RAM32M: begin
                    // use Xilinx's 16-bit wide by 32-deep RAM32M16 primitive with
                    // one write port and eight independent read ports
                    for (genvar gr = 0; gr < (PORT_RD_CNT_TOTAL + 7) / 8; gr++) begin
                        for (genvar gm = 0; gm < MAX_PORT_W / 16; gm++) begin
                            logic [MAX_ADDR_W-1:0] rd_addr_0, rd_addr_1, rd_addr_2, rd_addr_3, rd_addr_4, rd_addr_5, rd_addr_6, rd_addr_7;
                            logic [          15:0] rd_data_0, rd_data_1, rd_data_2, rd_data_3, rd_data_4, rd_data_5, rd_data_6, rd_data_7;
                            RAM32M16 xlnx_ram32m16_inst (
                                .DOA    ( rd_data_0                       ),
                                .DOB    ( rd_data_1                       ),
                                .DOC    ( rd_data_2                       ),
                                .DOD    ( rd_data_3                       ),
                                .DOE    ( rd_data_4                       ),
                                .DOF    ( rd_data_5                       ),
                                .DOG    ( rd_data_6                       ),
                                .DOH    ( rd_data_7                       ),
                                .ADDRA  ( rd_addr_0                       ),
                                .ADDRB  ( rd_addr_1                       ),
                                .ADDRC  ( rd_addr_2                       ),
                                .ADDRD  ( rd_addr_3                       ),
                                .ADDRE  ( rd_addr_4                       ),
                                .ADDRF  ( rd_addr_5                       ),
                                .ADDRG  ( rd_addr_6                       ),
                                .ADDRH  ( rd_addr_7                       ),
                                .DIA    ( wr_data[16*gm +: 16]            ),
                                .DIB    ( wr_data[16*gm +: 16]            ),
                                .DIC    ( wr_data[16*gm +: 16]            ),
                                .DID    ( wr_data[16*gm +: 16]            ),
                                .DIE    ( wr_data[16*gm +: 16]            ),
                                .DIF    ( wr_data[16*gm +: 16]            ),
                                .DIG    ( wr_data[16*gm +: 16]            ),
                                .DIH    ( wr_data[16*gm +: 16]            ),
                                .WCLK   ( clk_i                           ),
                                .WE     ( wr_we_i[gw] & wr_be_i[gw][gm/2] )
                            );
                            assign rd_addr_0                      = rd_addr[gr*8][gw];
                            assign rd_data[gr*8][gw][16*gm +: 16] = rd_data_0;
                            if (gr*8+1 < PORT_RD_CNT_TOTAL) begin
                                assign rd_addr_1                        = rd_addr[gr*8+1][gw];
                                assign rd_data[gr*8+1][gw][16*gm +: 16] = rd_data_1;
                            end else begin
                                assign rd_addr_1                        = '0;
                            end
                            if (gr*8+2 < PORT_RD_CNT_TOTAL) begin
                                assign rd_addr_2                        = rd_addr[gr*8+2][gw];
                                assign rd_data[gr*8+2][gw][16*gm +: 16] = rd_data_2;
                            end else begin
                                assign rd_addr_2                        = '0;
                            end
                            if (gr*8+3 < PORT_RD_CNT_TOTAL) begin
                                assign rd_addr_3                        = rd_addr[gr*8+3][gw];
                                assign rd_data[gr*8+3][gw][16*gm +: 16] = rd_data_3;
                            end else begin
                                assign rd_addr_3                        = '0;
                            end
                            if (gr*8+4 < PORT_RD_CNT_TOTAL) begin
                                assign rd_addr_4                        = rd_addr[gr*8+4][gw];
                                assign rd_data[gr*8+4][gw][16*gm +: 16] = rd_data_4;
                            end else begin
                                assign rd_addr_4                        = '0;
                            end
                            if (gr*8+5 < PORT_RD_CNT_TOTAL) begin
                                assign rd_addr_5                        = rd_addr[gr*8+5][gw];
                                assign rd_data[gr*8+5][gw][16*gm +: 16] = rd_data_5;
                            end else begin
                                assign rd_addr_5                        = '0;
                            end
                            if (gr*8+6 < PORT_RD_CNT_TOTAL) begin
                                assign rd_addr_6                        = rd_addr[gr*8+6][gw];
                                assign rd_data[gr*8+6][gw][16*gm +: 16] = rd_data_6;
                            end else begin
                                assign rd_addr_6                        = '0;
                            end
                            if (gr*8+7 < PORT_RD_CNT_TOTAL) begin
                                assign rd_addr_7                        = rd_addr[gr*8+7][gw];
                                assign rd_data[gr*8+7][gw][16*gm +: 16] = rd_data_7;
                            end else begin
                                assign rd_addr_7                        = '0;
                            end
                        end
                    end
                end


                vproc_pkg::VREG_ASIC: begin
                    for (genvar gr = 0; gr < PORT_RD_CNT_TOTAL; gr++) begin
                        always_ff @(posedge clk_i) begin
                            for (int i = 0; i < MAX_PORT_W / 8; i++) begin
                                if (wr_we_i[gw] & wr_be_i[gw][i]) begin
                                    ram_asic[wr_addr_i[gw]][i*8 +: 8] <= wr_data[i*8 +: 8];
                                end
                            end
                        end
                        assign rd_data[gr][gw] = ram_asic[rd_addr[gr][gw]];
                    end
                end

                default: ;

            endcase
        end
    endgenerate

    // compose read data
    always_comb begin
        for (int i = 0; i < PORT_RD_CNT; i++) begin
            rd_data_o[i] = rd_data[i][0];
            for (int j = 1; j < PORT_WR_CNT; j++) begin
                if (VREG_TYPE != vproc_pkg::VREG_ASIC) begin
                    rd_data_o[i] = rd_data_o[i] ^ rd_data[i][j];
                end else begin
                    rd_data_o[i] = ram_asic[rd_addr_i[i]];
                end
            end
        end
    end

endmodule
