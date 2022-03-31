#
# Copyright (c) 2022 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

if(NOT CONFIG_HW_CODEC_CIRRUS_LOGIC)
  return()
endif()

zephyr_sources(
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/common/regmap.c
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/cs47l63/cs47l63.c
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/cs47l63/bsp/bsp_cs47l63.c
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/cs47l63/bsp/vregmap.c
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/cs47l63/generated/cs47l63_fw_img.c
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/cs47l63/generated/cs47l63_syscfg_regs.c
)

zephyr_include_directories(
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/common
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/common/bridge
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/common/platform_bsp
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/cs47l63
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/cs47l63/bsp
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/cs47l63/config
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/cs47l63/fw_pt
    ${ZEPHYR_CIRRUS_LOGIC_MODULE_DIR}/cs47l63/generated
)