#include <avr/io.h>

unsigned char display[4] = {0x04, 0x03, 0x02, 0x01};

int main(void)
{
	unsigned char x, i;
	
	
	unsigned int j;

	DDRD = 0x02;
	DDRB = 0xB0;            // PB7(SCK), PB5(MOSI) outputs
	SPCR = 0x50;            // Enable SPI hardware

	while(1)
	{
		for (i = 0; i < 4; i++)
		{
			
			PORTD = PORTD & 0xFD;
			
			
			x = (i << 4) | display[i];
			SPDR = x;
			while((SPSR & 0x80) == 0x0);
			
			
			x = SPDR;
			
			
			PORTD = PORTD | 0x02;
			
			
			for(j = 0; j < 3000; j++);
		}
	}
}

