/**
 ******************************************************************************
 * @file    BME280_SPI.cpp
 * @author  Toyomasa Watarai
 * @version V1.0.0
 * @date    11 March 2017
 * @brief   BME280_SPI class implementation
 ******************************************************************************
 * @attention
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "BME280_SPI.h"

BME280_SPI::BME280_SPI(PinName mosi, PinName miso, PinName sclk, PinName cs)
    :
    _spi(mosi, miso, sclk),
    _cs(cs),
    t_fine(0)
{
    initialize();
}


BME280_SPI::~BME280_SPI()
{
}

void BME280_SPI::initialize()
{
    char cmd[18];

    _cs = 1;
    _spi.format(8, 0); // 8-bit, mode=0
    _spi.frequency(1000000); // 1MHZ

    _cs = 0;
    _spi.write(0xd0); // chip_id
    cmd[0] = _spi.write(0); // read chip_id
    _cs = 1;
    
    DEBUG_PRINT("chip_id = 0x%x\n", cmd[0]);

    _cs = 0;
    _spi.write(0xf2 & BME280_SPI_MASK); // ctrl_hum
    _spi.write(0x04); // Humidity oversampling x4
    _cs = 1;

    _cs = 0;
    _spi.write(0xf4 & BME280_SPI_MASK); // ctrl_meas
    _spi.write((4<<5)|(4<<2)|3); // Temparature oversampling x4, Pressure oversampling x4, Normal mode
    _cs = 1;

    _cs = 0;
    _spi.write(0xf5 & BME280_SPI_MASK); // config
    _spi.write(0xa0); // Standby 1000ms, Filter off, 4-wire SPI interface
    _cs = 1;

    wait_us(1000000);
    
    _cs = 0;
    _spi.write(0x88); // read dig_T regs
    for(int i = 0; i < 6; i++)
        cmd[i] = _spi.write(0);
    _cs = 1;

    dig_T1 = (cmd[1] << 8) | cmd[0];
    dig_T2 = (cmd[3] << 8) | cmd[2];
    dig_T3 = (cmd[5] << 8) | cmd[4];

    DEBUG_PRINT("dig_T = 0x%x, 0x%x, 0x%x\n", dig_T1, dig_T2, dig_T3);
    DEBUG_PRINT("dig_T = %d, %d, %d\n", dig_T1, dig_T2, dig_T3);

    _cs = 0;
    _spi.write(0x8e); // read dig_P regs
    for(int i = 0; i < 18; i++)
        cmd[i] = _spi.write(0);
    _cs = 1;

    dig_P1 = (cmd[ 1] << 8) | cmd[ 0];
    dig_P2 = (cmd[ 3] << 8) | cmd[ 2];
    dig_P3 = (cmd[ 5] << 8) | cmd[ 4];
    dig_P4 = (cmd[ 7] << 8) | cmd[ 6];
    dig_P5 = (cmd[ 9] << 8) | cmd[ 8];
    dig_P6 = (cmd[11] << 8) | cmd[10];
    dig_P7 = (cmd[13] << 8) | cmd[12];
    dig_P8 = (cmd[15] << 8) | cmd[14];
    dig_P9 = (cmd[17] << 8) | cmd[16];

    DEBUG_PRINT("dig_P = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9);
    DEBUG_PRINT("dig_P = %d, %d, %d, %d, %d, %d, %d, %d, %d\n", dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9);

    _cs = 0;
    _spi.write(0xA1); // read dig_H1 reg
    cmd[0] = _spi.write(0);
    _cs = 1;

    _cs = 0;
    _spi.write(0xE1); // read dig_H regs
    for(int i = 0; i < 7; i++)
        cmd[1+i] = _spi.write(0);
    _cs = 1;

    dig_H1 = cmd[0];
    dig_H2 = (cmd[2] << 8) | cmd[1];
    dig_H3 = cmd[3];
    dig_H4 = (cmd[4] << 4) | (cmd[5] & 0x0f);
    dig_H5 = (cmd[6] << 4) | ((cmd[5]>>4) & 0x0f);
    dig_H6 = cmd[7];

    DEBUG_PRINT("dig_H = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", dig_H1, dig_H2, dig_H3, dig_H4, dig_H5, dig_H6);
    DEBUG_PRINT("dig_H = %d, %d, %d, %d, %d, %d\n", dig_H1, dig_H2, dig_H3, dig_H4, dig_H5, dig_H6);
}

float BME280_SPI::getTemperature()
{
    uint32_t temp_raw;
    float tempf;
    char cmd[3];

    _cs = 0;
    _spi.write(0xfa);
    for(int i = 0; i < 3; i++)
        cmd[i] = _spi.write(0);
    _cs = 1;

    temp_raw = (cmd[0] << 12) | (cmd[1] << 4) | (cmd[2] >> 4);

    int32_t temp;

    temp =
        (((((temp_raw >> 3) - (dig_T1 << 1))) * dig_T2) >> 11) +
        ((((((temp_raw >> 4) - dig_T1) * ((temp_raw >> 4) - dig_T1)) >> 12) * dig_T3) >> 14);

    t_fine = temp;
    temp = (temp * 5 + 128) >> 8;
    tempf = (float)temp;

    return (tempf/100.0f);
}

float BME280_SPI::getPressure()
{
    uint32_t press_raw;
    float pressf;
    char cmd[3];

    _cs = 0;
    _spi.write(0xf7); // press_msb
    for(int i = 0; i < 3; i++)
        cmd[i] = _spi.write(0);
    _cs = 1;

    press_raw = (cmd[0] << 12) | (cmd[1] << 4) | (cmd[2] >> 4);

    int32_t var1, var2;
    uint32_t press;

    var1 = (t_fine >> 1) - 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * dig_P6;
    var2 = var2 + ((var1 * dig_P5) << 1);
    var2 = (var2 >> 2) + (dig_P4 << 16);
    var1 = (((dig_P3 * (((var1 >> 2)*(var1 >> 2)) >> 13)) >> 3) + ((dig_P2 * var1) >> 1)) >> 18;
    var1 = ((32768 + var1) * dig_P1) >> 15;
    if (var1 == 0) {
        return 0;
    }
    press = (((1048576 - press_raw) - (var2 >> 12))) * 3125;
    if(press < 0x80000000) {
        press = (press << 1) / var1;
    } else {
        press = (press / var1) * 2;
    }
    var1 = ((int32_t)dig_P9 * ((int32_t)(((press >> 3) * (press >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(press >> 2)) * (int32_t)dig_P8) >> 13;
    press = (press + ((var1 + var2 + dig_P7) >> 4));

    pressf = (float)press;
    return (pressf/100.0f);
}

float BME280_SPI::getHumidity()
{
    uint32_t hum_raw;
    float humf;
    char cmd[2];

    _cs = 0;
    _spi.write(0xfd); // hum_msb
    for(int i = 0; i < 2; i++)
        cmd[i] = _spi.write(0);
    _cs = 1;

    hum_raw = (cmd[0] << 8) | cmd[1];

    int32_t v_x1;

    v_x1 = t_fine - 76800;
    v_x1 =  (((((hum_raw << 14) -(((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1)) +
               ((int32_t)16384)) >> 15) * (((((((v_x1 * (int32_t)dig_H6) >> 10) *
                                            (((v_x1 * ((int32_t)dig_H3)) >> 11) + 32768)) >> 10) + 2097152) *
                                            (int32_t)dig_H2 + 8192) >> 14));
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * (int32_t)dig_H1) >> 4));
    v_x1 = (v_x1 < 0 ? 0 : v_x1);
    v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);

    humf = (float)(v_x1 >> 12);

    return (humf/1024.0f);
}
