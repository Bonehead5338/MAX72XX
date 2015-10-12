# MAX72XX
MAX7219 / MAX7221 arduino library

Library documentation [here](http://dave93cab.github.io/MAX72XX/class_m_a_x72_x_x_class.html)

An example arduino sketch file to initalize library:

```
#include <SPI.h>
#include <MAX72XX.h>

//pin for loading matrix (LOAD/CS)
#define LOAD_PIN 10

//instance of MAX72XX class (default settings)
MAX72XXClass MAX(LOAD_PIN);

void setup()
{
	//initialize SPI
	SPI.begin();

	//init MAX
	MAX.init();
	
}

void loop()
{
	//some logic here
}
```
