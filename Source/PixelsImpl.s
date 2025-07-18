// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

.global rip_memcpy
.global rip_swapPixelBytesImpl
.global rip_RGB8ToRGBA8
.global rip_RGB565ToRGBA8
.global rip_RGB5A1ToRGBA8
.global rip_RGBA4ToRGBA8

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
.type rip_swapPixelBytesImpl, %function
rip_swapPixelBytesImpl:
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

// R0 = dst, R1 = src, R2 = numPixels.
.type rip_RGB8ToRGBA8, %function
rip_RGB8ToRGBA8:
    mov r3, #3
    mul r3, r2
    add r1, r3
    lsl r3, r2, #2
    add r0, r3
    cmp r2, #4
    blo _rip_RGB8ToRGBA8_copySingle

    push {r4, r5, r6}

    // RGB RGB RGB RGB = RGBR GBRG BRGB = 0xRRBBGGRR 0xGGRRBBGG 0xBBGGRRBB
    _rip_RGB8ToRGBA8_copyMulti:
    ldmdb r1!, {r3, r4, r5}
    pld [r1, #-32]

    // First pixel.
    mov r6, r5, lsr #8
    orr r6, r6, #0xFF000000

    // Second pixel.
    lsl r5, #16
    orr r5, r5, r4, lsr #16
    orr r5, r5, #0xFF000000

    // Third pixel.
    lsl r4, #8
    orr r4, r4, r3, lsr #24
    orr r4, r4, #0xFF000000

    // Fourth pixel.
    orr r3, r3, #0xFF000000

    stmdb r0!, {r3, r4, r5, r6}
    subs r2, #4
    bhi _rip_RGB8ToRGBA8_copyMulti
    pop {r4, r5, r6}

    _rip_RGB8ToRGBA8_copySingle:
    cmp r2, #0
    bxeq lr
    sub r1, #3
    sub r0, #4
    ldr r3, [r1]
    pld [r1, #-32]
    orr r3, r3, #0xFF000000
    str r3, [r0]
    sub r2, #1
    b _rip_RGB8ToRGBA8_copySingle

// R0 = dst, R1 = src, R2 = numPixels.
.type rip_RGB565ToRGBA8, %function
rip_RGB565ToRGBA8:
    // ...
    bx lr

// R0 = dst, R1 = src, R2 = numPixels.
.type rip_RGB5A1ToRGBA8, %function
rip_RGB5A1ToRGBA8:
    // ...
    bx lr

// R0 = dst, R1 = src, R2 = numPixels.
.type rip_RGBA4ToRGBA8, %function
rip_RGBA4ToRGBA8:
    // ...
    bx lr