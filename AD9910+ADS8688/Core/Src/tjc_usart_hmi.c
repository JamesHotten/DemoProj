/**
 * �ļ�˵����
 * 1. ���ļ�Ϊ tjc_usart_hmi.c �� tjc_usart_hmi.h ��ʵ�ֲ��֡�
 * 2. ʹ��ʱ��Ҫ����ͷ�ļ� #include "tjc_usart_hmi.h"��
 * 3. ʹ��ǰ��ȷ�� HAL_UART_Transmit_IT() ����ȷ����Ϊ�жϷ�ʽ�������ݡ�
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include <stdio.h>
#include "tjc_usart_hmi.h"

// ���廷�λ������Ĵ�С
#define RINGBUFFER_LEN 128

typedef struct {
	uint16_t Head;          // ���λ�������ͷ��ָ��
	uint16_t Tail;          // ���λ�������β��ָ��
	uint16_t Length;        // ���λ����������ݵĳ���
	uint8_t Ring_data[RINGBUFFER_LEN]; // ���λ��������ݴ洢
} RingBuffer_t;

RingBuffer_t ringBuffer;    // ����һ�����λ�����ʵ��
uint8_t RxBuffer[1];        // ���ջ�����

/********************************************************
 ��������       : intToStr
 ��������     : 2024.09.18
 ��������     : ������ת��Ϊ�ַ���
 �������     : Ҫת����������Ŀ���ַ���
 ����ֵ       : ��
 �޸ļ�¼��
 **********************************************************/
void intToStr(int num, char *str) {
	int i = 0;
	int isNegative = 0;

	// ����Ƿ�Ϊ����
	if (num < 0) {
		isNegative = 1;
		num = -num;
	}

	// ��ȡÿһλ����
	do {
		str[i++] = (num % 10) + '0';
		num /= 10;
	} while (num);

	// ����Ǹ�������Ӹ���
	if (isNegative) {
		str[i++] = '-';
	}

	// ����ַ���������
	str[i] = '\0';

	// ��ת�ַ���
	int start = 0;
	int end = i - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
	return;
}

/********************************************************
 ��������       : uart_send_char
 ��������     : 2024.09.18
 ��������     : ����һ���ַ�
 �������     : Ҫ���͵��ַ�
 ����ֵ       : ��
 �޸ļ�¼��
 **********************************************************/
void uart_send_char(char ch) {
	uint8_t ch2 = (uint8_t) ch;
	// �ȴ��������
	while (__HAL_UART_GET_FLAG(&TJC_UART, UART_FLAG_TC) == RESET)
		;
	// �����ַ�
	HAL_UART_Transmit_IT(&TJC_UART, &ch2, 1);
	return;
}

/********************************************************
 ��������       : uart_send_string
 ��������     : 2024.09.18
 ��������     : ����һ���ַ���
 �������     : Ҫ���͵��ַ���
 ����ֵ       : ��
 �޸ļ�¼��
 **********************************************************/
void uart_send_string(char *str) {
	// �����ַ���ֱ�������ַ���������
	while (*str != 0 && str != 0) {
		// ����ַ�����
		uart_send_char(*str++);
	}
	return;
}

/********************************************************
 ��������       : tjc_send_string
 ��������     : 2024.09.18
 ��������     : �����ַ�������ӽ�����־
 �������     : Ҫ���͵��ַ���
 ����ֵ       : ��
 ʾ����         tjc_send_string("n0.val=100"); �����ַ��� n0.val=100 ����ӽ�����־
 �޸ļ�¼��
 **********************************************************/
void tjc_send_string(char *str) {
	// �����ַ���ֱ�������ַ���������
	while (*str != 0 && str != 0) {
		// ����ַ�����
		uart_send_char(*str++);
	}
	// ��ӽ�����־
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

/********************************************************
 ��������       : tjc_send_txt
 ��������     : 2024.09.18
 ��������     : �����ı�����ֵ
 �������     : �������ƣ��������ƣ��ı�����
 ����ֵ       : ��
 ʾ����         tjc_send_txt("t0", "txt", "ABC"); ���� t0.txt="ABC"
 �޸ļ�¼��
 **********************************************************/
void tjc_send_txt(char *objname, char *attribute, char *txt) {
	// ���Ͷ�������
	uart_send_string(objname);
	uart_send_char('.');
	// ������������
	uart_send_string(attribute);
	uart_send_string("=\"");
	// �����ı�����
	uart_send_string(txt);
	uart_send_char('\"');
	// ��ӽ�����־
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

/********************************************************
 ��������       : tjc_send_val
 ��������     : 2024.09.18
 ��������     : ������ֵ����ֵ
 �������     : �������ƣ��������ƣ���ֵ
 ����ֵ       : ��
 ʾ����         tjc_send_val("n0", "val", 100); ���� n0.val=100
 �޸ļ�¼��
 **********************************************************/
void tjc_send_val(char *objname, char *attribute, int val) {
	// ���Ͷ�������
	uart_send_string(objname);
	uart_send_char('.');
	// ������������
	uart_send_string(attribute);
	uart_send_char('=');
	// ����ֵת��Ϊ�ַ���
	char txt[12] = "";
	intToStr(val, txt);
	// ������ֵ�ַ���
	uart_send_string(txt);
	// ��ӽ�����־
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

/********************************************************
 ��������       : tjc_send_nstring
 ��������     : 2024.09.18
 ��������     : ����ָ�����ȵ��ַ���
 �������     : Ҫ���͵��ַ������ַ�������
 ����ֵ       : ��
 �޸ļ�¼��
 **********************************************************/
void tjc_send_nstring(char *str, unsigned char str_length) {
	// �����ַ���ֱ���ﵽָ������
	for (int var = 0; var < str_length; ++var) {
		// ����ַ�����
		uart_send_char(*str++);
	}
	// ��ӽ�����־
	uart_send_char(0xff);
	uart_send_char(0xff);
	uart_send_char(0xff);
	return;
}

/********************************************************
 ��������       : HAL_UART_RxCpltCallback
 ��������     : 2022.10.08
 ��������     : UART������ɻص�����
 �������     : UART���
 ����ֵ       : void
 �޸ļ�¼��
 **********************************************************/
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if(huart->Instance == TJC_UART_INS) // �ж��Ƿ�Ϊָ����UARTʵ��
//     {
//         // �����յ�������д�뻷�λ�����
//         write1ByteToRingBuffer(RxBuffer[0]);
//         // ����������һ���ֽ�
//         HAL_UART_Receive_IT(&TJC_UART, RxBuffer, 1);
//     }
//     return;
// }
/********************************************************
 ��������       : initRingBuffer
 ��������     : 2022.10.08
 ��������     : ��ʼ�����λ�����
 �������     : ��
 ����ֵ       : void
 �޸ļ�¼��
 **********************************************************/
void initRingBuffer(void) {
	// ��ʼ�����λ�������Ϣ
	ringBuffer.Head = 0;
	ringBuffer.Tail = 0;
	ringBuffer.Length = 0;
	return;
}

/********************************************************
 ��������       : write1ByteToRingBuffer
 ��������     : 2022.10.08
 ��������     : ���λ�����д��һ���ֽ�
 �������     : Ҫд�������
 ����ֵ       : void
 �޸ļ�¼��
 **********************************************************/
void write1ByteToRingBuffer(uint8_t data) {
	if (ringBuffer.Length >= RINGBUFFER_LEN) // �жϻ��λ������Ƿ�����
	{
		return; // ���������ֱ�ӷ���
	}
}
