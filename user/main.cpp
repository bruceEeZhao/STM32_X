/*
file   : *.cpp
author : shentq
version: V1.1
date   : 2016/03/26

Copyright 2016 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ultrasonic_wave.h"

#include "wstring.h"
String s="shentqlf";

float value;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("ok \r\n");
    ultra.begin();

    s += "fuck";
    uart1.printf(s.c_str());
}
int main(void)
{
    setup();
    while(1)
    {       
        value = ultra.detect();
        uart1.printf("value = %0.2fcm\r\n",value);
		delay_ms(1000);
    }

}


