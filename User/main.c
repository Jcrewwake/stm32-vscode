#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"

uint16_t times;
int main(void)
{
	OLED_Init();
    Key_Init();
    LED_Init();
    Timer_Init();

    LED_ON();
    OLED_ShowString(0, 0, "Hello, world!", OLED_6X8);
    OLED_Update();
    while (1) {

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
