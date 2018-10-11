; getpins.asm - getPins function

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; C function:                               ;
;   int getPins();    				        ;
; Registers:                                ;
;     hl: out                               ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_user

EXTERN asm_getPins
PUBLIC getPins

getPins:
	jp asm_getPins