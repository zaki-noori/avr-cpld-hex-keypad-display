module MYCLPD (
    input CLK, MOSI, EN,
    output wire C1, C2, C3, C4,  
    output wire A, B, C, D, E, F, G, 
    input wire R1, R2, R3, R4,  
    output reg [7:0] Q,  
    output wire MUXOUT 
);

    // -------- shift register --------
    always @(posedge CLK) 
    begin
        Q[0] <= MOSI;
        Q[1] <= Q[0];
        Q[2] <= Q[1];
        Q[3] <= Q[2];
        Q[4] <= Q[3];
        Q[5] <= Q[4];
        Q[6] <= Q[5];
        Q[7] <= Q[6];
    end

    //------- 7-segment led decoder --------
  
    assign A = ( (~Q[3] & ~Q[2] & ~Q[1] & Q[0]) | (Q[2] & ~Q[1] & ~Q[0]) | (Q[3] & Q[1]) | (Q[3] & Q[2]) ) & EN;
    assign B = ( (Q[2] & ~Q[1] & Q[0]) | (Q[2] & Q[1] & ~Q[0]) | (Q[3] & Q[1]) | (Q[3] & Q[2]) ) & EN;
    assign C = ( (~Q[2] & Q[1] & ~Q[0]) | (Q[3] & Q[1]) | (Q[3] & Q[2]) ) & EN;
    assign D = ( (~Q[3] & ~Q[2] & ~Q[1] & Q[0]) | (Q[2] & ~Q[1] & ~Q[0]) | (Q[2] & Q[1] & Q[0]) | (Q[3] & Q[1]) | (Q[3] & Q[2]) ) & EN;
    assign E = ( (Q[0]) | (Q[2] & ~Q[1]) | (Q[3] & Q[1]) ) & EN;
    assign F = ( (~Q[3] & ~Q[2] & Q[0]) | (~Q[2] & Q[1]) | (Q[1] & Q[0]) | (Q[3] & Q[2]) ) & EN;
    assign G = ( (~Q[3] & ~Q[2] & ~Q[1]) | (Q[2] & Q[1] & Q[0]) | (Q[3] & Q[1]) | (Q[3] & Q[2]) ) & EN;

    //------ walking zero decoder ---------    
   
    assign C1 = ~(Q[5]&Q[4]);
    assign C2 = ~(Q[5]&~Q[4]);
    assign C3 = ~(~Q[5]&Q[4]);
    assign C4 = ~(~Q[5]&~Q[4]);

    //------- multiplexer------------

    assign MUXOUT = (~Q[7]&~Q[6]& R1) | (~Q[7]&Q[6]& R2) | (Q[7]&~Q[6]& R3) | (Q[7]&Q[6]& R4);

endmodule