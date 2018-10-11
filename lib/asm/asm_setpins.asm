; asm_setpins.asm - asm version of setPins function

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; C function:                               ;
;   void setPins(int val);    				;
; Registers:                                ;
;     hl: val                               ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_user

PUBLIC asm_setPins

asm_setPins:
	ld a, l
	out ($00), a
	out ($08), a
	ret