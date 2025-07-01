.global rip_swapBytesImpl

.section .text

// R0 = dst, R1 = src, R2 = size, R3 = bytesPerPixel.
// We assume we have at least one pixel.
.type rip_swapBytesImpl, %function
rip_swapBytesImpl:
    cmp r3, #4
    beq _rip_swapBytes4
    cmp r3, #3
    beq _rip_swapBytes3
    cmp r3, #2
    beq _rip_swapBytes2
    cmp r0, r1
    blne memcpy
    bx lr

    _rip_swapBytes4:
    ldr r3, [r1], #4
    pld [r1, #32]
    rev r3, r3
    str r3, [r0], #4
    sub r2, #4
    cmp r2, #4
    bxlo lr // Return if data left < 4.
    b _rip_swapBytes4

    _rip_swapBytes3:
    push {r4, lr}

    _rip_swapBytes3_loop:
    ldrb r4, [r1, #2]
    ldrb r3, [r1], #3
    pld [r1, #32]
    strb r3, [r0, #2]
    strb r4, [r0], #3
    sub r2, #3
    cmp r2, #3
    poplo {r4, pc} // Return if data left < 3.
    b _rip_swapBytes3_loop

    _rip_swapBytes2:
    cmp r2, #4
    blo _rip_swapBytes2_halfword

    // Optimized version that handles 2 pixels at time.
    _rip_swapBytes2_word:
    ldr r3, [r1], #4
    pld [r1, #32]
    rev16 r3, r3
    str r3, [r0], #4
    sub r2, #4
    cmp r2, #4
    bhs _rip_swapBytes2_word
    cmp r2, #0
    bxeq lr // Return if no data left.

    _rip_swapBytes2_halfword:
    ldrh r3, [r1]
    revsh r3, r3
    strh r3, [r0]
    bx lr
