#include <avr/io.h>

unsigned char display[4] = {0x04, 0x03, 0x02, 0x01};


unsigned char key_hold = 0;


unsigned char data[16] = {0x01, 0x04, 0x07, 0x0D, 0x02, 0x05, 0x08, 0x00, 0x03, 0x06, 0x09, 0x0E, 0x0A, 0x0B, 0x0C, 0x0F};

void send_data(unsigned char x);
void keypad_scan(void);

int main(void)
{
	DDRB = 0xB0;       // PB5(MOSI), PB7(SCK) outputs
	DDRD = 0x02;       // PD1 output for EN, PD2 input for MUX_OUT
	PORTD = 0x04;      // Enable internal Pull-up resistor on PD2
	SPCR = 0x50;       // Enable SPI hardware
	
	while(1)
	{
		keypad_scan();
	}
}

void send_data(unsigned char x)
{
	// 1. TURN SCREEN OFF (EN = LOW hides the scrambling bits)
	PORTD = PORTD & 0xFD;
	
	// 2. SEND SPI DATA
	SPDR = x;
	while ((SPSR & 0x80) == 0);
	
	// 3. CLEAR SPI FLAG
	unsigned char junk = SPDR;
	
	// 4. TURN SCREEN ON (EN = HIGH shows the stable number)
	PORTD = PORTD | 0x02;
}

void keypad_scan(void)
{
	unsigned char scan, y;
	unsigned char key_value = 0;
	unsigned char key_pressed = 0;
	
	volatile unsigned int j;
	
	// Loop through all 16 states (4 rows * 4 columns)
	for(scan = 0; scan < 16; scan++)
	{
		y = (scan << 4) | display[scan & 0x03];
		
		send_data(y);
		
		for(j = 0; j < 3000; j++);
		
		
		if ((PIND & 0x04) != 0)
		{
			key_pressed = 1;
			key_value = data[scan];
		}
	}
	
	// If a key was pressed AND we aren't already holding a key down
	if (key_pressed == 1 && key_hold == 0)
	{
		// Shift all digits to the left
		display[0] = display[1];
		display[1] = display[2];
		display[2] = display[3];
		
		// Put the new key on the far right
		display[3] = key_value;

		// Lock the keypad so it doesn't rapid-fire shift
		key_hold = 1;
	}
	
	// If no keys are being pressed, unlock the keypad
	if (key_pressed == 0)
	{
		key_hold = 0;
	}
}


