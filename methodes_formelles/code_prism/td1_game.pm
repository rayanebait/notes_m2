dtmc
module M1
	x : [0..3] init 0;

	[] x=1 -> 0.1111:(x'=2)+0.4166:(x'=3)+0.4723:(x'=0);
	[] x=2 -> 0.1111:(x'=1)+0.4166:(x'=0)+0.4723:(x'=3);
	[] x=0 -> (x'=0);
	[] x=3 -> (x'=3);

endmodule