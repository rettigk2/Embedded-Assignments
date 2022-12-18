#include "mbed.h"
#include "BME280_SPI.h"
#include "TextLCD.h"

#define BLINKING_RATE     500ms

UnbufferedSerial pc(USBTX, USBRX);

BME280_SPI sensor(D11, D12, D13, D9); // mosi, miso, sclk, cs

I2C i2c_lcd(D14,D15); // SDA, SCL
TextLCD_I2C_N lcd(&i2c_lcd, ST7032_SA, TextLCD_Base::LCD16x2, NC, 
TextLCD::ST7032_3V3);

InterruptIn button(BUTTON1);
DigitalOut led(LED1);

int select = 0;
int selectm = 0; 
float sensout = 0;
float temp = 0;
float pressure = 0;
float humi = 0;

void buttonselect();
void bme();
void display();
void iot();

int main()
{
    printf("Program Starting\n");
    //lcd.setCursor(TextLCD::CurOn_BlkOn);
    wait_us(5000);
    lcd.setMode(TextLCD::DispOn);
    wait_us(500000);
    lcd.printf("Initializing\n");
    while(1)
    {
        printf("main loop started\n");
        button.rise(&buttonselect);
        if (selectm != select)
        {
            printf("display changed, case: %d\n", select);
            selectm = select;
            ThisThread::sleep_for(BLINKING_RATE);
        }
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
    button.rise(&buttonselect);
    if (selectm != select)
    {
        printf("display changed, case: %d\n", select);
        selectm = select;
        ThisThread::sleep_for(BLINKING_RATE);
    }
    select = select + 1;
    if(select == 4)
        select = 0;
}

void bme()
{
    button.rise(&buttonselect);
    if (selectm != select)
    {
        printf("display changed, case: %d\n", select);
        selectm = select;
        ThisThread::sleep_for(BLINKING_RATE);
    }
    led = !led;
    printf("bme running\n");
    temp = sensor.getTemperature(); //%2.2f degC
    pressure = sensor.getPressure(); //%04.2f hPA
    humi = sensor.getHumidity(); //%2.2f %%
}

void display()
{
    button.rise(&buttonselect);
    if (selectm != select)
    {
        printf("display changed, case: %d\n", select);
        selectm = select;
        ThisThread::sleep_for(BLINKING_RATE);
    }
    printf("display running\n");
    lcd.locate(0, 0); //set cursor row 0, column 0
    switch(select)
    {
        case 0 :
            lcd.printf("Temperature:    %2.2fC    %2.2fF\n", temp, temp*9/5+32);//%2.2f degC  F = C * 9/5 + 32
            printf("Temp: %2.2f C\n", temp);
            break;
        case 1 : 
            lcd.printf("Pressure:              %04.1fhPA\n", pressure); //%04.2f hPA "%2.2fC   %%04.2fhPA\nShowing Press:\n"
            printf("Pressure: %04.2f hPA\n", pressure);
            break;
        case 2 : 
            lcd.printf("Humidity:                 %2.2f%%\n", humi); //%2.2f %% "%2.2fC   %  "%2.2f%%\nHumidity:\n"
            printf("Humidity: %2.2f %%\n", humi);
            break;
        case 3 : 
            lcd.printf("%2.2fC   %2.2fF  %4.0fhPa   %2.1f%%\n", temp, temp*9/5+32,pressure,humi);
            printf("%2.2f C, %04.2f hPa, %2.2f %%\n",sensor.getTemperature(),sensor.getPressure(),sensor.getHumidity());
            break;
    }
    lcd.locate(1, 13); //set cursor row 0, column 0
}

void iot()
{
    button.rise(&buttonselect);
    if (selectm != select)
    {
        printf("display changed, case: %d\n", select);
        selectm = select;
        ThisThread::sleep_for(BLINKING_RATE);
    }
    printf("iot running\n");
}
