#include "mbed.h"
#include "BME280_SPI.h"
#include "TextLCD.h"
#include "EthernetInterface.h"
#include "SocketAddress.h"
#include "http_request.h"
#include <string>

#define BLINKING_RATE     500ms

EthernetInterface eth0;
char ip[] ="192.168.2.101";
char mask[]= "255.255.255.0";
char gateway[] = "192.168.2.1";

BME280_SPI sensor(D11, D12, D13, D9); // mosi, miso, sclk, cs
I2C i2c_lcd(D14,D15); // SDA, SCL
TextLCD_I2C_N lcd(&i2c_lcd, ST7032_SA, TextLCD_Base::LCD16x2, NC, 
TextLCD::ST7032_3V3);

InterruptIn button(BUTTON1);
DigitalOut led(LED1);

int select = 0;
int selectm = 0; 
int cnt = 10;

string stationID("Station101");
float temp = 0;
float pres = 0;
float hum = 0;
int Verbose = 1;
//char url[] =  "http://192.168.2.100/log/station101/%2.2f/%04.2f/%2.2f";

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
        if (cnt == 50)
        {
            select = 4;
            display();
            iot();
            cnt = 0;
        }
        cnt = cnt+1;
    }
}

void buttonselect()
{
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
    pres = sensor.getPressure(); //%04.2f hPA
    hum = sensor.getHumidity(); //%2.2f %%
}

void display()
{
    printf("display running\n");
    lcd.locate(0, 0);
    switch(select)
    {
        case 0 :
            lcd.printf("Temperature:    %2.2fC    %2.2fF\n", temp, temp*9/5+32);//%2.2f degC  F = C * 9/5 + 32
            printf("Temperature: %2.2f C\n", temp);
            break;
        case 1 : 
            if (pres >= 1000)
                lcd.printf("Pressure:              %04.1fhPA\n", pres); //%04.2f hPA "%2.2fC   %%04.2fhPA\nShowing Press:\n"
            else 
                lcd.printf("Pressure:              %04.1fhPA \n", pres); //%04.2f hPA "%2.2fC   %%04.2fhPA\nShowing Press:\n"
            printf("Pressure: %04.2f hPA\n", pres);
            break;
        case 2 : 
            lcd.printf("Humidity:                 %2.2f%%\n", hum); //%2.2f %% "%2.2fC   %  "%2.2f%%\nHumidity:\n"
            printf("Humidity: %2.2f %%\n", hum);
            break;
        case 3 : 
            if (pres >= 1000)
                lcd.printf("%2.2fC   %2.2fF  %4.0fhPa   %2.1f%%\n", temp, temp*9/5+32,pres,hum);
            else
                lcd.printf("%2.2fC   %2.2fF  %4.0fhPa   %2.1f%%\n", temp, temp*9/5+32,pres,hum);
            printf("%2.2f C, %04.2f hPa, %2.2f %%\n",sensor.getTemperature(),sensor.getPressure(),sensor.getHumidity());
            break;
        case 4 : 
            if (selectm == 0)
            {
                lcd.locate(12, 0);
                lcd.printf(" IoT");
                select = selectm;
            }
            else
            {
                lcd.locate(9, 0);
                lcd.printf("    IoT");
                select = selectm;
            }
            
            break;
    }
    lcd.locate(15, 0);
    button.rise(&buttonselect);
    if (selectm != select)
    {
        printf("display changed, case: %d\n", select);
        selectm = select;
        ThisThread::sleep_for(BLINKING_RATE);
    }
}

void iot()
{
    printf("iot running\n");
    
    eth0.set_network( SocketAddress(ip), SocketAddress(mask), SocketAddress(gateway));
    eth0.connect();

    HttpRequest* post_req = new HttpRequest(&eth0, HTTP_POST, "http://192.168.2.101/",NULL);
    post_req->set_header("Content-Type", "text/xml");
    const char body[] = "{\"hello\":\"world\"}"; //i cant figure this out
    HttpResponse* post_res = post_req->send(body, strlen(body));

//printf causes memory errors:
    //printf("status is %d - %s\n", response->get_status_code(), response->get_status_message().c_str());
    //printf("body is:\n%s\n", response->get_body_as_string().c_str());
    
    delete post_req;
}

