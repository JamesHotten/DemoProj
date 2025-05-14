#pragma once

#include <stdint.h>
#include <stdio.h>
#include <ti/devices/msp432e4/driverlib/driverlib.h>


extern void UART_printf(const char *pcString, ...);
extern void UART_Configure(void);

extern void HMI_printf(const char *pcString, ...);
extern void HMI_Configure(void);

extern void BT_printf(const char *pcString, ...);
extern void BT_Configure(void);
