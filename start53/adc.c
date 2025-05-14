#include "ti/devices/msp432e4/driverlib/rom_map.h"
#include <adc.h>
#include <ti/devices/msp432e4/driverlib/adc.h>
#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include <ti/devices/msp432e4/driverlib/interrupt.h>
#include <ti/devices/msp432e4/driverlib/timer.h>
#include <ti/devices/msp432e4/driverlib/udma.h>
#include <ti/devices/msp432e4/inc/msp432e401y.h>

static uint32_t timerPeriph0 = SYSCTL_PERIPH_TIMER4;
static uint32_t timerBase0 = TIMER4_BASE;
static uint32_t adcPeriph0 = SYSCTL_PERIPH_ADC0;
static uint32_t adcBase0 = ADC0_BASE;
static uint32_t adcSeq0 = 2;
static uint32_t adcInt0 = INT_ADC0SS2;
static uint32_t adcIntFlag0 = ADC_INT_DMA_SS2;
static uint32_t dmaChann0 = UDMA_CH16_ADC0_2;
static uint32_t dmaArb0 = UDMA_ARB_4;
static uint32_t adcPriority0 = 0;

// static uint32_t timerPeriph1 = SYSCTL_PERIPH_TIMER4;
// static uint32_t timerBase1   = TIMER4_BASE;
// static uint32_t adcPeriph1   = SYSCTL_PERIPH_ADC1;
// static uint32_t adcBase1     = ADC1_BASE;
// static uint32_t adcSeq1      = 2;
// static uint32_t adcInt1      = INT_ADC1SS2;
// static uint32_t adcIntFlag1  = ADC_INT_DMA_SS2;
// static uint32_t dmaChann1    = UDMA_CH26_ADC1_2;
// static uint32_t dmaArb1      = UDMA_ARB_4;
// static uint32_t adcPriority1 = 0;

uint16_t adcData0[1024], adcData1[1024];
uint16_t *adcData = adcData0;

volatile bool adc_done = false;

// DMA���Ʊ�
#if defined(__ICCARM__)
#pragma data_alignment = 1024
uint8_t pui8ControlTable[1024];
#elif defined(__TI_ARM__)
#pragma DATA_ALIGN(pui8ControlTable, 1024)
uint8_t pui8ControlTable[1024];
#else
uint8_t pui8ControlTable[1024] __attribute__((aligned(1024)));
#endif

void AdcHandler() {
  if ((MAP_ADCIntStatusEx(adcBase0, true) & adcIntFlag0) == adcIntFlag0) {
    // ֹͣADC
    //        MAP_ADCSequenceDMADisable(adcBase0, adcSeq0);
    //        MAP_ADCSequenceDisable(adcBase0, adcSeq0);
    MAP_ADCIntClearEx(adcBase0, adcIntFlag0);
    MAP_ADCIntDisableEx(adcBase0, adcIntFlag0);
    MAP_IntDisable(adcInt0);
    MAP_TimerDisable(timerBase0, TIMER_A);
    //        MAP_uDMAChannelDisable(dmaChann0);
    adc_done = true;
  }
}

void ConfigureAdc() {
  MAP_SysCtlPeripheralEnable(timerPeriph0);
  //    MAP_SysCtlPeripheralEnable(timerPeriph1);
  MAP_SysCtlPeripheralEnable(adcPeriph0);
  //    MAP_SysCtlPeripheralEnable(adcPeriph1);
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
  MAP_uDMAControlBaseSet(pui8ControlTable); // ����DMA�Ŀ��ƽṹ
  MAP_uDMAEnable();

  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
  GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
  GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
  GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);
}

void StartAdcAMP1(int freq, int adcDataLen) {
  int ch = AMP1;
  MAP_ADCSequenceConfigure(adcBase0, adcSeq0, ADC_TRIGGER_TIMER,
                           adcPriority0); // ������ʱ���Զ��������д���
  MAP_ADCSequenceStepConfigure(adcBase0, adcSeq0, 0,
                               ch | ADC_CTL_IE | ADC_CTL_END);
  MAP_ADCSequenceDMAEnable(adcBase0, adcSeq0); // ���ö�������Ӧ��DMA
  MAP_ADCSequenceEnable(adcBase0, adcSeq0); // ���ö�����
  MAP_ADCIntClearEx(adcBase0, adcIntFlag0); // �����жϱ���
  // MAP_ADCIntEnableEx(adcBase0, adcIntFlag0);
  MAP_IntEnable(adcInt0); // ���ö������ж�
  ADCIntRegister(adcBase0, adcSeq0, AdcHandler); // ע�ᶨ�������ж�

  MAP_uDMAChannelAssign(dmaChann0); // ��ADCͨ�����ӵ�DMA��ͨ��
  MAP_uDMAChannelAttributeDisable(
      dmaChann0, UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
                     UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK); // Ĭ���ѽ���

  // �������ݣ����ݴ�СΪ16λ��Դ���ݵ�ַ�����ӣ�Ŀ�����ݵ�ַ����
  MAP_uDMAChannelControlSet(dmaChann0 | UDMA_PRI_SELECT,
                            UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 |
                                dmaArb0);

  ADCClockConfigSet(adcBase0, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL,
                    15); // ADCʱ������

  // ADCʱ������
  if (freq >= 75 * 20000)
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 12);
  else
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 15);

  // ������ʱ��
  MAP_TimerConfigure(timerBase0, TIMER_CFG_A_PERIODIC);
  // MAP_TimerPrescaleSet(timerBase0, TIMER_A, 1);
  MAP_TimerLoadSet(timerBase0, TIMER_A, 120000000 / freq);
  MAP_TimerADCEventSet(timerBase0, TIMER_ADC_TIMEOUT_A);
  MAP_TimerControlTrigger(timerBase0, TIMER_A, true);
  MAP_TimerEnable(timerBase0, TIMER_A);

  if (adcData == adcData1)
    adcData = adcData0;
  else
    adcData = adcData1;

  // ��ʼDMA����
  MAP_uDMAChannelTransferSet(dmaChann0 | UDMA_PRI_SELECT, UDMA_MODE_BASIC,
                             (void *)&ADC0->SSFIFO2, (void *)adcData,
                             adcDataLen);
  MAP_uDMAChannelEnable(dmaChann0);
  MAP_ADCIntEnableEx(adcBase0, adcIntFlag0);

  while (!adc_done)
    ;
}
