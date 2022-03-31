/**
 * @file cs47l63_syscfg_regs.c
 *
 * @brief Register values to be applied after CS47L63 Driver boot().
 *
 * @copyright
 * Copyright (c) Cirrus Logic 2022 All Rights Reserved, http://www.cirrus.com/
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
/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "cs47l63_syscfg_regs.h"
#include "cs47l63_spec.h"

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

uint32_t cs47l63_syscfg_regs[] = { 0x1400, 0x0041,     0x1c04, 0x28601177, 0x1c08, 0x10000,
				   0x1c0c, 0x23f05004, 0x1c00, 0x0006,	   0x1404, 0x0404,
				   0x1420, 0x0003,     0x1510, 0x25800004, 0x0c08, 0xe1000000,
				   0x0c0c, 0xe1000000, 0x0c10, 0xe1000000, 0x0c14, 0xe1000000,
				   0x6004, 0x0033,     0x6008, 0x20200200, 0x6000, 0x30003 };
