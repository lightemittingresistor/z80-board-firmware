; setpins.asm - setPins function

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; C function:                               ;
;   void setPins(int val);    				;
; Registers:                                ;
;     hl: val                               ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_user

EXTERN asm_setPins
PUBLIC setPins

setPins:
	pop af
	pop hl

	push hl
	push af

	jp asm_setPins