#include "stm32f10x.h"

static uint8_t Key_Num = 0;

void Key_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Get_Key_Num(void) {
	uint8_t Temp;
	if (Key_Num) {
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
    return 0;
}

uint8_t Key_Scan(void) {
    uint8_t key = 0;
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 0) {
        key |= 0x01;
    }
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0) {
        key |= 0x02;
    }
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0) {
        key |= 0x04;
    }
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0) {
        key |= 0x08;
    }
    return key;
}

void Key_Tick(void) {
    static uint8_t Count;
    static uint8_t Current, PrevState;

    Count++;
    if (Count >= 20) {
        Count = 0;

        PrevState = Current;
        Current = Key_Scan();

        if (((Current & 0x01) == 0 && (PrevState & 0x01) != 0) ||
            ((Current & 0x02) == 0 && (PrevState & 0x02) != 0) ||
            ((Current & 0x04) == 0 && (PrevState & 0x04) != 0) ||
            ((Current & 0x08) == 0 && (PrevState & 0x08) != 0)) {
            // Key release event
            Key_Num = PrevState;
        }
    }
}