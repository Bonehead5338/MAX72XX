/**
 * @file C:\Users\DAVE\Documents\Arduino\libraries\MAX72XX\MAX72XX.cpp
 *
 * Implements the MAX72XX library
 */
#include "MAX72XX.h"

void MAX72XX::init()
{
	//setup pin
	pinMode(LoadPin, OUTPUT);
	
	//clear data registers
	for (uint8_t i = 0; i < MATRIX_DIM; i++) MAXData[i] = 0x00;
	
	//send data
	setMatrix(MAXData);		

	//set decode mode
	Transmit(MAX72XX_REG_DECODEMODE, DecodeMode);

	//set intensity
	Transmit(MAX72XX_REG_INTENSITY, Intensity);

	//set scan limit
	Transmit(MAX72XX_REG_SCANLIMIT, ScanLimit);

	//set shutdown mode
	Transmit(MAX72XX_REG_SHUTDOWN, ShutdownMode ? MAX72XX_SDMODE_SHUTDOWN : MAX72XX_SDMODE_NORMAL);

	//set display test mode
	Transmit(MAX72XX_REG_DISPLAYTEST, DisplayTestMode ? MAX72XX_TESTMODE_TEST : MAX72XX_TESTMODE_NORMAL);
}

void MAX72XX::Transmit(uint8_t address, uint8_t data)
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

void MAX72XX::setMatrix(uint8_t* data)
{
	//send 8 bytes to digit registers
	for (uint8_t reg = MAX72XX_REG_DIGIT0, i = 0; reg <= MAX72XX_REG_DIGIT7; reg++, i++)
	{
		Transmit(reg, data[i]);
	}
}

void MAX72XX::setRowValue(uint8_t row, uint8_t value)
{
	//test if valid row
	if (row >= MATRIX_DIM)
		return;

	//store locally
	MAXData[row] = value;

	//send to MAX (digits are 1-based, add 1)
	Transmit(DigitRegFromZeroIndex(row), MAXData[row]);
}

void MAX72XX::setColumnValue(uint8_t col, uint8_t value)
{
	//test if valid row
	if (col >= MATRIX_DIM)
		return;

	//for each row..
	for (uint8_t row = 0; row < MATRIX_DIM; row++)
	{
		//test "row-th" bit of new column value
		if ((0x01 << row) && value)
		{
			//set "column-th" bit of matrix data
			MAXData[row] |= (0x01 << col);
		}
		else
		{
			//clear "column-th" bit of matrix data
			MAXData[row] &= ~(0x01 << col);
		}
	}

	//update entire matrix
	setMatrix(MAXData);
}

void MAX72XX::setLEDValue(uint8_t row, uint8_t col, bool value)
{
	//test if valid row and column
	if (row >= MATRIX_DIM || col >= MATRIX_DIM)
		return;

	//modify local row data
	if (value)
	{
		//set "column-th" bit in row
		MAXData[row] |= 0x01 << col;
	}
	else
	{
		//clear "column-th" bit in row
		MAXData[row] &= ~(0x01 << col);
	}

	//send updated row to Matrix
	Transmit(DigitRegFromZeroIndex(row), MAXData[row]);
}

uint8_t* MAX72XX::getMatrix()
{	
	//copy current display data to read only area
	for (uint8_t i = 0; i < MATRIX_DIM; i++)
	{
		MAXData_ReadOnly[i] = MAXData[i];
	}

	//return pointer to read-only data
	return MAXData_ReadOnly;
}

uint8_t MAX72XX::getRowValue(uint8_t row)
{
	//return zero for invalid row
	if (row >= MATRIX_DIM)
		return 0;
	else
	{
		//return requested row data
		return MAXData[row];
	}
}

uint8_t MAX72XX::getColumnValue(uint8_t col)
{
	//return zero for invalid column
	if (col >= MATRIX_DIM)
		return 0;
	else
	{
		//byte for result
		uint8_t result = 0;
		
		//for each row..
		for (uint8_t row = 0; row < MATRIX_DIM; row++)
		{
			//test the "column-th" bit
			if (MAXData[row] && (0x01 << col))
			{
				//set "row-th" bit in result
				result |= (0x01 << col);
			}
		}

		return result;
	}	
}

bool MAX72XX::getLEDValue(uint8_t row, uint8_t col)
{
	//return false for invalid row or column
	if (row >= MATRIX_DIM || col >= MATRIX_DIM)
		return false;
	else
	{
		//return "column-th" bit of row
		return (MAXData[row] && (0x01 << col));
	}
}

void MAX72XX::setCharacter(uint8_t digit, char character, bool decimal_point)
{
	//check valid digit 
	if (digit >= MATRIX_DIM) return;

	//check against all valid characters in font
	for (uint8_t i = 0; i < MAX72XX_FONT_CHARS; i++)
	{
		//if valid character, send for display
		if (character == FontChars[i])
		{
			//send to MAX72XX (or with 0xE0 to add decimal point)
			Transmit(DigitRegFromZeroIndex(digit), decimal_point ? i |= MAX72XX_7SEG_DP : i);
		}
	}
}


