/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for TFT_SPI */
#define TFT_SPI_INST                                                       SPI0
#define TFT_SPI_INST_IRQHandler                                 SPI0_IRQHandler
#define TFT_SPI_INST_INT_IRQN                                     SPI0_INT_IRQn
#define GPIO_TFT_SPI_PICO_PORT                                            GPIOB
#define GPIO_TFT_SPI_PICO_PIN                                    DL_GPIO_PIN_17
#define GPIO_TFT_SPI_IOMUX_PICO                                 (IOMUX_PINCM43)
#define GPIO_TFT_SPI_IOMUX_PICO_FUNC                 IOMUX_PINCM43_PF_SPI0_PICO
#define GPIO_TFT_SPI_POCI_PORT                                            GPIOA
#define GPIO_TFT_SPI_POCI_PIN                                     DL_GPIO_PIN_4
#define GPIO_TFT_SPI_IOMUX_POCI                                  (IOMUX_PINCM9)
#define GPIO_TFT_SPI_IOMUX_POCI_FUNC                  IOMUX_PINCM9_PF_SPI0_POCI
/* GPIO configuration for TFT_SPI */
#define GPIO_TFT_SPI_SCLK_PORT                                            GPIOA
#define GPIO_TFT_SPI_SCLK_PIN                                    DL_GPIO_PIN_11
#define GPIO_TFT_SPI_IOMUX_SCLK                                 (IOMUX_PINCM22)
#define GPIO_TFT_SPI_IOMUX_SCLK_FUNC                 IOMUX_PINCM22_PF_SPI0_SCLK
#define GPIO_TFT_SPI_CS0_PORT                                             GPIOA
#define GPIO_TFT_SPI_CS0_PIN                                      DL_GPIO_PIN_2
#define GPIO_TFT_SPI_IOMUX_CS0                                   (IOMUX_PINCM7)
#define GPIO_TFT_SPI_IOMUX_CS0_FUNC                    IOMUX_PINCM7_PF_SPI0_CS0



/* Port definition for Pin Group TFT */
#define TFT_PORT                                                         (GPIOA)

/* Defines for DC: GPIOA.10 with pinCMx 21 on package pin 56 */
#define TFT_DC_PIN                                              (DL_GPIO_PIN_10)
#define TFT_DC_IOMUX                                             (IOMUX_PINCM21)
/* Defines for RES: GPIOA.13 with pinCMx 35 on package pin 6 */
#define TFT_RES_PIN                                             (DL_GPIO_PIN_13)
#define TFT_RES_IOMUX                                            (IOMUX_PINCM35)
/* Defines for CS: GPIOA.18 with pinCMx 40 on package pin 11 */
#define TFT_CS_PIN                                              (DL_GPIO_PIN_18)
#define TFT_CS_IOMUX                                             (IOMUX_PINCM40)
/* Defines for BL: GPIOA.17 with pinCMx 39 on package pin 10 */
#define TFT_BL_PIN                                              (DL_GPIO_PIN_17)
#define TFT_BL_IOMUX                                             (IOMUX_PINCM39)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TFT_SPI_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
