#include <Adafruit_NeoPixel.h>

#define DATAPIN 2
#define PIXELS 60

byte pixelBuffer[3];

Adafruit_NeoPixel leds = Adafruit_NeoPixel(PIXELS, DATAPIN);

void setup()
{
  // Clear LEDs
  leds.begin();
  leds.show();

  // Open serial port and tell the controller we're ready.
  Serial.begin(1228800);
  Serial.write(0x00);
}

void loop()
{
  // Read a command
  while (Serial.available() == 0);
  byte command = Serial.read();
  byte count;
  switch (command)
  {
    // Show frame
    case 0x00:
      
      // Update LEDs
      leds.show();

      // Tell the controller we're ready
      // We don't want to be receiving serial data during leds.show() because data will be dropped
      Serial.write(0x00);
      break;
      
    // Load frame
    case 0x01:

      // Read number of pixels
      while (Serial.available() == 0);
      count = Serial.read();

      // Read and update pixels
      for (int i = 0; i < count; i++)
      {
        Serial.readBytes(pixelBuffer, 3);
        leds.setPixelColor(i, pixelBuffer[2], pixelBuffer[1], pixelBuffer[0]);
      }
      break;

    // Clear
    case 0xFF:
      leds.clear();
      leds.show();
      break;
  }
}

/***************************************************
 * COMMANDS                                        *
 * ----------------------------------------------- *
 * 00                                              *
 * Displays the frame currently in the buffer      *
 * Replies: 00                                     *
 *                                                 *
 * 01 nn b1 g1 r1 b2 g2 r2 ... bn gn rn            *
 * Reads nn pixels into the frame buffer           *
 * Replies: nothing                                *
 *                                                 *
 * FF                                              *
 * Clears the display and terminates the animation *
 * Replies: nothing                                *
 ***************************************************/
 