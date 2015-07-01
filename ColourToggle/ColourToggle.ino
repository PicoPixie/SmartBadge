/*
Lanyard-friendly conference/gathering badge. 
Uses traffic-light colour indicator.

On power-up displays BLUE, ALL_ON
Use switches on badge (back)
SWITCH 1 : cycle colour BLUE -> RED -> YELLOW -> GREEN
SWITCH 2 : cycle pattern ALL_ON -> SPIRAL_IN -> RADIATE_OUT -> CONVERGE_IN
SWITCH 3 :
SWITCH 4 :

suggested use:
	BLUE 	: 	New to the scene, don't know anyone here;
			Talk To Me Please!
	GREEN 	: 	OK to include me in photos/videos 
			that may be shared online
	YELLOW 	: 	please ask me first
	RED 	:	do not include me in any photo/videography; 
			Seriously, I'm not even supposed to be here!

firmware for @4tronix_uk SmartBadge v0.2 (ATMega328, chain of 19 ws2812 RGB LED)
blame: @_PicoPixie_
*/

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

// which pin on the Atmel is connected to the NeoPixels?
#define PIN	13

// how many NeoPixels are attached
// NeoPixels are CENTRE No.18, INNER hexagon 12..17, OUTER hexagon 0..11    
#define NUMPIXELS	19

// 4 pins of our input buttons
#define SW1    7	// colour toggle
#define SW2    8	// pattern toggle
#define SW3    9	// 
#define SW4    10	//

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// we need to keep track of where we are in the colour, pattern carousel 
uint8_t newColour, lastColour = 0; 
uint8_t newPattern, lastPattern = 0; 

bool oldStateSW1 = HIGH;
bool oldStateSW2 = HIGH;
bool state = LOW;

// timing governor in ms (1/1000th sec) 
uint8_t delayval = 400; 
long time = 0;

/*
SETUP()
set button pins to be inputs and initialise OFF
initialise, dim, and flush NeoPixels
*/
void setup() {

	pinMode(SW1, INPUT_PULLUP);
	pinMode(SW2, INPUT_PULLUP);
	pinMode(SW3, INPUT_PULLUP);
	pinMode(SW4, INPUT_PULLUP);
	
	// initialise the NeoPixel lib
	pixels.begin(); 
	// set to 1/8th max of 255, this reduces current draw and 
	// looks after YOUR EYES!
	pixels.setBrightness(32); 
	// flush
	pixels.show();
	// init display on power-up
	draw(lastColour, lastPattern); 
}

void loop() {

	bool newStateSW1 = digitalRead(SW1);
	bool newStateSW2 = digitalRead(SW2);

	if(newStateSW1 == LOW && oldStateSW1 == HIGH) {
		if(debounce(1) == LOW) { 
			newColour = update(lastColour);
			lastColour = newColour;
		}
	}	

	oldStateSW1 = newStateSW1;

	if(newStateSW2 == LOW && oldStateSW2 == HIGH) {
		if(debounce(2) == LOW) { 
			newPattern = update(lastPattern);
			lastPattern = newPattern;
		}
	}

	oldStateSW2 = newStateSW2;	

	draw(lastColour, lastPattern); 
}

bool debounce(uint8_t sw) {

	uint8_t count = 10, counter = 0;
	bool reading;

	if(millis() != time) {
		switch(sw) {
			case 1: reading = digitalRead(SW1); break;
			case 2: reading = digitalRead(SW2); break;
		}
	
		if(reading == state && counter > 0)
			counter--;
		if(reading != state)
			counter++;
		if(counter >= count) {
			counter = 0;
			state = reading;
		}
	
		time = millis();
		return state;
	}	
}

uint8_t update(uint8_t last) {

	if(last >= 3) 
		last = 0; 
	else
		last++;
	return last;
}

void draw (uint8_t lastC, uint8_t lastP) {

	pixels.clear();

	switch(lastC) {
		case 0: setColour(pixels.Color(0,0,255),lastP); break;
		case 1: setColour(pixels.Color(255,0,0),lastP); break;
		case 2: setColour(pixels.Color(255,255,0),lastP); break;
		case 3: setColour(pixels.Color(0,255,0),lastP); break;
		default: setColour(pixels.Color(255,0,255),lastP); 
	}
}

void setColour(uint32_t c, uint8_t p) {

	switch(p) {
		case 0: // ALL_ON
			for(uint8_t i=0; i<NUMPIXELS; i++) 
				pixels.setPixelColor(i,c);
			pixels.show();
			break;
		case 1: // SPIRAL_IN
			for(uint8_t i=0; i<NUMPIXELS; i++) { 
				pixels.setPixelColor(i,c);
				pixels.show();
				delay(delayval);
			}
			break;
		case 2: // RADIATE_OUT
			// centre NeoPixel
			for(uint8_t i=18; i<NUMPIXELS; i++) { 
				pixels.setPixelColor(i,c);
				pixels.show();
			}
			delay(delayval);
			// NeoPixels 12..17 - inner ring
			for(uint8_t i=12; i<18; i++) { 
				pixels.setPixelColor(i,c);
				pixels.show();
			}
			delay(delayval);
			// NeoPixels 0..11 - outer ring
			for(uint8_t i=0; i<12; i++) { 
				pixels.setPixelColor(i,c);
				pixels.show();
			}
			break;
		case 3: // CONVERGE_IN
			// NeoPixels 0..11 - outer ring
			for(uint8_t i=0; i<12; i++) { 
				pixels.setPixelColor(i,c);
				pixels.show();
			}
			delay(delayval);
			// NeoPixels 12..17 - inner ring
			for(uint8_t i=12; i<18; i++) { 
				pixels.setPixelColor(i,c);
				pixels.show();
			}
			delay(delayval);
			// centre NeoPixel
			for(uint8_t i=18; i<NUMPIXELS; i++) { 
				pixels.setPixelColor(i,c);
				pixels.show();
			}
			break;
	}
}
