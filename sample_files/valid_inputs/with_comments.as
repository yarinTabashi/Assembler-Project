.entry LIST
.extern W

MAIN:	add 	r3, LIST
LOOP:	prn 	#48
	lea 	STR, r6
	inc r6
.entry MAIN
	sub 	LOOP[r10] ,r14
END: 	stop
STR: 	.string "abcd"
LIST: 	.data 	 6, -9
	.data 	-100
.entry K
K: 	.data 	31
.extern val1

; This is a comment
macro m1
	add r1, r2
	sub r2, r3
endm

m1