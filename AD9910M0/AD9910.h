#ifndef __AD9910_H__
#define __AD9910_H__

#include "ti_msp_dl_config.h"

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
#define u32 uint32_t
#define u16 uint16_t

#define AD9910_PWR(set)                                                        \
  (set ? DL_GPIO_setPins(AD9910_PWR_PORT, AD9910_PWR_PIN)                      \
       : DL_GPIO_clearPins(AD9910_PWR_PORT, AD9910_PWR_PIN))

#define AD9910_SDIO(set)                                                       \
  (set ? DL_GPIO_setPins(AD9910_SDIO_PORT, AD9910_SDIO_PIN)                    \
       : DL_GPIO_clearPins(AD9910_SDIO_PORT, AD9910_SDIO_PIN))

#define AD9910_DRHOLD(set)                                                     \
  (set ? DL_GPIO_setPins(AD9910_DRHOLD_PORT, AD9910_DRHOLD_PIN)                \
       : DL_GPIO_clearPins(AD9910_DRHOLD_PORT, AD9910_DRHOLD_PIN))

#define AD9910_DROVER(set)                                                     \
  (set ? DL_GPIO_setPins(AD9910_DROVER_PORT, AD9910_DROVER_PIN)                \
       : DL_GPIO_clearPins(AD9910_DROVER_PORT, AD9910_DROVER_PIN))

#define AD9910_UP_DAT(set)                                                     \
  (set ? DL_GPIO_setPins(AD9910_UP_DAT_PORT, AD9910_UP_DAT_PIN)                \
       : DL_GPIO_clearPins(AD9910_UP_DAT_PORT, AD9910_UP_DAT_PIN))

#define AD9910_PROFILE1(set)                                                   \
  (set ? DL_GPIO_setPins(AD9910_PROFILE1_PORT, AD9910_PROFILE1_PIN)            \
       : DL_GPIO_clearPins(AD9910_PROFILE1_PORT, AD9910_PROFILE1_PIN))

#define AD9910_PROFILE2(set)                                                   \
  (set ? DL_GPIO_setPins(AD9910_PROFILE2_PORT, AD9910_PROFILE2_PIN)            \
       : DL_GPIO_clearPins(AD9910_PROFILE2_PORT, AD9910_PROFILE2_PIN))

#define AD9910_PROFILE3(set)                                                   \
  (set ? DL_GPIO_setPins(AD9910_PROFILE3_PORT, AD9910_PROFILE3_PIN)            \
       : DL_GPIO_clearPins(AD9910_PROFILE3_PORT, AD9910_PROFILE3_PIN))

#define AD9910_MAS_REST(set)                                                   \
  (set ? DL_GPIO_setPins(AD9910_MAS_REST_PORT, AD9910_MAS_REST_PIN)            \
       : DL_GPIO_clearPins(AD9910_MAS_REST_PORT, AD9910_MAS_REST_PIN))

#define AD9910_SCLK(set)                                                       \
  (set ? DL_GPIO_setPins(AD9910_SCLK_PORT, AD9910_SCLK_PIN)                    \
       : DL_GPIO_clearPins(AD9910_SCLK_PORT, AD9910_SCLK_PIN))

#define AD9910_DRCTL(set)                                                      \
  (set ? DL_GPIO_setPins(AD9910_DRCTL_PORT, AD9910_DRCTL_PIN)                  \
       : DL_GPIO_clearPins(AD9910_DRCTL_PORT, AD9910_DRCTL_PIN))

#define AD9910_OSK(set)                                                        \
  (set ? DL_GPIO_setPins(AD9910_OSK_PORT, AD9910_OSK_PIN)                      \
       : DL_GPIO_clearPins(AD9910_OSK_PORT, AD9910_OSK_PIN))

#define AD9910_CS(set)                                                         \
  (set ? DL_GPIO_setPins(AD9910_CS_PORT, AD9910_CS_PIN)                        \
       : DL_GPIO_clearPins(AD9910_CS_PORT, AD9910_CS_PIN))

typedef enum {
  TRIG_WAVE = 0,
  SQUARE_WAVE,
  SINC_WAVE,
} AD9910_WAVE_ENUM;

void AD9910_Init();
void AD9910_FreWrite(ulong Freq);
void AD9910_AmpWrite(uint16_t Amp);
void AD9910_RAM_WAVE_Set(AD9910_WAVE_ENUM wave);
void AD9910_DRG_AMP_Init(void);
void AD9910_DRG_FreInit_AutoSet(bool autoSweepEn);
void AD9910_DRG_FrePara_Set(u32 lowFre, u32 upFre, u32 posStep, u32 negStep,
                            u16 posRate, u16 negRate);

#endif