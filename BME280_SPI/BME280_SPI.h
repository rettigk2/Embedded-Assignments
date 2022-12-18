/**
 ******************************************************************************
 * @file    BME280_SPI.h
 * @author  Toyomasa Watarai
 * @version V1.0.0
 * @date    11 March 2017
 * @brief   This file contains the class of a BME280 Combined humidity and pressure sensor library with SPI interface
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

/**
 *  Library for "BME280 temperature, humidity and pressure sensor module" from Switch Science
 *    https://www.switch-science.com/catalog/2236/
 *
 *  For more information about the BME280:
 *    https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS001-11.pdf
*/

#ifndef MBED_BME280_SPI_H
#define MBED_BME280_SPI_H

#include "mbed.h"

#ifdef _DEBUG
extern Serial pc;
#define DEBUG_PRINT(...) pc.printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

/**  Interface for controlling BME280 Combined humidity and pressure sensor
 *
 * @code
 * #include "mbed.h"
 * #include "BME280_SPI.h"
 *
 * Serial pc(USBTX, USBRX);
 * BME280_SPI sensor(D11, D12, D13, D9); // mosi, miso, sclk, cs
 *
 * int main() {
 *
 *     while(1) {
 *         pc.printf("%2.2f degC, %04.2f hPa, %2.2f %%\n", sensor.getTemperature(), sensor.getPressure(), sensor.getHumidity());
 *         wait(1);
 *     }
 * }
 *
 * @endcode
 */

/** BME280_SPI class
 *
 *  BME280_SPI: A library to correct environmental data using Boshe BME280 environmental sensor device
 *
 */
class BME280_SPI
{
public:

    enum spi_mask {
        BME280_SPI_MASK = 0x7F
    };

    /** Create a BME280 instance
     *  which is connected to specified SPI pins
     *
     * @param mosi SPI MOSI pin
     * @param miso SPI MISO pin
     * @param sclk SPI SCLK pin
     * @param cs device CS pin
     */
    BME280_SPI(PinName mosi, PinName miso, PinName sclk, PinName cs);

    /** Destructor of BME280_SPI
     */
    virtual ~BME280_SPI();

    /** Initializa BME280 sensor
     *
     *  Configure sensor setting and read parameters for calibration
     *
     */
    void initialize(void);

    /** Read the current temperature value (degree Celsius) from BME280 sensor
     *
     * @return Temperature value (degree Celsius)
     */
    float getTemperature(void);

    /** Read the current pressure value (hectopascal) from BME280 sensor
     *
     * @return Pressure value (hectopascal)
     */
    float getPressure(void);

    /** Read the current humidity value (humidity %) from BME280 sensor
     *
     * @return Humidity value (humidity %)
     */
    float getHumidity(void);

private:

    SPI         _spi;
    DigitalOut  _cs;
    uint16_t    dig_T1;
    int16_t     dig_T2, dig_T3;
    uint16_t    dig_P1;
    int16_t     dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
    uint16_t    dig_H1, dig_H3;
    int16_t     dig_H2, dig_H4, dig_H5, dig_H6;
    int32_t     t_fine;

};

#endif // MBED_BME280_SPI_H
