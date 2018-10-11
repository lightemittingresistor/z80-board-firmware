; digitalwrite.asm - digitalWrite function

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; C function:                               ;
;   void digitalWrite(int pin, int val);    ;
; Registers:                                ;
;     de: pin                               ;
;     hl: val                               ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_user

PUBLIC digitalWrite
EXTERN asm_digitalWrite

digitalWrite:
	pop af
	pop de
	pop hl

	push hl
	push de
	push af

	jp asm_digitalWrite