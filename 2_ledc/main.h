# ifndef __MAIN_H
# define __MAIN_H

/*
author: forrest long and openedv

project: led lighting

version: 1.0

description: header file for led lighting, GPIO1_03 register definitions

arm: I.MX6ULL
*/

/*
clocking CCM
*/

// (unsigned int *)0X020C4068 is to force address 0X020C4068 to a pointer
// volatile is to avoid optimizing the variable

#define CCM_CCGR0             *((volatile unsigned int *)0X020C4068)
#define CCM_CCGR1             *((volatile unsigned int *)0X020C406C)
#define CCM_CCGR2             *((volatile unsigned int *)0X020C4070)
#define CCM_CCGR3             *((volatile unsigned int *)0X020C4074)
#define CCM_CCGR4             *((volatile unsigned int *)0X020C4078)
#define CCM_CCGR5             *((volatile unsigned int *)0X020C407C)
#define CCM_CCGR6             *((volatile unsigned int *)0X020C4080)

/*
SW_MUX/PAD_GPIO1_IO03 definitions
*/
#define SW_MUX_GPIO01_IO03    *((volatile unsigned int *)0X020E0068)
#define SW_PAD_GPIO01_IO03    *((volatile unsigned int *)0X020E02F4)

/*
GPIO1 registers definitions
*/
#define GPIO1_DR              *((volatile unsigned int *)0X0209C000)
#define GPIO1_GDIR            *((volatile unsigned int *)0X0209C004)
#define GPIO1_PSR             *((volatile unsigned int *)0X0209C008)
#define GPIO1_ICR1            *((volatile unsigned int *)0X0209C00C)
#define GPIO1_ICR2            *((volatile unsigned int *)0X0209C010)
#define GPIO1_IMR             *((volatile unsigned int *)0X0209C014)
#define GPIO1_ISR             *((volatile unsigned int *)0X0209C018)
#define GPIO1_EDGE_SEL        *((volatile unsigned int *)0X0209C01C)

#endif