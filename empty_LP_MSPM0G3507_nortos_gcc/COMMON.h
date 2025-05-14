#ifndef COMMON_H
#define COMMON_H
#include "TFT180.h"
#include "ti_msp_dl_config.h"


#define system_delay_ms(m) delay_cycles((m * 32000))
#endif
