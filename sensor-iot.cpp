#include "mbed.h"
#include "BME280_SPI.h"
#include "TextLCD.h"

void buttonselect();
void bme();
void display();
void iot();

UnbufferedSerial pc(USBTX, USBRX);

BME280_SPI sensor(D11, D12, D13, D9); // mosi, miso, sclk, cs
#define BLINKING_RATE     300ms

I2C i2c_lcd(D14,D15); // SDA, SCL
TextLCD_I2C_N lcd(&i2c_lcd, ST7032_SA, TextLCD_Base::LCD16x2, NC, TextLCD::ST7032_3V3);  //I2c, Slaveadress, LCD type

InterruptIn button(BUTTON1);
DigitalOut led(LED1);
int select = 0;
float sensout = 0;
float temp = 0;
float pressure = 0;
float humi = 0;

int main()
{
    printf("Program Starting\n");
    lcd.setCursor(TextLCD::CurOn_BlkOn);
    wait_us(10000);
    lcd.setMode(TextLCD::DispOn);
    wait_us(10000);
    lcd.printf("Initializing\n");
    while(1)
    {
        button.rise(&buttonselect);
        printf("select value: %d\n", select);
        bme();
        ThisThread::sleep_for(BLINKING_RATE);
        display();
        printf("main loop completed\n");
        //iot();
    }
}

void buttonselect()
{
    select++;
    if(select == 4)
        select = 0;
}

void bme()
{
    led = !led;
    printf("bme running\n");
    temp = sensor.getTemperature(); //%2.2f degC
    pressure = sensor.getPressure(); //%04.2f hPA
    humi = sensor.getHumidity(); //%2.2f %%
}

void display()
{
    printf("display running\n");
    switch(select)
    {
        case 0 :
            lcd.printf("Temp:%2.2fC\n", temp);//%2.2f degC  F = C * 9/5 + 32
            printf("Temp: %2.2f degC\n", temp);
            break;
        case 1 : 
            lcd.printf("Press:%04.2fhPA\n", pressure); //%04.2f hPA
            printf("Pressure: %04.2f hPA\n", pressure);
            break;
        case 2 : 
            lcd.printf("Humid:%2.2f%%\n", humi); //%2.2f %%
            printf("Humidity: %2.2f %%\n", humi);
            break;
        case 3 : 
            lcd.printf("%2.2fdegC, %04.2fhPa, %2.2f%%\n",sensor.getTemperature(),sensor.getPressure(),sensor.getHumidity());
            printf("%2.2f C, %04.2f hPa, %2.2f %%\n",sensor.getTemperature(),sensor.getPressure(),sensor.getHumidity());
            break;
    }

}

void iot()
{
    printf("iot running\n");
}
