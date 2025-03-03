#include "stm32f10x.h"                  // Device header
#include <stdint.h>
#include <math.h>
#include <float.h>
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"
#include "RP.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"

uint8_t Key_Num;

float Target, Actual, Out;
float Kp = 0.20, Ki = 0.20, Kd = 0;
float Error0, Error1, Error2;

int main(void)
{
    OLED_Init();
    Key_Init();
    RP_Init();
    Motor_Init();
    Encoder_Init();
    Serial_Init();

    Timer_Init();

    OLED_Printf(0, 0, OLED_8X16, "Location Control");
    OLED_Update();

    while (1) {
        // Key_Num = Get_Key_Num();
        // if (Key_Num & 0x01) {
        //     Target += 10;
        // } else if (Key_Num & 0x02) {
        //     Target -= 10;
        // } else if (Key_Num & 0x04) {
        //     Target = 0;
        // }
        Target = (float)RP_Read(4) / 4096 * 816 - 408;
        Kp = (float)RP_Read(1) / 4096 * 2;
        Ki = (float)RP_Read(2) / 4096 * 2;
        Kd = (float)RP_Read(3) / 4096 * 2;
        OLED_Printf(0, 16, OLED_8X16, "Kp:%4.2f", Kp);
        OLED_Printf(0, 32, OLED_8X16, "Ki:%4.2f", Ki);
        OLED_Printf(0, 48, OLED_8X16, "Kd:%4.2f", Kd);
        OLED_Printf(64, 16, OLED_8X16, "Tar:%+04.0f", Target);
        OLED_Printf(64, 32, OLED_8X16, "Act:%+04.0f", Actual);
        OLED_Printf(64, 48, OLED_8X16, "Out:%+04.0f", Out);
        // OLED_ShowFloatNum(0, 16, Target, 3, 0, OLED_8X16);
        OLED_Update();
        Serial_Printf("%f,%f,%f\r\n", Target, Actual, Out);
    }
}

void TIM1_UP_IRQHandler(void)
{
    static uint16_t cnt = 0;
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        Key_Tick();

        cnt++;
        if (cnt >= 40) {
            cnt = 0;
            Actual += (float)Encoder_Get();
            
            Error2 = Error1;
            Error1 = Error0;
            Error0 = Target - Actual;

            Out += Kp * (Error0 - Error1) + Ki * Error0 + Kd * (Error0 - 2*Error1 + Error2);

            if (Out > 100) {
                Out = 100;
            } else if (Out < -100) {
                Out = -100;
            }
            Motor_SetPWM(Out);

        }
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
