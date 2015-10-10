// 
// 
// 

#include "MAX72XX.h"

void MAX72XXClass::init()
{
	//setup pin
	pinMode(LoadPin, OUTPUT);
	
	//clear data registers
	for (uint8_t i = 0; i < MATRIX_DIM; i++) MAXData[i] = 0;
	
	//send data
	sendData(MAXData);		

	//set decode mode
	Transmit(MAX72XX_REG_DECODEMODE, DecodeMode);

	//set intensity
	Transmit(MAX72XX_REG_INTENSITY, Intensity);

	//set scan limit
	Transmit(MAX72XX_REG_SCANLIMIT, ScanLimit);

	//set shutdown mode
	Transmit(MAX72XX_REG_SHUTDOWN, ShutdownMode ? MAX72XX_SDMODE_SHUTDOWN : MAX72XX_SDMODE_NORMAL);

	//set display test mode
	Transmit(MAX72XX_REG_DISPLAYTEST, TestMode ? MAX72XX_TESTMODE_TEST : MAX72XX_TESTMODE_NORMAL);
}

void MAX72XXClass::Transmit(uint8_t address, uint8_t data)
{
	SPI.beginTransaction(SPISettings(SPIClock, MSBFIRST, SPI_MODE0));

	//clear CS/LOAD pin
	digitalWrite(LoadPin, LOW);

	SPI.transfer(address);
	SPI.transfer(data);

	//set CS/LOAD pin
	digitalWrite(LoadPin, HIGH);

	SPI.endTransaction();
}

void MAX72XXClass::sendData(uint8_t* data)
{
	//send 8 bytes to digit registers
	for (uint8_t reg = MAX72XX_REG_DIGIT0, i = 0; reg <= MAX72XX_REG_DIGIT7; reg++, i++)
	{
		Transmit(reg, *(data + i));
	}
}

bool MAX72XXClass::sendRow(uint8_t row, uint8_t data)
{
	//test if valid row
	if (row < 0 || row >= MATRIX_DIM) return false;

	//store locally
	MAXData[row] = data;

	//send to MAX (digits are 1-based, add 1)
	Transmit(DigitFromRow(row), data);

	return true;
}

bool MAX72XXClass::setPixel(uint8_t row, uint8_t col, bool value)
{
	//test if valid row
	if (row < 0 || row >= MATRIX_DIM) return false;

	//test if valid col
	if (row < 0 || row >= MATRIX_DIM) return false;

	//modify local row data
	if (value)
	{
		//set bit
		MAXData[row] |= 0x01 << col;
	}
	else
	{
		//clear bit
		MAXData[row] &= ~(0x01 << col);
	}

	//send updated row
	Transmit(DigitFromRow(row), MAXData[row]);
}


