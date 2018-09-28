; example.s - z80 board example

define(`pio_a_d', 0x0)
define(`pio_b_d', 0x01)
define(`pio_a_c', 0x02)
define(`pio_b_c', 0x03)

    ORG $0
init:
    ;initialise stack pointer
    ld sp, 0xffff
    ; zero out memory
    ld hl, 0x8000
    jp start
start:
    ; set PIO to control
    ld a, 0xcf
    out (pio_a_c), a ; send 0xcf to A (control mode)
    ld a, 0x00
    out (pio_a_c), a ; set A to output (0)
    ld a, 0xcf
    out (pio_b_c), a ; send 0xcf to B (control mode)
    ld a, 0xff
    out (pio_b_c), a ; set B to input (1)

    ; disable interupts
    ld a, 0x07
    out (pio_a_c), a
    out (pio_b_c), a 

    ; turn LEDs on
    ld a, 0xff
    out (pio_a_d), a ; set outputs to on

loop:
    in a, (pio_b_d)
    out (pio_a_d), a
    jp loop

