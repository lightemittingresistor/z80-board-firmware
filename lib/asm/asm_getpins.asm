; asm_getpins.asm - asm version of getPins function

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; C function:                               ;
;   in getPins();            				;
; Registers:                                ;
;     hl: return                            ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_user

PUBLIC asm_getPins

asm_getPins:
	in a, ($01)
	ld l, a
	ret