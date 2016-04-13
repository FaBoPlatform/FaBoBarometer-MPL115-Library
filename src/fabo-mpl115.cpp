/**
 * @file  fabo-mpl115.cpp
 * @brief fabo libtary of MPL115
 * @author Akira Sasaki
 * @date 2,10, 2016
 */
#include "fabo-mpl115.h"

/**
 * @brief Read PADC
 * @return int : PADC Value
 */
int FaBoBarometer::readPADC()
{
  byte buffer[2];
  readI2c(MPL115_PADC_MSB_REG, 2, buffer);
  int pressure_value = buffer[0]<<2;
  pressure_value |= buffer[1];

  return pressure_value;
}

/**
 * @brief Read TADC
 * @return int : TADC Value
 */
int FaBoBarometer::readTADC()
{
  byte buffer[2];
  readI2c(MPL115_TADC_MSB_REG, 2, buffer);
  int temperature_value = buffer[0]<<2;
  temperature_value |= buffer[1];

  return temperature_value;
}

/**
 * @brief Read Coefficient
 * @param [out] *a0  : pressure offset coefficient
 * @param [out] *b1  : pressure sensitivity coefficient
 * @param [out] *b2  : pressure linearity (2nd order) coefficient
 * @param [out] *c12 : coefficient for temperature sensitivity coefficient
 */
void FaBoBarometer::readCoefficient(uint16_t *a0, uint16_t *b1, uint16_t *b2, uint16_t *c12)
{
  byte buffer[8];
  readI2c(MPL115_A0_MSB_REG, 8, buffer);
  *a0 = (((int)buffer[0]) << 8) | buffer[1];
  *b1 = (((int)buffer[2]) << 8) | buffer[3];
  *b2 = (((int)buffer[4]) << 8) | buffer[5];
  *c12 = (((int)buffer[6]) << 8) | buffer[7];
}

/**
 * @brief Read Pressure
 * @return float : Pressure Value
 */
float FaBoBarometer::readPressure(){
  int padc = readPADC();
  int tadc = readTADC();

  uint16_t a0, b1, b2, c12;
  readCoefficient(&a0, &b1, &b2, &c12);

  float pcomp = a0 + (b1 + c12 * tadc) * padc + b2 * tadc;
  float pressure = pcomp*((float)(115-50)/1023) + 50;

  return pressure;
}

/**
 * @brief Write I2C Data
 * @param [in] register_addr : Write Register Address
 * @param [in] value  : Write Data
 */
void FaBoBarometer::writeI2c(byte register_addr, byte value) {
  Wire.begin();       // I2C Start
  Wire.beginTransmission(MPL115_SLAVE_ADDRESS);
  Wire.write(register_addr);
  Wire.write(value);
  Wire.endTransmission();
}

/**
 * @brief Read I2C Data
 * @param [in] register_addr : register address
 * @param [in] num   : Data Length
 * @param [out] *buf : Read Data
 */
void FaBoBarometer::readI2c(byte register_addr, int num, byte buffer[]) {
  Wire.begin();       // I2C Start
  Wire.beginTransmission(MPL115_SLAVE_ADDRESS);
  Wire.write(register_addr);
  Wire.endTransmission();

  Wire.beginTransmission(MPL115_SLAVE_ADDRESS);
  Wire.requestFrom(MPL115_SLAVE_ADDRESS, num);

  int i = 0;
  while(Wire.available())
  {
    buffer[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
}
