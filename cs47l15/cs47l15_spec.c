/**
 * @file cs47l15_spec.c
 *
 * @brief Constant Arrays/Lists from CS47L15 datasheet DS1215F2
 *
 * @copyright
 * Copyright (c) Cirrus Logic 2019 All Rights Reserved, http://www.cirrus.com/
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "cs47l15_spec.h"

/***********************************************************************************************************************
 * LOCAL LITERAL SUBSTITUTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * GLOBAL VARIABLES
 **********************************************************************************************************************/

const uint8_t cs47l15_bst_k1_table[4][5] =
{
// C_BST values (in uF) of:
//   <20    20-50   51-100  101-200 >200
    {0x24,  0x32,   0x32,   0x4F,   0x57}, // L_BST = 1.0 uH
    {0x24,  0x32,   0x32,   0x4F,   0x57}, // L_BST = 1.2 uH
    {0x40,  0x32,   0x32,   0x4F,   0x57}, // L_BST = 1.5 uH
    {0x40,  0x32,   0x32,   0x4F,   0x57}  // L_BST = 2.2 uH
};

const uint8_t cs47l15_bst_k2_table[4][5] =
{
// C_BST values (in uF) of:
//   <20    20-50   51-100  101-200 >200
    {0x24, 0x49, 0x66, 0xA3, 0xEA}, // L_BST = 1.0 uH
    {0x24, 0x49, 0x66, 0xA3, 0xEA}, // L_BST = 1.2 uH
    {0x48, 0x49, 0x66, 0xA3, 0xEA}, // L_BST = 1.5 uH
    {0x48, 0x49, 0x66, 0xA3, 0xEA}  // L_BST = 2.2 uH
};

const uint8_t cs47l15_bst_slope_table[4] =
{
    0x75, // L_BST = 1.0 uH
    0x6B, // L_BST = 1.2 uH
    0x3B, // L_BST = 1.5 uH
    0x28  // L_BST = 2.2 uH
};

const struct cs47l15_register_encoding cs47l15_pll_sysclk[64] = {
    { 32768,    0x00 },
    { 8000,     0x01 },
    { 11025,    0x02 },
    { 12000,    0x03 },
    { 16000,    0x04 },
    { 22050,    0x05 },
    { 24000,    0x06 },
    { 32000,    0x07 },
    { 44100,    0x08 },
    { 48000,    0x09 },
    { 88200,    0x0A },
    { 96000,    0x0B },
    { 128000,   0x0C },
    { 176400,   0x0D },
    { 192000,   0x0E },
    { 256000,   0x0F },
    { 352800,   0x10 },
    { 384000,   0x11 },
    { 512000,   0x12 },
    { 705600,   0x13 },
    { 750000,   0x14 },
    { 768000,   0x15 },
    { 1000000,  0x16 },
    { 1024000,  0x17 },
    { 1200000,  0x18 },
    { 1411200,  0x19 },
    { 1500000,  0x1A },
    { 1536000,  0x1B },
    { 2000000,  0x1C },
    { 2048000,  0x1D },
    { 2400000,  0x1E },
    { 2822400,  0x1F },
    { 3000000,  0x20 },
    { 3072000,  0x21 },
    { 3200000,  0x22 },
    { 4000000,  0x23 },
    { 4096000,  0x24 },
    { 4800000,  0x25 },
    { 5644800,  0x26 },
    { 6000000,  0x27 },
    { 6144000,  0x28 },
    { 6250000,  0x29 },
    { 6400000,  0x2A },
    { 6500000,  0x2B },
    { 6750000,  0x2C },
    { 7526400,  0x2D },
    { 8000000,  0x2E },
    { 8192000,  0x2F },
    { 9600000,  0x30 },
    { 11289600, 0x31 },
    { 12000000, 0x32 },
    { 12288000, 0x33 },
    { 12500000, 0x34 },
    { 12800000, 0x35 },
    { 13000000, 0x36 },
    { 13500000, 0x37 },
    { 19200000, 0x38 },
    { 22579200, 0x39 },
    { 24000000, 0x3A },
    { 24576000, 0x3B },
    { 25000000, 0x3C },
    { 25600000, 0x3D },
    { 26000000, 0x3E },
    { 27000000, 0x3F },
};

const struct cs47l15_register_encoding cs47l15_sclk_encoding[48] =
{
    { 128000,    0x0C },
    { 176400,    0x0D },
    { 192000,    0x0E },
    { 256000,    0x0F },
    { 352800,    0x10 },
    { 384000,    0x11 },
    { 512000,    0x12 },
    { 705600,    0x13 },
    { 750000,    0x14 },
    { 768000,    0x15 },
    { 1000000,   0x16 },
    { 1024000,   0x17 },
    { 1200000,   0x18 },
    { 1411200,   0x19 },
    { 1500000,   0x1A },
    { 1536000,   0x1B },
    { 2000000,   0x1C },
    { 2048000,   0x1D },
    { 2400000,   0x1E },
    { 2822400,   0x1F },
    { 3000000,   0x20 },
    { 3072000,   0x21 },
    { 3200000,   0x22 },
    { 4000000,   0x23 },
    { 4096000,   0x24 },
    { 4800000,   0x25 },
    { 5644800,   0x26 },
    { 6000000,   0x27 },
    { 6144000,   0x28 },
    { 6250000,   0x29 },
    { 6400000,   0x2A },
    { 6500000,   0x2B },
    { 6750000,   0x2C },
    { 7526400,   0x2D },
    { 8000000,   0x2E },
    { 8192000,   0x2F },
    { 9600000,   0x30 },
    { 11289600,  0x31 },
    { 12000000,  0x32 },
    { 12288000,  0x33 },
    { 12500000,  0x34 },
    { 12800000,  0x35 },
    { 13000000,  0x36 },
    { 13500000,  0x37 },
    { 19200000,  0x38 },
    { 22579200,  0x39 },
    { 24000000,  0x3A },
    { 24576000,  0x3B },
};

/***********************************************************************************************************************
 * LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * API FUNCTIONS
 **********************************************************************************************************************/