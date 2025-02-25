#include "stm32f10x.h"                  // Device header
// #include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"
#include "RP.h"

uint16_t times;
int main(void)
{
	OLED_Init();
    Key_Init();
    LED_Init();
    Timer_Init();
    RP_Init();

    LED_ON();
    while (1) {
        if (times % 100 == 0) {
            OLED_Printf(0, 0, OLED_8X16, "RP1:%4d", RP_Read(1));
            OLED_Printf(0, 16, OLED_8X16, "RP2:%4d", RP_Read(2));
            OLED_Printf(0, 32, OLED_8X16, "RP3:%4d", RP_Read(3));
            OLED_Printf(0, 48, OLED_8X16, "RP4:%4d", RP_Read(4));
            OLED_Update();
        }
    }
}

void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        Key_Tick();
        times++;
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
