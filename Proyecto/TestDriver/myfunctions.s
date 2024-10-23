		AREA |.text|, CODE, READONLY, ALIGN=2
		THUMB
		REQUIRE8
		PRESERVE8
			
		EXPORT display
		EXPORT rmodify 
		EXPORT re0 
 
re0
	push{lr}
	pop{pc}
 
display 
	push {lr} 
	CMP r0 , #1
	beq cr1
	CMP r0 , #2 
	beq cr2
	CMP r0 , #3 
	beq cr3
	CMP r0 , #4 
	beq cr4
	CMP r0 , #5 
	beq cr5
	CMP r0 , #6 
	beq cr6
	CMP r0 , #7 
	beq cr7
	CMP r0 , #8 
	beq cr8
	CMP r0 , #9 
	beq cr9
	CMP r0 , #10 
	beq cr10
	CMP r0 , #11 
	beq cr11
	CMP r0 , #12
	beq cr12
	CMP r0 , #13
	beq cr13
	CMP r0 , #14 
	beq cr14
	CMP r0 , #15 
	beq cr15 
cr1
	MOV r0, r1
	b fin
cr2
	MOV r0, r2
	b fin
cr3
	MOV r0, r3
	b fin
cr4
	MOV r0, r4
	b fin
cr5
	MOV r0, r5
	b fin
cr6
	MOV r0, r6
	b fin
cr7
	MOV r0, r7
	b fin
cr8
	MOV r0, r8
	b fin
cr9
	MOV r0, r9
	b fin
cr10
	MOV r0, r10
	b fin
cr11
	MOV r0, r11
	b fin
cr12
	MOV r0, r12
	b fin
cr13
	MOV r0, sp
	b fin
cr14
	MOV r0, lr
	b fin
cr15
	MOV r0, pc
	b fin	

rmodify
	push {lr}
	CMP r0 , #0
	beq rr0 
	CMP r0 , #2 
	beq rr2
	CMP r0 , #3 
	beq rr3
	CMP r0 , #4 
	beq rr4
	CMP r0 , #5 
	beq rr5
	CMP r0 , #6 
	beq rr6
	CMP r0 , #7 
	beq rr7
	CMP r0 , #8 
	beq rr8
	CMP r0 , #9 
	beq rr9
	CMP r0 , #10 
	beq rr10
	CMP r0 , #11 
	beq rr11
	CMP r0 , #12
	beq rr12 
rr0 
	MOV r0,r1
	b fin1 
rr2
	MOV r2,r1
	b fin1
rr3
	MOV r3, r1
	b fin1
rr4
	MOV r4, r1
	b fin1
rr5
	MOV r5, r1
	b fin1
rr6
	MOV r6, r1
	b fin1
rr7
	MOV r7,r1
	b fin1
rr8
	MOV r8, r1
	b fin1
rr9
	MOV r9, r1
	b fin1
rr10
	MOV r10, r1
	b fin1
rr11
	MOV r11, r1
	b fin1
rr12
	MOV r12, r1
	b fin1
 
fin
	pop {pc}
	
fin1
	pop {pc}
	end

	
 	