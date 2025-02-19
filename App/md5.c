/*------------------------------------------------------------------------------
 * Copyright (c) 2019 JCXX. All rights reserved.
 *------------------------------------------------------------------------------
 * Autor:   lsp
 * Name:    md5.c
 * Purpose: md5 calculate
 *----------------------------------------------------------------------------*/
#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <stdint.h>
 

// Constants are the integer part of the sines of integers (in radians) * 2^32.

static uint32_t k[64];

 

// r specifies the per-round shift amounts

static uint32_t r[64];

 

// leftrotate function definition

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

 

void to_bytes(uint32_t val, uint8_t *bytes)

{

    bytes[0] = (uint8_t) val;

    bytes[1] = (uint8_t) (val >> 8);

    bytes[2] = (uint8_t) (val >> 16);

    bytes[3] = (uint8_t) (val >> 24);

}

 

uint32_t to_int32(const uint8_t *bytes)

{

    return (uint32_t) bytes[0]

        | ((uint32_t) bytes[1] << 8)

        | ((uint32_t) bytes[2] << 16)

        | ((uint32_t) bytes[3] << 24);

}

void md5_init(void)
{
    k[0]  = 0xd76aa478;
    k[1]  = 0xe8c7b756;
    k[2]  = 0x242070db;
    k[3]  = 0xc1bdceee;
    
    k[4]  = 0xf57c0faf;
    k[5]  = 0x4787c62a;
    k[6]  = 0xa8304613;
    k[7]  = 0xfd469501;
    
    k[8]  = 0x698098d8;
    k[9]  = 0x8b44f7af;
    k[10] = 0xffff5bb1;
    k[11] = 0x895cd7be;
    
    k[12] = 0x6b901122;
    k[13] = 0xfd987193;
    k[14] = 0xa679438e;
    k[15] = 0x49b40821;
    
    k[16] = 0xf61e2562;
    k[17] = 0xc040b340;
    k[18] = 0x265e5a51;
    k[19] = 0xe9b6c7aa;
    
    k[20] = 0xd62f105d;
    k[21] = 0x02441453;
    k[22] = 0xd8a1e681;
    k[23] = 0xe7d3fbc8;
    
    k[24] = 0x21e1cde6;
    k[25] = 0xc33707d6;
    k[26] = 0xf4d50d87;
    k[27] = 0x455a14ed;
    
    k[28] = 0xa9e3e905;
    k[29] = 0xfcefa3f8;
    k[30] = 0x676f02d9;
    k[31] = 0x8d2a4c8a;
    
    k[32] = 0xfffa3942;
    k[33] = 0x8771f681;
    k[34] = 0x6d9d6122;
    k[35] = 0xfde5380c;
    
    k[36] = 0xa4beea44;
    k[37] = 0x4bdecfa9;
    k[38] = 0xf6bb4b60;
    k[39] = 0xbebfbc70;
    
    k[40] = 0x289b7ec6;
    k[41] = 0xeaa127fa;
    k[42] = 0xd4ef3085;
    k[43] = 0x04881d05;
    
    k[44] = 0xd9d4d039;
    k[45] = 0xe6db99e5;
    k[46] = 0x1fa27cf8;
    k[47] = 0xc4ac5665;
    
    k[48] = 0xf4292244;
    k[49] = 0x432aff97;
    k[50] = 0xab9423a7;
    k[51] = 0xfc93a039;
    
    k[52] = 0x655b59c3;
    k[53] = 0x8f0ccc92;
    k[54] = 0xffeff47d;
    k[55] = 0x85845dd1;
    
    k[56] = 0x6fa87e4f;
    k[57] = 0xfe2ce6e0;
    k[58] = 0xa3014314;
    k[59] = 0x4e0811a1;
    
    k[60] = 0xf7537e82;
    k[61] = 0xbd3af235;
    k[62] = 0x2ad7d2bb;
    k[63] = 0xeb86d391;
    
    
    r[0]  = 7;
    r[1]  = 12;
    r[2]  = 17;
    r[3]  = 22;
    
    r[4]  = 7;
    r[5]  = 12;
    r[6]  = 17;
    r[7]  = 22;
    
    r[8]  = 7;
    r[9]  = 12;
    r[10] = 17;
    r[11] = 22;
    
    r[12] = 7;
    r[13] = 12;
    r[14] = 17;
    r[15] = 22;
    
    r[16] = 5;
    r[17] = 9;
    r[18] = 14;
    r[19] = 20;

    r[20] = 5;
    r[21] = 9;
    r[22] = 14;
    r[23] = 20;
    
    r[24] = 5;
    r[25] = 9;
    r[26] = 14;
    r[27] = 20;
    
    r[28] = 5;
    r[29] = 9;
    r[30] = 14;
    r[31] = 20;
    
    r[32] = 4;
    r[33] = 11;
    r[34] = 16;
    r[35] = 23;
    
    r[36] = 4;
    r[37] = 11;
    r[38] = 16;
    r[39] = 23;
    
    r[40] = 4;
    r[41] = 11;
    r[42] = 16;
    r[43] = 23;
    
    r[44] = 4;
    r[45] = 11;
    r[46] = 16;
    r[47] = 23;
    
    r[48] = 6;
    r[49] = 10;
    r[50] = 15;
    r[51] = 21;
    
    r[52] = 6;
    r[53] = 10;
    r[54] = 15;
    r[55] = 21;
    
    r[56] = 6;
    r[57] = 10;
    r[58] = 15;
    r[59] = 21;
    
    r[60] = 6;
    r[61] = 10;
    r[62] = 15;
    r[63] = 21;
}

void md5_random(uint8_t *dst, uint8_t line)
{
    line %= 16;
    memcpy(dst, &k[line*4], 16);
}

void md5(const uint8_t *initial_msg, int initial_len, uint8_t *digest) {

 

    // These vars will contain the hash

    uint32_t h0, h1, h2, h3;

 

    // Message (to prepare)

    uint8_t *msg = NULL;

 

    size_t new_len, offset;

    uint32_t w[16];

    uint32_t a, b, c, d, i, f, g, temp;

 

    // Initialize variables - simple count in nibbles:

    h0 = 0x67452301;

    h1 = 0xefcdab89;

    h2 = 0x98badcfe;

    h3 = 0x10325476;

 

    //Pre-processing:

    //append "1" bit to message    

    //append "0" bits until message length in bits ¡Ô 448 (mod 512)

    //append length mod (2^64) to message

 

    for (new_len = initial_len + 1; new_len % (512/8) != 448/8; new_len++)

        ;

 

    msg = (uint8_t*)malloc(new_len + 8);

    memcpy(msg, initial_msg, initial_len);

    msg[initial_len] = 0x80; // append the "1" bit; most significant bit is "first"

    for (offset = initial_len + 1; offset < new_len; offset++)

        msg[offset] = 0; // append "0" bits

 

    // append the len in bits at the end of the buffer.

    to_bytes(initial_len*8, msg + new_len);

    // initial_len>>29 == initial_len*8>>32, but avoids overflow.

    to_bytes(initial_len>>29, msg + new_len + 4);

 

    // Process the message in successive 512-bit chunks:

    //for each 512-bit chunk of message:

    for(offset=0; offset<new_len; offset += (512/8)) {

 

        // break chunk into sixteen 32-bit words w[j], 0 ¡Ü j ¡Ü 15

        for (i = 0; i < 16; i++)

            w[i] = to_int32(msg + offset + i*4);

 

        // Initialize hash value for this chunk:

        a = h0;

        b = h1;

        c = h2;

        d = h3;

 

        // Main loop:

        for(i = 0; i<64; i++) {

 

            if (i < 16) {

                f = (b & c) | ((~b) & d);

                g = i;

            } else if (i < 32) {

                f = (d & b) | ((~d) & c);

                g = (5*i + 1) % 16;

            } else if (i < 48) {

                f = b ^ c ^ d;

                g = (3*i + 5) % 16;          

            } else {

                f = c ^ (b | (~d));

                g = (7*i) % 16;

            }

 

            temp = d;

            d = c;

            c = b;

            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);

            a = temp;

 

        }

 

        // Add this chunk's hash to result so far:

        h0 += a;

        h1 += b;

        h2 += c;

        h3 += d;

 

    }

 

    // cleanup

    free(msg);

 

    //var char digest[16] := h0 append h1 append h2 append h3 //(Output is in little-endian)

    to_bytes(h0, digest);

    to_bytes(h1, digest + 4);

    to_bytes(h2, digest + 8);

    to_bytes(h3, digest + 12);

}

