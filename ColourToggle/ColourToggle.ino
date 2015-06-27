/*
Lanyard-friendly conference/gathering badge. 
Uses traffic-light colour indicator.
displays BLUE on power-up, use switch 1 on badge (back) 
to cycle through RED->YELLOW->GREEN->BLUE

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
#define NUMPIXELS	19

// 4 pins of our input buttons
#define SW1    7	// colour toggle
#define SW2    8	// pattern toggle
#define SW3    9	// 
#define SW4    10	//

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/* 
we need to keep track of the last colour displayed 
as we carousel thru' the options with each button_1 press
*/
int lastColour = 0; 

/*
governor to change colour one pixel at a time
in ms (1/1000th sec)
*/
int delayval = 500; 



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
	pixels.show();

	// send an initial BLUE colour on power-up
        for(int i=0; i<NUMPIXELS; i++) pixels.setPixelColor(i,0,0,255);
	pixels.show();
}

void loop() {
  
        if(digitalRead(SW1) == LOW) {
		// they pressed button_1, toggle colour 
		// wait for rebound of switch
		delay(300); 
		// what was the last colour.? R->Y->G->B->R
		switch(lastColour) {
			case 0: // was init colour
				// so display red
                                for(int i=0; i<NUMPIXELS; i++) 
					pixels.setPixelColor(i,255,0,0);
                                pixels.show();
				/*
				record this change, 
				we ntk for next time 'round the loop
				*/
				lastColour = 1;
				break;
			case 1: // was red
				// become yellow
                                for(int i=0; i<NUMPIXELS; i++) 
					pixels.setPixelColor(i,255,255,0);
                                pixels.show();
				lastColour = 2;
				break;
			case 2: // was yellow
				// become green
                                for(int i=0; i<NUMPIXELS; i++) 
					pixels.setPixelColor(i,0,255,0);
                                pixels.show();
				lastColour = 3;
				break;
			case 3: // was green
				// become blue
                                for(int i=0; i<NUMPIXELS; i++) 
					pixels.setPixelColor(i,0,0,255);
                                pixels.show();
				lastColour = 4;
				break;
			default: // go red
                                for(int i=0; i<NUMPIXELS; i++) 
					pixels.setPixelColor(i,255,0,0);
                                pixels.show();
				lastColour = 1;
		}
	}
}


