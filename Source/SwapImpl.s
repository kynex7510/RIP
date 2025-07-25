// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

.global rip_memcpy
.global rip_swapBytesImpl

.section .text

// R0 = dst, R1 = src, R2 = size.
rip_memcpy:
    pld [r1, #32]
    cmp r2, #32
    blo _rip_memcpy_words

    // Copy 32 bytes of data.
    ldr r3, [r1], #4
    str r3, [r0], #4
    ldr r3, [r1], #4
    str r3, [r0], #4
    ldr r3, [r1], #4
    str r3, [r0], #4
    ldr r3, [r1], #4
    str r3, [r0], #4
    ldr r3, [r1], #4
    str r3, [r0], #4
    ldr r3, [r1], #4
    str r3, [r0], #4
    ldr r3, [r1], #4
    str r3, [r0], #4
    ldr r3, [r1], #4
    str r3, [r0], #4
    sub r2, #32
    b rip_memcpy

    _rip_memcpy_words:
    cmp r2, #4
    blo _rip_memcpy_bytes

    ldr r3, [r1], #4
    str r3, [r0], #4
    sub r2, #4
    bhs _rip_memcpy_words

    _rip_memcpy_bytes:
    cmp r2, #0
    bxeq lr

    ldrb r3, [r1], #1
    strb r3, [r0], #1
    sub r2, #1
    b _rip_memcpy_bytes

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
    blne rip_memcpy
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
    ldrh r3, [r1], #3
    pld [r1, #32]
    strb r3, [r0, #2]
    lsr r3, #8
    strb r3, [r0, #1]
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
