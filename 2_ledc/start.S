/*
author: forrest long and openedv

project: led lighting

version: 1.0

description: use this file to light the led, basing on C.

arm: I.MX6ULL
*/

.global _start

_start:

/*
using SVC mode
*/
mrs r0, cpsr                /* read cpsr to r0 */

bic r0, r0, #0x1f           /* clean r0[4:0] */

orr r0, r0, #0x13           /* set r0[4:0] to  [1 0011]*/

msr cpsr, r0                /* write r0 to cpsr */


ldr sp, = 0X80200000        /* set stack pointer */

b main                      /* jump to main function */