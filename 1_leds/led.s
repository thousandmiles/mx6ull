/*
author: forrest long and openedv

project: led lighting

version: 1.0

description: use this file to light the led

arm: I.MX6ULL
*/

.global _start

/*
Program entry, GPIO initialization, and output are low level to control the LED light on.
*/

_start:

/*
1. Enable all clocks
*/

ldr r0, = 0X020C4068         /* register CCGR0 */
ldr r1, = 0XFFFFFFFF
str r1, [r0]

ldr r0, = 0X020C406C         /* register CCGR1 */
str r1, [r0]

ldr r0, = 0X020C4070         /* register CCGR2 */
str r1, [r0]

ldr r0, = 0X020C4074         /* register CCGR3 */
str r1, [r0]

ldr r0, = 0X020C4078         /* register CCGR4 */
str r1, [r0]

ldr r0, = 0X020C407C         /* register CCGR5 */
str r1, [r0]

ldr r0, = 0X020C4080         /* register CCGR6 */
str r1, [r0]

/*
2. Multiplex GPIO: set the IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03 to ALT5(0X0101)
*/

ldr r0, = 0X020E0068         /* SW_MUX_CTL register for GPIO1_IO03 */
ldr r1, = 0X5
str r1, [r0]

/*
3. Configure the GPIO1_IO03: IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03
--bit [16]:0            HYS: disabled
--bit [15:14]:00        PUS: pull down enable
--bit [13]:0            PUE: keeper status
--bit [12]:1            PKE: pull/keeper enable
--bit [11]:0            ODE: open circuit disabled
--bit [7:6]:10          SPEED: moderate
--bit [5:3]:110         DSE: R0/R6
--bit [0]:0             SRE: low rate

bit[16:0]: 0    0   0   0   1   0   0   0   0   1   0   1   1   0   0   0   0
bit        16   15  14  13  12  11  10  9   8   7   6   5   4   3   2   1   0
           0    -------1------  ------0------   ------B------   ------0------
--> 10B0
*/

ldr r0, = 0X020E02F4         /* SW_PAD_CTL register for GPIO1_IO03 */
ldr r1, = 0X10B0
str r1, [r0]

/*
4. Set GPIO1_IO03 as output
*/

ldr r0, = 0X0209C004         /* GPIO direction register, GPIO1_GDIR */
ldr r1, = 0X8
str r1, [r0]

/*
5. Output 0 to GPIO1_IO03
*/

ldr r0, = 0X0209C000         /* GPIO data register, GPIO1_DR */
ldr r1, = 0X0
str r1, [r0]

/*
6. Loop
*/

loop:
    b loop