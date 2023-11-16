
.global _start  			

_start:
	ldr pc, =Reset_Handler		
	ldr pc, =Undefined_Handler	
	ldr pc, =SVC_Handler		
	ldr pc, =PrefAbort_Handler	
	ldr pc, =DataAbort_Handler	
	ldr	pc, =NotUsed_Handler	
	ldr pc, =IRQ_Handler		
	ldr pc, =FIQ_Handler		


Reset_Handler:

	cpsid i				/* disable IRQ */		

	mrc     p15, 0, r0, c1, c0, 0     /* read C1 of CP15 */
    bic     r0,  r0, #(0x1 << 12)     /* disable I Cache */
    bic     r0,  r0, #(0x1 <<  2)     /* disable D Cache */
    bic     r0,  r0, #0x2             
    bic     r0,  r0, #(0x1 << 11)     
    bic     r0,  r0, #0x1           
    mcr     p15, 0, r0, c1, c0, 0     /* write C1 */
    
	/* IRQ */
	mrs r0, cpsr		/* read cpsr to ro */
	bic r0, r0, #0x1f 	/* clear cpsr[M0:M4] 	*/
	orr r0, r0, #0x12 	/* 10010				*/
	msr cpsr, r0		/* write ro to cpsr		*/
	ldr sp, =0x80600000	

	/* SYS */
	mrs r0, cpsr
	bic r0, r0, #0x1f 	
	orr r0, r0, #0x1f 	
	msr cpsr, r0		
	ldr sp, =0x80400000	

	/* SVC */
	mrs r0, cpsr
	bic r0, r0, #0x1f 	
	orr r0, r0, #0x13 	
	msr cpsr, r0		
	ldr sp, =0X80200000	

	cpsie i				/* enable IRQ */


	b main				/* jump to main function*/


Undefined_Handler:
	ldr r0, =Undefined_Handler
	bx r0


SVC_Handler:
	ldr r0, =SVC_Handler
	bx r0


PrefAbort_Handler:
	ldr r0, =PrefAbort_Handler	
	bx r0


DataAbort_Handler:
	ldr r0, =DataAbort_Handler
	bx r0


NotUsed_Handler:

	ldr r0, =NotUsed_Handler
	bx r0


IRQ_Handler:

/*----step1: preserve the condition before interupt */

	push {lr}					/* store lr (link refister) address */
	push {r0-r3, r12}			/* store r0-r3，r12 */

	mrs r0, spsr				/* read spsr */
	push {r0}					/* store spsr */

/* step1 end */

/* step2: get current interupt ID */

	mrc p15, 4, r1, c15, c0, 0  /* read GIC base address and save it to r1 */							
	add r1, r1, #0X2000			/* GIC base address add 0x2000, then get the CPU interface base address */
	ldr r0, [r1, #0XC]			/* obtain the GICC_IAR register, where an interupt ID is restored. The ID will be loaded in r0.*/
	push {r0, r1}			

/* step2 end */
	
	cps #0x13					/* enable SVC, and allow other interupts can be used */
	push {lr}		

/* step3: execute interupt function */		

	ldr r2, =system_irqhandler	/* load .c interupt function to r2*/
	blx r2						/* run system_irqhandler() */

/* step3 end */


	pop {lr}					
	cps #0x12					/* goes into IRQ */
	pop {r0, r1}				
	str r0, [r1, #0X10]			/* interupt finished, and we need to wirte interupt ID to GICC_EOIR register */

/* step4: restore condition */

	pop {r0}						
	msr spsr_cxsf, r0			/* restore spsr */

	pop {r0-r3, r12}			
	pop {lr}					
	subs pc, lr, #4				/* lr-4, then back to the next command before interupt */

/* step4 end */
	
FIQ_Handler:

	ldr r0, =FIQ_Handler	
	bx r0									

