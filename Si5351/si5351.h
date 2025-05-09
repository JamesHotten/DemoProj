#ifndef __si5351_H
#define __si5351_H
#include "ti_msp_dl_config.h"

#ifndef GPIO_Si5351_PIN_SCL_PORT
#define GPIO_Si5351_PIN_SCL_PORT GPIO_Si5351_PORT
#endif

#ifndef GPIO_Si5351_PIN_SDA_PORT
#define GPIO_Si5351_PIN_SDA_PORT GPIO_Si5351_PORT
#endif

//----------------------------------------------------------------------------------
// Si5351 I2C 时钟SCL
#define Si5351_SCL_Set()                                                       \
  (DL_GPIO_setPins(GPIO_Si5351_PIN_SCL_PORT, GPIO_Si5351_PIN_SCL_PIN))
#define Si5351_SCL_Clr()                                                       \
  (DL_GPIO_clearPins(GPIO_Si5351_PIN_SCL_PORT, GPIO_Si5351_PIN_SCL_PIN))

//----------------------------------------------------------------------------------
// Si5351 I2C 数据SDA
#define Si5351_SDA_Set()                                                       \
  (DL_GPIO_setPins(GPIO_Si5351_PIN_SDA_PORT, GPIO_Si5351_PIN_SDA_PIN))
#define Si5351_SDA_Clr()                                                       \
  (DL_GPIO_clearPins(GPIO_Si5351_PIN_SDA_PORT, GPIO_Si5351_PIN_SDA_PIN))

void Si5351_Init(void);
#endif
