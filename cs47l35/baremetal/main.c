/**
 * @file main.c
 *
 * @brief The main function for CS47L35 System Test Harness
 *
 * @copyright
 * Copyright (c) Cirrus Logic 2021 All Rights Reserved, http://www.cirrus.com/
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
#include "platform_bsp.h"
#include <stddef.h>
#include <stdlib.h>

/***********************************************************************************************************************
 * LOCAL LITERAL SUBSTITUTIONS
 **********************************************************************************************************************/
#define APP_STATE_UNINITIALIZED                  (0)
#define APP_STATE_STANDBY                        (1)
#define APP_STATE_TG_HP                          (2)
#define APP_STATE_OPUS_16K_RECORD_INIT           (3)
#define APP_STATE_OPUS_16K_RECORD                (4)
#define APP_STATE_OPUS_16K_RECORD_DONE           (5)

/***********************************************************************************************************************
 * LOCAL VARIABLES
 **********************************************************************************************************************/
static uint16_t app_state = APP_STATE_UNINITIALIZED;
static bool bsp_pb_pressed = false;

/***********************************************************************************************************************
 * GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL FUNCTIONS
 **********************************************************************************************************************/
void app_bsp_callback(uint32_t status, void *arg)
{
    if (status == BSP_STATUS_FAIL)
    {
        exit(1);
    }

    return;
}

/***********************************************************************************************************************
 * API FUNCTIONS
 **********************************************************************************************************************/

/**
 * @brief The Main Entry Point from __main
 *  By this time, the RAM RW-Data section has been initialized by the ARM-provided __main function.
 *
 * @return N/A (does not return)
 */
int main(void)
{
    int ret_val = 0;

    bsp_initialize(app_bsp_callback, NULL);
    bsp_dut_initialize();

    bsp_set_ld2(BSP_LD2_MODE_ON, 0);

    bsp_dut_reset();
    app_state = APP_STATE_STANDBY;

    while (1)
    {
        bsp_dut_process();
        if (bsp_was_pb_pressed(BSP_PB_ID_USER))
        {
            bsp_pb_pressed = true;
        }
        switch (app_state)
        {
            case APP_STATE_STANDBY:
                if (bsp_pb_pressed)
                {
                    bsp_dut_use_case(BSP_USE_CASE_TG_HP_EN);
                    app_state++;
                }
                break;

            case APP_STATE_TG_HP:
                if (bsp_pb_pressed)
                {
                    bsp_dut_use_case(BSP_USE_CASE_TG_HP_DIS);
                    app_state++;
                }
                break;

            case APP_STATE_OPUS_16K_RECORD_INIT:
                if (bsp_pb_pressed)
                {
                    bsp_dut_use_case(BSP_USE_CASE_OPUS_RECORD_16K_INIT);
                    app_state++;
                }
                break;

            case APP_STATE_OPUS_16K_RECORD:
                bsp_dut_use_case(BSP_USE_CASE_OPUS_RECORD);
                if (bsp_read_process_done & bsp_write_process_done)
                {
                    app_state++;
                }
                break;

            case APP_STATE_OPUS_16K_RECORD_DONE:
                bsp_dut_use_case(BSP_USE_CASE_OPUS_RECORD_DONE);
                app_state = APP_STATE_STANDBY;
                break;

            default:
                break;
        }

        bsp_pb_pressed = false;

        bsp_sleep();
    }

    exit(1);

    return ret_val;
}
