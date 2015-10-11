/**
 * @file 
 *
 * Declares the MAX72XX library.
 */
#ifndef _MAX72XX_h
#define _MAX72XX_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SPI.h>

//display dimension
#define MATRIX_DIM 8

//MAX72XX Register Address Map (Ref Datasheet Table 2)
#define MAX72XX_REG_NOOP		0x00
#define MAX72XX_REG_DIGIT0		0x01
#define MAX72XX_REG_DIGIT1		0x02
#define MAX72XX_REG_DIGIT2		0x03
#define MAX72XX_REG_DIGIT3		0x04
#define MAX72XX_REG_DIGIT4		0x05
#define MAX72XX_REG_DIGIT5		0x06
#define MAX72XX_REG_DIGIT6		0x07
#define MAX72XX_REG_DIGIT7		0x08
#define MAX72XX_REG_DECODEMODE	0x09
#define MAX72XX_REG_INTENSITY	0x0A
#define MAX72XX_REG_SCANLIMIT	0x0B
#define MAX72XX_REG_SHUTDOWN	0x0C
#define MAX72XX_REG_DISPLAYTEST	0x0F

//Shutdown Regsister Values (Ref Datasheet Table 3)
#define MAX72XX_SDMODE_SHUTDOWN	0x00
#define MAX72XX_SDMODE_NORMAL		0x01

//number of valid characters in font
#define MAX72XX_FONT_CHARS 16
#define MAX72XX_7SEG_DP		0xE0	//OR with 7-SEG characters to add DP

//Display Test values
#define MAX72XX_TESTMODE_NORMAL	0x00
#define MAX72XX_TESTMODE_TEST	0x01

class MAX72XXClass
{
 private:
	 //pin used for LOAD/CS
	 int LoadPin;

	 //configurable SPI clock setting
	 uint32_t SPIClock;

	 //MAX72XX configuration settings
	 uint8_t DecodeMode;
	 uint8_t Intensity;
	 uint8_t ScanLimit;
	 bool ShutdownMode;
	 bool TestMode;

	 //current display data
	 uint8_t MAXData[8];
	 uint8_t MAXData_ReadOnly[8];

	 //generic SPI transmit method
	 void Transmit(uint8_t address, uint8_t data);
	 
	 //digit register from 0-based index helper (for code clarity)
	 uint8_t DigitRegFromZeroIndex(uint8_t row) { return row + 1; }

	 /** \brief valid font characters (Ref Table 5 of datasheet) */
	 const char FontChars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', 'E', 'H', 'L', 'P', ' ' };

 public:
	/**
	 * Constructor.
	 *
	 * @param LOAD_CS_PIN The load create struct pin.
	 * @param spi_clock   The spi clock.
	 * @param decode_mode The decode mode.
	 * @param intensity   The intensity.
	 * @param scan_limit  The scan limit.
	 * @param shutdown    true to shutdown.
	 * @param test_mode   true to enable test mode, false to disable it.
	 */
	MAX72XXClass
	(
		int LOAD_CS_PIN, 
		uint32_t spi_clock = 10000000,
		uint8_t decode_mode = 0x00,
		uint8_t intensity = 0xFF,
		uint8_t scan_limit = 0x07,
		bool shutdown = false,
		bool test_mode = false
	) :
	LoadPin(LOAD_CS_PIN),
	SPIClock(spi_clock),
	DecodeMode(decode_mode),
	Intensity(intensity),
	ScanLimit(scan_limit),
	ShutdownMode(shutdown),
	TestMode(test_mode)
	{}


	/** Initialises this object. Call in Setup()*/
	void init();

	/**
	 * Sets decode mode.
	 *
	 * @param decode_mode The decode mode.
	 */
	void setDecodeMode(uint8_t decode_mode) { Transmit(MAX72XX_REG_DECODEMODE, decode_mode); DecodeMode = decode_mode; }

	/**
	 * Sets an intensity.
	 *
	 * @param intensity The intensity.
	 */
	void setIntensity(uint8_t intensity) { Transmit(MAX72XX_REG_INTENSITY, intensity); Intensity = intensity; }

	/**
	 * Sets scan limit.
	 *
	 * @param scan_limit The scan limit.
	 */
	void setScanLimit(uint8_t scan_limit) { Transmit(MAX72XX_REG_SCANLIMIT, scan_limit); ScanLimit = scan_limit; }

	/**
	 * Sets shut down mode.
	 *
	 * @param shutdown true to shutdown, false for normal operation.
	 */
	void setShutDownMode(bool shutdown) { Transmit(MAX72XX_REG_SHUTDOWN, shutdown ? MAX72XX_SDMODE_SHUTDOWN : MAX72XX_SDMODE_NORMAL); ShutdownMode = shutdown; }

	/**
	 * Sets test mode.
	 *
	 * @param test_mode true to enable test mode, false to disable it.
	 */
	void setTestMode(bool test_mode) { Transmit(MAX72XX_REG_DISPLAYTEST, test_mode ? MAX72XX_TESTMODE_TEST : MAX72XX_TESTMODE_NORMAL); TestMode = test_mode; }

	/**
	 * Gets decode mode.
	 *
	 * @return The decode mode.
	 */
	uint8_t getDecodeMode() { return DecodeMode; }

	/**
	 * Gets the intensity.
	 *
	 * @return The intensity.
	 */
	uint8_t getIntensity() { return Intensity; }

	/**
	 * Gets scan limit.
	 *
	 * @return The scan limit.
	 */
	uint8_t getScanLimit() { return ScanLimit; }

	/**
	 * Gets shut down mode.
	 *
	 * @return true if in Shutdown Mode
	 */
	bool getShutDownMode() { return ShutdownMode; }

	/**
	 * Gets test mode.
	 *
	 * @return true if in Display Test Mode
	 */
	bool getTestMode() { return TestMode; }

	/**
	 * Sets a matrix.
	 *
	 * @param uint8_t[8][8] Row/Col data.
	 */
	void setMatrix(uint8_t* data);

	/**
	 * Sets row value.
	 *
	 * @param row   The row.
	 * @param value The value.
	 */
	void setRowValue(uint8_t row, uint8_t value);

	/**
	 * Sets column value.
	 *
	 * @param col   The col.
	 * @param value The value.
	 */
	void setColumnValue(uint8_t col, uint8_t value);

	/**
	 * Sets LED value.
	 *
	 * @param row   LED Row (0-7)
	 * @param col   LED Column (0-8)
	 * @param value Requested LED display value
	 */
	void setLEDValue(uint8_t row, uint8_t col, bool value);

	/**
	 * Gets the currently-displayed data.
	 *
	 * @return pointer to uint8_t[8][8] [row][col]
	 */
	uint8_t* getMatrix();

	/**
	 * Gets current display status of a row.
	 *
	 * @param row Requested row (0-7).
	 *
	 * @return The row value.
	 */
	uint8_t getRowValue(uint8_t row);

	/**
	 * Gets current display status of a column.
	 *
	 * @param col Requested column (0-7)
	 *
	 * @return The column value.
	 */
	uint8_t getColumnValue(uint8_t col);

	/**
	 * Gets value of specific LED form Row/Column co-ordinates
	 *
	 * @param row Row co-ordinate
	 * @param col Column co-ordinate
	 *
	 * @return Status of LED
	 */
	bool getLEDValue(uint8_t row, uint8_t col);

	/**
	 * Displays a character at selected digit position.
	 *
	 * @param digit		    Digit position (0-7)
	 * @param character	    The characteCharacter to display (Refer to datasheet for valid characters)
	 * @param decimal_point true to add decimal point to digit.
	 */
	void setCharacter(uint8_t digit, char character, bool decimal_point);
};



#endif

