; setpins_callee.asm - callee version of setPins function

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; C function:                               ;
;   void setPins(int val);    				;
; Registers:                                ;
;     hl: val                               ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_user

EXTERN asm_setPins
PUBLIC setPins_callee

setPins_callee:
	pop hl
	ex (sp),hl

	jp asm_setPins