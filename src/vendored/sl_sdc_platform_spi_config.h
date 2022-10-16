/***************************************************************************//**
 * @file sl_sdc_platform_spi_config.h
 * @brief Platform SPI Driver Configuration
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
 ********************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided \'as-is\', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#ifndef SL_SDC_PLATFORM_SPI_CONFIG_H
#define SL_SDC_PLATFORM_SPI_CONFIG_H

// Modified by Håvard to use the spidrv sdcard instance
#include "sl_spidrv_instances.h"
#include "sl_spidrv_usart_sdcard_config.h"

#define sdc_spi_handle                      sl_spidrv_usart_sdcard_handle

#define SD_CARD_MMC_PERIPHERAL              SL_SPIDRV_USART_SDCARD_PERIPHERAL
#define SD_CARD_MMC_PERIPHERAL_NO           SL_SPIDRV_USART_SDCARD_PERIPHERAL_NO

// Set slow clock for card initialization (100k-400k)
#define SD_CARD_MMC_SLOW_CLOCK              100000
// Set fast clock for generic read/write
#define SD_CARD_MMC_FAST_CLOCK              SL_SPIDRV_USART_SDCARD_BITRATE

// USART0 CS on PC03
#define SD_CARD_MMC_CS_PORT                 SL_SPIDRV_USART_SDCARD_CS_PORT
#define SD_CARD_MMC_CS_PIN                  SL_SPIDRV_USART_SDCARD_CS_PIN

#endif /* SL_SDC_PLATFORM_SPI_CONFIG_H_ */
