; digitalwrite_callee.asm - digitalWrite function - callee version

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; C function:                               ;
;   void digitalWrite(int pin, int val);    ;
; Registers:                                ;
;     de: pin                               ;
;     hl: val                               ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_user

PUBLIC digitalWrite_callee
EXTERN asm_digitalWrite

digitalWrite_callee:
	pop hl
	pop de
	ex (sp),hl

	jp asm_digitalWrite