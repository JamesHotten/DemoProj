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




/* Defines for PWR: GPIOA.13 with pinCMx 35 on package pin 6 */
#define AD9910_PWR_PORT                                                  (GPIOA)
#define AD9910_PWR_PIN                                          (DL_GPIO_PIN_13)
#define AD9910_PWR_IOMUX                                         (IOMUX_PINCM35)
/* Defines for SDIO: GPIOA.12 with pinCMx 34 on package pin 5 */
#define AD9910_SDIO_PORT                                                 (GPIOA)
#define AD9910_SDIO_PIN                                         (DL_GPIO_PIN_12)
#define AD9910_SDIO_IOMUX                                        (IOMUX_PINCM34)
/* Defines for DRHOLD: GPIOA.11 with pinCMx 22 on package pin 57 */
#define AD9910_DRHOLD_PORT                                               (GPIOA)
#define AD9910_DRHOLD_PIN                                       (DL_GPIO_PIN_11)
#define AD9910_DRHOLD_IOMUX                                      (IOMUX_PINCM22)
/* Defines for DROVER: GPIOA.10 with pinCMx 21 on package pin 56 */
#define AD9910_DROVER_PORT                                               (GPIOA)
#define AD9910_DROVER_PIN                                       (DL_GPIO_PIN_10)
#define AD9910_DROVER_IOMUX                                      (IOMUX_PINCM21)
/* Defines for UP_DAT: GPIOB.13 with pinCMx 30 on package pin 1 */
#define AD9910_UP_DAT_PORT                                               (GPIOB)
#define AD9910_UP_DAT_PIN                                       (DL_GPIO_PIN_13)
#define AD9910_UP_DAT_IOMUX                                      (IOMUX_PINCM30)
/* Defines for PROFILE1: GPIOB.20 with pinCMx 48 on package pin 19 */
#define AD9910_PROFILE1_PORT                                             (GPIOB)
#define AD9910_PROFILE1_PIN                                     (DL_GPIO_PIN_20)
#define AD9910_PROFILE1_IOMUX                                    (IOMUX_PINCM48)
/* Defines for PROFILE2: GPIOA.31 with pinCMx 6 on package pin 39 */
#define AD9910_PROFILE2_PORT                                             (GPIOA)
#define AD9910_PROFILE2_PIN                                     (DL_GPIO_PIN_31)
#define AD9910_PROFILE2_IOMUX                                     (IOMUX_PINCM6)
/* Defines for PROFILE3: GPIOA.28 with pinCMx 3 on package pin 35 */
#define AD9910_PROFILE3_PORT                                             (GPIOA)
#define AD9910_PROFILE3_PIN                                     (DL_GPIO_PIN_28)
#define AD9910_PROFILE3_IOMUX                                     (IOMUX_PINCM3)
/* Defines for MAS_REST: GPIOB.12 with pinCMx 29 on package pin 64 */
#define AD9910_MAS_REST_PORT                                             (GPIOB)
#define AD9910_MAS_REST_PIN                                     (DL_GPIO_PIN_12)
#define AD9910_MAS_REST_IOMUX                                    (IOMUX_PINCM29)
/* Defines for SCLK: GPIOB.17 with pinCMx 43 on package pin 14 */
#define AD9910_SCLK_PORT                                                 (GPIOB)
#define AD9910_SCLK_PIN                                         (DL_GPIO_PIN_17)
#define AD9910_SCLK_IOMUX                                        (IOMUX_PINCM43)
/* Defines for DRCTL: GPIOB.15 with pinCMx 32 on package pin 3 */
#define AD9910_DRCTL_PORT                                                (GPIOB)
#define AD9910_DRCTL_PIN                                        (DL_GPIO_PIN_15)
#define AD9910_DRCTL_IOMUX                                       (IOMUX_PINCM32)
/* Defines for OSK: GPIOB.8 with pinCMx 25 on package pin 60 */
#define AD9910_OSK_PORT                                                  (GPIOB)
#define AD9910_OSK_PIN                                           (DL_GPIO_PIN_8)
#define AD9910_OSK_IOMUX                                         (IOMUX_PINCM25)
/* Defines for CS: GPIOB.7 with pinCMx 24 on package pin 59 */
#define AD9910_CS_PORT                                                   (GPIOB)
#define AD9910_CS_PIN                                            (DL_GPIO_PIN_7)
#define AD9910_CS_IOMUX                                          (IOMUX_PINCM24)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
