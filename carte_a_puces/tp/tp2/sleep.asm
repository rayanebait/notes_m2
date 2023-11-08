begin:
	cbi 0x05, 5
	sbi 0x04, 5
	ser r24
	ser r25
	ldi r26, 32
loop:
	dec r24
	cpi r24, 0; r24 = 0 ? break
	brne loop
loop1:
	ser r24 ;r25 || r24 - 1 = 00010001||00000000 - 1 = 00010000||11111111
	dec r25
	cpi r25, 0 ;r25 = 0? break
	brne loop
loop2:
	ser r24
	ser r25 ;r25 || r24 - 1 = 00010001||00000000 - 1 = 00010000||11111111
	dec r26 
	cpi r26, 0 ;r25 = 0? break
	sbic 0x05, 5
	rjmp dark
	rjmp light
continue:
	brne loop
	rjmp end
light:
	sbi 0x05, 5
	rjmp continue
dark:
	cbi 0x05, 5
	rjmp continue
end:
	cbi 0x05, 5

