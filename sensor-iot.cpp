#include "mbed.h"
#include "BME280_SPI.h"
#include "TextLCD.h"

void buttonselect();
void bme();
void display();
void iot();

BME280_SPI sensor(D11, D12, D13, D9); // mosi, miso, sclk, cs
#define BLINKING_RATE     500ms

I2C i2c_lcd(D14,D15); // SDA, SCL
TextLCD_I2C_N lcd(&i2c_lcd, ST7032_SA, TextLCD_Base::LCD16x2, NC, TextLCD::ST7032_3V3);  //I2c, Slaveadress, LCD type

InterruptIn button(BUTTON1);
DigitalOut led(LED1);
int select = 0;
float sensout = 0;


int main()
{
    printf("debug0\n");
    lcd.setCursor(TextLCD::CurOn_BlkOn);
    wait_us(10000);
    lcd.setMode(TextLCD::DispOn);
    wait_us(10000);
    lcd.printf("Initializing\n");

    while(1)
    {
        button.rise(&buttonselect);
        bme();
        ThisThread::sleep_for(BLINKING_RATE);
        display();
        ThisThread::sleep_for(BLINKING_RATE);   
        printf("main loop completed\n");
        //iot();
    }
}

void buttonselect()
{
    select++;
    if(select == 3)
        select = 0;
    printf("button pressed\n");
}

void bme()
{
    led = !led;
    switch(select) 
    {
        case 0 : sensout = sensor.getTemperature(); //%2.2f degC
            break;
        case 1 : sensout = sensor.getPressure(); //%04.2f hPA
            break;
        case 2 : sensout = sensor.getHumidity(); //%2.2f %%
            break;
    }
    printf("bme running\n");
}

void display()
{
    switch(select)
    {
        case 0 : lcd.printf("Temp: %2.2f degC\n", sensor.getTemperature());//%2.2f degC
            break;
        case 1 : lcd.printf("Temp: %04.2f hPA\n", sensor.getPressure()); //%04.2f hPA
            break;
        case 2 : lcd.printf("Temp: %2.2f %%\n", sensor.getHumidity()); //%2.2f %%
            break;
        case 3 : lcd.printf("%2.2f degC, %04.2f hPa, %2.2f %%\n",sensor.getTemperature(),sensor.getPressure(),sensor.getHumidity());
            break;
    }
    printf("display running\n");
}

void iot()
{
    printf("display running\n");
}
