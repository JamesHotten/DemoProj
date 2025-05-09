#include "i2c5351.h"
#include "si5351.h"
#include "ti/driverlib/m0p/dl_core.h"


#ifdef __CC_ARM
#pragma O0
#elif defined(__GNUC__)
#pragma GCC optimize("O0")
#elif defined(__clang__)
#pragma clang optimize off
#else
// adding ur own compiler controlling pragmas
#endif
void delay_ms(unsigned long ms) {
  while (ms--)
    delay_cycles(CPUCLK_FREQ / 1000);
}

// 起始信号
void I2C_Start(void) {
  Si5351_SDA_Set();
  Si5351_SCL_Set();

  Si5351_SDA_Clr();
  Si5351_SCL_Clr();
}

// 结束信号
void I2C_Stop(void) {
  Si5351_SDA_Clr();
  Si5351_SCL_Set();

  Si5351_SDA_Set();
}

// 等待信号响应
void I2C_WaitAck(void) // 测数据信号的电平
{
  Si5351_SDA_Set();

  Si5351_SCL_Set();

  Si5351_SCL_Clr();
}

// 写入一个字节
void Send_Byte(uint8_t dat) {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    Si5351_SCL_Clr(); // 将时钟信号设置为低电平
    if (dat & 0x80) // 将dat的8位从最高位依次写入
    {
      Si5351_SDA_Set();
    } else {
      Si5351_SDA_Clr();
    }

    Si5351_SCL_Set();

    Si5351_SCL_Clr();
    dat <<= 1;
  }
}

void I2C_sendREG(uint8_t REG_Address, uint8_t REG_data) {
  I2C_Start();
  Send_Byte(0xC0);
  I2C_WaitAck();
  Send_Byte(REG_Address);
  I2C_WaitAck();
  Send_Byte(REG_data);
  I2C_WaitAck();
  I2C_Stop();
  // if (!I2C2_Start()) return false;
  // I2C2_Send_Byte(0xC0);
  // if (!I2C2_Wait_Ack()) {
  //   I2C2_Stop();
  //   return false;
  // }
  // I2C2_Send_Byte(REG_Address);
  // if (!I2C2_Wait_Ack()) {
  //   I2C2_Stop();
  //   return false;
  // }
  // I2C2_Send_Byte(REG_data);
  // if (!I2C2_Wait_Ack()) {
  //   I2C2_Stop();
  //   return false;
  // }
  // if (!I2C2_Stop())
  //   return false;
  // return true;
}

void Si5351_Init(void) {
  for (int i = 0; i < SI5351A_REVB_REG_CONFIG_NUM_REGS; i++)
    I2C_sendREG(si5351a_revb_registers[i].address,
                si5351a_revb_registers[i].value);
}