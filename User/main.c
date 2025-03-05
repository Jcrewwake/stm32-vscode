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

uint8_t Motor_Runing_Flag = 1;

float Target, Actual, Out;
float Kp = 0.20, Ki = 0.20, Kd = 0;
float Error0, Error1, ErrorInt;

int main(void)
{
    OLED_Init();
    LED_Init();
    Key_Init();
    RP_Init();
    Motor_Init();
    Encoder_Init();
    Serial_Init();

    Timer_Init();

    OLED_Printf(0, 0, OLED_8X16, "Speed Control?");
    OLED_Update();

    while (1) {
        Key_Num = Get_Key_Num();
        if (Key_Num & 0x01) {
            // Target += 10;
            Motor_Runing_Flag = !Motor_Runing_Flag;
        } 
        // else if (Key_Num & 0x02) {
        //     Target -= 10;
        // } else if (Key_Num & 0x04) {
        //     Target = 0;
        // }

        if (Motor_Runing_Flag) {
            LED_ON();
        } else {
            LED_OFF();
        }

        Target = (float)RP_Read(4) / 4096 * 300 - 150;
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
        Serial_Printf("%f,%f,%f,%f\r\n", Target, Actual, Out, ErrorInt);
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
            Actual = (float)Encoder_Get();
            
            Error1 = Error0;
            Error0 = Target - Actual;

            if (fabs(Ki) > FLT_EPSILON) {
                ErrorInt += Error0;
            } else {
                ErrorInt = 0;
            }

            if (ErrorInt > 100 / Ki) {
                ErrorInt = 100 / Ki;
            }else if (ErrorInt < -100 / Ki) {
                ErrorInt = -100 / Ki;
            }

            Out = Kp * Error0 + Ki * ErrorInt + Kd * (Error0 - Error1);

            if (Out > 100) {
                Out = 100;
            } else if (Out < -100) {
                Out = -100;
            }
            if (Motor_Runing_Flag)
                Motor_SetPWM(Out);
            else
                Motor_SetPWM(0);
        }
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
