#pragma once
#include <stdint.h>
#include <ti\devices\msp432e4\driverlib\driverlib.h>

// CH0-PE3-10±∂ CH1-PE2-1±∂
#define AMP1  ADC_CTL_CH0
#define AMP10 ADC_CTL_CH0


extern uint16_t *adcData;

#ifdef __cplusplus
extern "C" {
#endif

void ConfigureAdc();             // ≈‰÷√ADC
void StartAdcAMP1(int freq, int adcDataLen); // ∆Ù∂ØADC

#ifdef __cplusplus
}
#endif
