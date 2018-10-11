#ifndef Z80BOARD_H
#define Z80BOARD_H

extern void __LIB__ digitalWrite(int, int) __smallc;
extern void __LIB__ digitalWrite_callee(int, int) __smallc __z88dk_callee;
#define digitalWrite(a,b) digitalWrite_callee(a,b)

extern void __LIB__ setPins(int val) __smallc;
extern void __LIB__ setPins_callee(int val) __smallc __z88dk_callee;
#define setPins(a) setPins_callee(a)

extern int __LIB__ getPins() __smallc;

#endif
