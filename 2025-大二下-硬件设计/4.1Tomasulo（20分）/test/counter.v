// src/counter.v
module counter #(
    parameter WIDTH = 8
)(
    input wire clk,
    input wire rst_n,
    output reg [WIDTH-1:0] count
);

always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        count <= {WIDTH{1'b0}};
    end else begin
        count <= count + 1'b1;
    end
end

endmodule