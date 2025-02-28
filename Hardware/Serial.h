#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>

void Serial_Init(void);
void Serial_SendByte(uint8_t data);
void Serial_SendArray(uint8_t *data, uint16_t len);
void Serial_SendString(char *str);
void Serial_SendNumber(uint32_t num, uint8_t len);
void Serial_Printf(char *fmt, ...);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

#endif
