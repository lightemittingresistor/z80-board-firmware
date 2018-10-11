; asm_digitalwrite.asm - asm version of digitalWrite function
; Currenly broken!

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; C function:                               ;
;   void digitalWrite(int pin, int val);    ;
; Registers:                                ;
;     de: pin                               ;
;     hl: val                               ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION data_user
_asm_digitalwrite_state: defb $00

SECTION code_user

PUBLIC asm_digitalWrite

asm_digitalWrite:
	LD b, 1
_asm_digitalWrite_loop1:
	; decrement e
	DEC e
	; if e is now zero, jump out of loop
	JP Z, _asm_digitalWrite_loopdone
	; clear carry
	OR a
	; shift a left to get mask
	RL b
	; repeat
	jp _asm_digitalWrite_loop1
_asm_digitalWrite_loopdone:
	; now b contains a mask
	ld a, b
	; invert the mask
	XOR $ff
	ld d, a
	; get stored value into a
	PUSH hl
	LD hl, _asm_digitalwrite_state
	LD a, (hl)
	POP hl
	; a <- a AND d
	AND d
	; at this point, a contains the previous value with the bit in question masked
	LD d, a
	; if h is zero, we're done. Otherwise, set the bit
	LD a, l
	OR a
	JP z, _asm_digitalWrite_setzero
	; need to OR in the mask since h was non-zero
	LD a, d
	OR b
	JP _asm_digitalWrite_end
_asm_digitalWrite_setzero:
	ld a, d
_asm_digitalWrite_end:
	OUT ($0000), a
	ret