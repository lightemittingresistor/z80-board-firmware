; example.s - z80 board example

pio_a_d:    equ 0x00
pio_b_d:    equ 0x01
pio_a_c:    equ 0x02
pio_b_c:    equ 0x03

    org 0
init:
    ;initialise stack pointer
    ld sp, 0xffff
    ; zero out memory
    ld hl, 0x8000
    jp start
zero:
    ld (hl), 0x00
    inc hl
    jp c, start
    jp zero
start:
    ; set PIO to control
    ld a, 0xcf
    out pio_a_c, a ; send 0xcf to A (control mode)
    ld a, 0x00
    out pio_a_c, a ; set A to output (0)
    ld a, 0xcf
    out pio_b_c, a ; send 0xcf to B (control mode)
    ld a, 0xff
    out pio_b_c, a ; set B to input (1)

    ; disable interupts
    ld a, 0x07
    out pio_a_c, a
    out pio_b_c, a 

    ; turn LEDs on
    ld a, 0xff
    out pio_a_d, a ; set outputs to on

loop:
    in a, pio_b_d
    out pio_a_d, a
    jp start