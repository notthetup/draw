#include "utils.h"

/* This busywait loop is roughly accurate when running at 24 MHz. */
void udelay_busy(uint32_t usecs){
    while (usecs --> 0) {
        /* This inner loop is 3 instructions, one of which is a branch.
         * This gives us 4 cycles total.
         * We want to sleep for 1 usec, and there are cycles per usec at 24 MHz.
         * Therefore, loop 6 times, as 6*4=24.
         */
        __asm("mov   r1, #6");
        __asm("retry:");
        __asm("sub r1, #1");
        __asm("bne retry");
        __asm("nop");
    }
}
