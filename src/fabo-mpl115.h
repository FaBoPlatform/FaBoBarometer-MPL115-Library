/**
 * @file  fabo-mpl115.h
 * @brief fabo libtary of MPL115
 * @author Akira Sasaki
 * @date 2,10, 2016
 */
#include "Arduino.h"
#include "Wire.h"

/** SLAVE_ADDRESS Register */
#define MPL115_SLAVE_ADDRESS (0x60)

/** PADC(PASCAL) MSB Register */
#define MPL115_PADC_MSB_REG 0x00
/** TADC(Air Temperature) MSB Register */
#define MPL115_TADC_MSB_REG 0x02
/** A0 MSB Register */
#define MPL115_A0_MSB_REG 0x04

/**
 * @brief MPL115 Control
 */
class FaBoBarometer
{
public:
	int readPADC(void);
	int readTADC(void);
	void readCoefficient(uint16_t *a0, uint16_t *b1, uint16_t *b2, uint16_t *c12);
	float readPressure();
private:
	void writeI2c(byte register_addr, byte value);
	void readI2c(byte register_addr, int num, byte *buf);
};
