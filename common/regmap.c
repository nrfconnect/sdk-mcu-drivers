/**
 * @file regmap.c
 *
 * @brief Generic regmap operations implementation.
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
#include <stddef.h>
#include "regmap.h"

/***********************************************************************************************************************
 * LOCAL LITERAL SUBSTITUTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL FUNCTIONS
 **********************************************************************************************************************/
static uint32_t regmap_virtual_read(regmap_cp_config_t *cp, uint32_t addr, uint32_t *val)
{
    uint32_t ret = BSP_STATUS_FAIL;
    regmap_virtual_register_t *regfile = (regmap_virtual_register_t *) cp->dev_id;
    uint16_t regfile_length = cp->receive_max;

    for (uint16_t i = 0; i < regfile_length; i++)
    {
        if (regfile->address == addr)
        {
            if (regfile->on_read == NULL)
            {
                *val = regfile->default_value;
                ret = BSP_STATUS_OK;
            }
            else
            {
                ret = regfile->on_read((void *) regfile, val);
            }

            break;
        }

        regfile++;
    }

    return ret;
}

static uint32_t regmap_virtual_write(regmap_cp_config_t *cp, uint32_t addr, uint32_t val)
{
    uint32_t ret = BSP_STATUS_FAIL;
    regmap_virtual_register_t *regfile = (regmap_virtual_register_t *) cp->dev_id;
    uint16_t regfile_length = cp->receive_max;

    for (uint16_t i = 0; i < regfile_length; i++)
    {
        if (regfile->address == addr)
        {
            if (regfile->on_write != NULL)
            {
                ret = regfile->on_write((void *) regfile, val);
            }

            break;
        }

        regfile++;
    }

    return ret;
}

/***********************************************************************************************************************
 * API FUNCTIONS
 **********************************************************************************************************************/

/**
 * Reads the contents of a single register/memory address
 *
 */
uint32_t regmap_read(regmap_cp_config_t *cp, uint32_t addr, uint32_t *val)
{
    uint32_t ret = REGMAP_STATUS_FAIL;
    uint8_t write_buffer[4];
    uint8_t read_buffer[4];

    // Currently only I2C and SPI transactions are supported
    switch (cp->bus_type)
    {
        case REGMAP_BUS_TYPE_I2C:
            write_buffer[0] = GET_BYTE_FROM_WORD(addr, 3);
            write_buffer[1] = GET_BYTE_FROM_WORD(addr, 2);
            write_buffer[2] = GET_BYTE_FROM_WORD(addr, 1);
            write_buffer[3] = GET_BYTE_FROM_WORD(addr, 0);

            ret = bsp_driver_if_g->i2c_read_repeated_start(cp->dev_id, write_buffer, 4, read_buffer, 4, NULL, NULL);

            if (BSP_STATUS_OK == ret)
            {
                ADD_BYTE_TO_WORD(*val, read_buffer[0], 3);
                ADD_BYTE_TO_WORD(*val, read_buffer[1], 2);
                ADD_BYTE_TO_WORD(*val, read_buffer[2], 1);
                ADD_BYTE_TO_WORD(*val, read_buffer[3], 0);

                ret = REGMAP_STATUS_OK;
            }
            break;

        case REGMAP_BUS_TYPE_SPI:
            write_buffer[0] = GET_BYTE_FROM_WORD(addr, 3);
            write_buffer[1] = GET_BYTE_FROM_WORD(addr, 2);
            write_buffer[2] = GET_BYTE_FROM_WORD(addr, 1);
            write_buffer[3] = GET_BYTE_FROM_WORD(addr, 0);
            // Set the R/W bit
            write_buffer[0] = 0x80 | write_buffer[0];
            ret = bsp_driver_if_g->spi_read(cp->dev_id, write_buffer, 4, read_buffer, 4, cp->spi_pad_len);

            if (BSP_STATUS_OK == ret)
            {
                ADD_BYTE_TO_WORD(*val, read_buffer[0], 3);
                ADD_BYTE_TO_WORD(*val, read_buffer[1], 2);
                ADD_BYTE_TO_WORD(*val, read_buffer[2], 1);
                ADD_BYTE_TO_WORD(*val, read_buffer[3], 0);

                ret = REGMAP_STATUS_OK;
            }
            break;

        case REGMAP_BUS_TYPE_VIRTUAL:
            ret = regmap_virtual_read(cp, addr, val);
            break;

        default:
            break;
    }

    if (ret)
    {
        ret = REGMAP_STATUS_FAIL;
    }
    else
    {
        ret = REGMAP_STATUS_OK;
    }

    return ret;
}

/**
 * Writes the contents of a single register/memory address
 *
 */
uint32_t regmap_write(regmap_cp_config_t *cp, uint32_t addr, uint32_t val)
{
    uint32_t ret = REGMAP_STATUS_FAIL;
    uint8_t write_buffer[8];

    switch (cp->bus_type)
    {
        case REGMAP_BUS_TYPE_I2C:
            write_buffer[0] = GET_BYTE_FROM_WORD(addr, 3);
            write_buffer[1] = GET_BYTE_FROM_WORD(addr, 2);
            write_buffer[2] = GET_BYTE_FROM_WORD(addr, 1);
            write_buffer[3] = GET_BYTE_FROM_WORD(addr, 0);
            write_buffer[4] = GET_BYTE_FROM_WORD(val, 3);
            write_buffer[5] = GET_BYTE_FROM_WORD(val, 2);
            write_buffer[6] = GET_BYTE_FROM_WORD(val, 1);
            write_buffer[7] = GET_BYTE_FROM_WORD(val, 0);

            ret = bsp_driver_if_g->i2c_write(cp->dev_id, write_buffer, 8, NULL, NULL);
            break;

        case REGMAP_BUS_TYPE_SPI:
            write_buffer[0] = GET_BYTE_FROM_WORD(addr, 3);
            write_buffer[1] = GET_BYTE_FROM_WORD(addr, 2);
            write_buffer[2] = GET_BYTE_FROM_WORD(addr, 1);
            write_buffer[3] = GET_BYTE_FROM_WORD(addr, 0);
            write_buffer[4] = GET_BYTE_FROM_WORD(val, 3);
            write_buffer[5] = GET_BYTE_FROM_WORD(val, 2);
            write_buffer[6] = GET_BYTE_FROM_WORD(val, 1);
            write_buffer[7] = GET_BYTE_FROM_WORD(val, 0);

            ret = bsp_driver_if_g->spi_write(cp->dev_id,
                                             write_buffer,
                                             4,
                                             (uint8_t *)&(write_buffer[4]),
                                             4,
                                             cp->spi_pad_len);
            break;

        case REGMAP_BUS_TYPE_VIRTUAL:
            ret = regmap_virtual_write(cp, addr, val);
            break;

        default:
            break;
    }

    if (ret)
    {
        ret = REGMAP_STATUS_FAIL;
    }
    else
    {
        ret = REGMAP_STATUS_OK;
    }

    return ret;
}

/**
 * Read-Modify-Write of register using 32-bit mask
 *
 */
uint32_t regmap_update_reg(regmap_cp_config_t *cp, uint32_t addr, uint32_t mask, uint32_t val)
{
    uint32_t ret = REGMAP_STATUS_FAIL;
    uint32_t temp_val, data;

    ret = regmap_read(cp, addr, &data);

    if (ret)
    {
        return ret;
    }

    temp_val = data & ~(mask);
    temp_val |= val;

    if (data == temp_val)
    {
        return REGMAP_STATUS_OK;
    }

    ret = regmap_write(cp, addr, temp_val);

    return ret;
}

/**
 * Reads contents from a consecutive number of memory addresses
 *
 */
uint32_t regmap_read_block(regmap_cp_config_t *cp, uint32_t addr, uint8_t *bytes, uint32_t length)
{
    uint32_t ret = REGMAP_STATUS_FAIL;
    uint8_t write_buffer[4];

    // Check that 'length' does not exceed the size of the BSP buffer
    if (length <= cp->receive_max)
    {
        switch (cp->bus_type)
        {
            case REGMAP_BUS_TYPE_I2C:
                write_buffer[0] = GET_BYTE_FROM_WORD(addr, 3);
                write_buffer[1] = GET_BYTE_FROM_WORD(addr, 2);
                write_buffer[2] = GET_BYTE_FROM_WORD(addr, 1);
                write_buffer[3] = GET_BYTE_FROM_WORD(addr, 0);

                ret = bsp_driver_if_g->i2c_read_repeated_start(cp->dev_id, write_buffer, 4, bytes, length, NULL, NULL);
                break;

            case REGMAP_BUS_TYPE_SPI:
                write_buffer[0] = GET_BYTE_FROM_WORD(addr, 3);
                write_buffer[1] = GET_BYTE_FROM_WORD(addr, 2);
                write_buffer[2] = GET_BYTE_FROM_WORD(addr, 1);
                write_buffer[3] = GET_BYTE_FROM_WORD(addr, 0);

                // Set the R/W bit
                write_buffer[0] = 0x80 | write_buffer[0];
                ret = bsp_driver_if_g->spi_read(cp->dev_id, write_buffer, 4, bytes, length, cp->spi_pad_len);

                break;

            case REGMAP_BUS_TYPE_VIRTUAL:
                // 'length' is in bytes, so /4 to get in terms of 32-bit words
                for (uint32_t i = 0; i < (length >> 2); i++)
                {
                    ret = regmap_virtual_read(cp, addr, (uint32_t *) bytes);
                    if (ret)
                    {
                        break;
                    }

                    addr += 4;
                    bytes += 4;
                }
                break;

            default:
                break;
        }
    }

    if (ret)
    {
        ret = REGMAP_STATUS_FAIL;
    }
    else
    {
        ret = REGMAP_STATUS_OK;
    }

    return ret;
}

/**
 * Writes from byte array to consecutive number of Control Port memory addresses
 *
 */
uint32_t regmap_write_block(regmap_cp_config_t *cp, uint32_t addr, uint8_t *bytes, uint32_t length)
{
    uint32_t ret = REGMAP_STATUS_FAIL;
    uint8_t write_buffer[4];

    switch (cp->bus_type)
    {
        case REGMAP_BUS_TYPE_I2C:
            write_buffer[0] = GET_BYTE_FROM_WORD(addr, 3);
            write_buffer[1] = GET_BYTE_FROM_WORD(addr, 2);
            write_buffer[2] = GET_BYTE_FROM_WORD(addr, 1);
            write_buffer[3] = GET_BYTE_FROM_WORD(addr, 0);

            ret = bsp_driver_if_g->i2c_db_write(cp->dev_id, write_buffer, 4, bytes, length, NULL, NULL);
            break;

        case REGMAP_BUS_TYPE_SPI:
            write_buffer[0] = GET_BYTE_FROM_WORD(addr, 3);
            write_buffer[1] = GET_BYTE_FROM_WORD(addr, 2);
            write_buffer[2] = GET_BYTE_FROM_WORD(addr, 1);
            write_buffer[3] = GET_BYTE_FROM_WORD(addr, 0);

            ret = bsp_driver_if_g->spi_write(cp->dev_id, write_buffer, 4, bytes, length, cp->spi_pad_len);
            break;

        case REGMAP_BUS_TYPE_VIRTUAL:
            // 'length' is in bytes, so /4 to get in terms of 32-bit words
            for (uint32_t i = 0; i < (length >> 2); i++)
            {
                ret = regmap_virtual_write(cp, addr, (uint32_t) *bytes);
                if (ret)
                {
                    break;
                }

                addr += 4;
                bytes += 4;
            }
            break;

        default:
            break;
    }

    if (ret)
    {
        ret = REGMAP_STATUS_FAIL;
    }
    else
    {
        ret = REGMAP_STATUS_OK;
    }

    return ret;
}

/**
 * Writes a value in a list to corresponding address. Can support multiple type and you can choose
 *
 */
uint32_t regmap_write_array(regmap_cp_config_t *cp, uint32_t *addr_val, uint32_t size, uint32_t type)
{
    uint32_t ret = REGMAP_STATUS_FAIL;

    switch (type)
    {
        case REGMAP_WRITE_ARRAY_TYPE_ADDR_VAL:
            for (uint32_t i = 0; i < size; i+=2)
            {
                ret = regmap_write(cp, addr_val[i], addr_val[i+1]);
                if (ret)
                {
                    return ret;
                }
            }
            break;

        default:
            break;
    }

    return ret;
}

/**
 * Reads a register for a specific bit for a specified amount of tries while waiting between reads.
 *
 */
uint32_t regmap_poll_reg(regmap_cp_config_t *cp, uint32_t addr, uint32_t val, uint8_t tries, uint32_t delay)
{
    uint32_t tmp, ret;

    ret = REGMAP_STATUS_FAIL;

    for (int i = 0; i < tries; i++)
    {
        ret = regmap_read(cp, addr, &tmp);

        if (ret)
        {
          return ret;
        }

        if (tmp & val)
        {
          return REGMAP_STATUS_OK;;
        }

        bsp_driver_if_g->set_timer(delay, NULL, NULL);
    }

    return REGMAP_STATUS_FAIL;
}

/**
 * Reads a register corresponding to the respective symbol_id.
 *
 */
uint32_t regmap_read_fw_control(regmap_cp_config_t *cp,
                                  fw_img_info_t *f, uint32_t symbol_id, uint32_t *val)
{
    uint32_t temp_reg_addr, ret;

    temp_reg_addr = fw_img_find_symbol(f, symbol_id);

    if (temp_reg_addr == 0)
    {
        return REGMAP_STATUS_FAIL;
    }

    ret = regmap_read(cp, temp_reg_addr, val);

    if (ret)
    {
        return REGMAP_STATUS_FAIL;
    }

    return REGMAP_STATUS_OK;
}

/**
 * Writes a value to a register corresponding to the respective symbol_id.
 *
 */
uint32_t regmap_write_fw_control(regmap_cp_config_t *cp, fw_img_info_t *f,
                                   uint32_t symbol_id, uint32_t val)
{
    uint32_t temp_reg_addr, ret;

    temp_reg_addr = fw_img_find_symbol(f, symbol_id);

    if (temp_reg_addr == 0)
    {
        return REGMAP_STATUS_FAIL;
    }

    ret = regmap_write(cp, temp_reg_addr, val);

    if (ret)
    {
        return REGMAP_STATUS_FAIL;
    }

    return REGMAP_STATUS_OK;
}
