#include "stm32f10x.h"                  // Device header
#include <stdint.h>
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"
#include "RP.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"

int main(void)
{
    OLED_Init();
    RP_Init();
    Serial_Init();
    while (1) {
        uint16_t rp1 = RP_Read(1);
        uint16_t rp2 = RP_Read(2);
        uint16_t rp3 = RP_Read(3);
        uint16_t rp4 = RP_Read(4);
        OLED_Printf(0, 0, OLED_8X16, "Rp1: %+05d", rp1);
        OLED_Printf(0, 16, OLED_8X16, "Rp2: %+05d", rp2);
        OLED_Printf(0, 32, OLED_8X16, "Rp3: %+05d", rp3);
        OLED_Printf(0, 48, OLED_8X16, "Rp4: %+05d", rp4);
        OLED_Update();
        Serial_Printf("%d, %d, %d, %d\n", rp1, rp2, rp3, rp4);
    }
}

void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
