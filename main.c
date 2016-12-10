/* Program that reads colors of objects from a TSC34725 RGB sensor,
 * and searches for the name of the closest matching colors,
 * which is shown on the display.
 *
 * Authors:
 * Toni Karppi - Communication logic between color sensor and Uno32 board.
 * Carl Leijonberg - RGB to color name module.
 */

#include <pic32mx.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "i2c.h"
#include "display.h"
#include "extrafunct.h"
//#include "Color-convert.c"

/* Color sensor values */
#define TCS34725_ADDRESS 0x29 // Device address
#define TCS34725_INTEGRATIONTIME_700MS 0x00 // Integration time 700 ms
#define TCS34725_GAIN_4X 0x01 // Gain 4x
#define TCS34725_ID 0x12 // Device ID
#define TCS34725_COMMAND_BIT 0x80 // Command bit
#define TCS34725_ENABLE 0x00 // Enable sensor
#define TCS34725_ENABLE_PON 0x01 // Power on
#define TCS34725_ENABLE_AEN 0x02 // RGBC enable
#define TCS34725_ATIME 0x01 // Integration time
#define TCS34725_RDATAL 0x16 // Lower red bits
#define TCS34725_GDATAL 0x18 // Lower green bits
#define TCS34725_BDATAL 0x1A // Lower blue bits
#define TCS34725_CDATAL 0x14 // Lower clear bits
#define TCS34725_CONTROL 0x0F // Gain level

void set_integration_time(uint8_t);
void set_gain(uint8_t);
void enable_sensor();
uint8_t read8(uint8_t);
uint16_t read16(uint8_t);
void write8(uint8_t, uint8_t);
bool start_sensor();
uint8_t normalize_colors(uint16_t, uint32_t);
void sleep(uint8_t);
void color_values(uint16_t*, uint16_t*, uint16_t*);

bool initialized = false;

/* Read 1 byte from register over I2C */
uint8_t read8(uint8_t reg) {
  // Set target register
  uint8_t write_address = TCS34725_ADDRESS << 1;
  begin_transmission(write_address);
  i2c_send(TCS34725_COMMAND_BIT | reg);

  // Read from register
  uint8_t read_address = (TCS34725_ADDRESS << 1) | 1;
  begin_transmission(read_address);
  uint8_t data = i2c_recv();
  i2c_nack();
  i2c_stop();
  return data; 
}

/* Read 2 bytes from register over I2C */
uint16_t read16(uint8_t reg) {
  // Read target register
  uint8_t write_address = TCS34725_ADDRESS << 1;
  begin_transmission(write_address);
  i2c_send(TCS34725_COMMAND_BIT | reg);

  // Read from register
  uint8_t read_address = (TCS34725_ADDRESS << 1) | 1;
  begin_transmission(read_address);
  uint16_t data1 = i2c_recv();
  i2c_ack();
  uint16_t data2 = i2c_recv();
  i2c_nack();
  i2c_stop();

  data2 <<= 8;
  data2 |= data1;

  return data2; 
}

/* Write 1 byte to register over I2C */
void write8(uint8_t reg, uint8_t value) {
  uint8_t write_address = TCS34725_ADDRESS << 1;
  begin_transmission(write_address);
  i2c_send(TCS34725_COMMAND_BIT | reg);
  i2c_send(value);
  i2c_stop();
}

/* Set up sensor state */
bool start_sensor() {
  // Make sure we're actually connected
  uint8_t id = read8(TCS34725_ID);
  if (id != 0x44) {
    return false;
  }
  initialized = true;
  delay(10000);

  set_integration_time(TCS34725_INTEGRATIONTIME_700MS);
  set_gain(TCS34725_GAIN_4X);

  enable_sensor();

  return true; 
}

/* Property of the sensor */
void set_integration_time(uint8_t it) {
  if (!initialized) {
    start_sensor();
  }
  
  write8(TCS34725_ATIME, it); 
}

/* Property of the sensor */
void set_gain(uint8_t gain) {
  if (!initialized) {
    start_sensor();
  }

  write8(TCS34725_CONTROL, gain); 
}

/* Enable power and analog converter in sensor */
void enable_sensor() {
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  delay(10000);
  write8(
      TCS34725_ENABLE,
      TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN
  );
}

/* Scale down color value to 1 byte */
uint8_t normalize_color(uint16_t color, uint32_t sum) {
   float c = color;
   c /= sum;
   c *= 256;
   return (uint8_t) c;
}

/* Pause the program a specified number of hundreds of micros */
void sleep(uint8_t hundreds_of_micros) {
  uint8_t timeout_count = 0;
  for (;;) {
    if (IFS(0) & 0x100){
      IFSCLR(0) = 0x100;
      if (timeout_count == hundreds_of_micros) {
        break;
      } else {
        timeout_count++;
      }
    }
  }
}

/* Read in the colors to the input variables from sensor */
void color_values(uint16_t *red, uint16_t *green, uint16_t *blue) {
  if (!initialized) {
    start_sensor();
  }

  *red = read16(TCS34725_RDATAL);
  *green = read16(TCS34725_GDATAL);
  *blue = read16(TCS34725_BDATAL);

  // Sleep for 10 hundred microseconds = 1 second.
  sleep(10);
}

void setup_clock() {
  OSCCON &= ~0x180000;
  OSCCON |= 0x080000;
}

void setup_output_pins() {
  AD1PCFG = 0xFFFF;
  ODCE = 0x0;
  TRISECLR = 0xFF;
  PORTE = 0x0;
  TRISESET = 0xff00;

  // Disply signals
  PORTF = 0xFFFF;
  PORTG = (1 << 9);
  ODCF = 0x0;
  ODCG = 0x0;
  TRISFCLR = 0x70;
  TRISGCLR = 0x200;
}

void setup_input_pins() {
  TRISDSET = (1 << 8);
  TRISFSET = (1 << 1);
}

void setup_spi() {
  SPI2CON = 0;
  SPI2BRG = 4;
  
  /* Clear SPIROV*/
  SPI2STATCLR &= ~0x40;
  /* Set CKP = 1, MSTEN = 1; */
  SPI2CON |= 0x60;
  
  /* Turn on SPI */
  SPI2CONSET = 0x8000;
}

void setup_i2c() {
  I2C1CON = 0x0;
  /* I2C Baud rate should be less than 400 kHz, is generated by dividing
  the 40 MHz peripheral bus clock down */
  I2C1BRG = 0x0C2;
  I2C1STAT = 0x0;
  I2C1CONSET = 1 << 13; // SIDL = 1
  I2C1CONSET = 1 << 15; // ON = 1
}

void setup_timer() {
  T2CON = 0;                // Reset timer
  TMR2 = 0;                 // Clear timer register
  T2CONSET = 7 << 4;        // Set prescaler 1:256
  PR2 = (80000000/256)/10;  // Set period register
  T2CONSET = 0x8000;        // Start timer
}

/* Main program loop */
void loop(void) {
  
  for (;;) {
    uint16_t red, green, blue;
    color_values(&red, &green, &blue);
    uint32_t sum = red + green + blue;
    
    uint8_t r = normalize_color(red, sum);
    uint8_t g = normalize_color(green, sum);
    uint8_t b = normalize_color(blue, sum);

    char* name = (char*) findColor(r, g, b);

    display_string(1, name); 
  
    /*char* r_str = itoaconv(r);
    char* g_str = itoaconv(g);
    char* b_str = itoaconv(b);

    display_string(1, r_str);
    display_string(2, g_str);
    display_string(3, b_str);*/
    display_update();
  }
}

int main(void) {
  setup_clock();
  setup_output_pins();
  setup_input_pins();
  setup_spi();
  setup_i2c();
  setup_timer();
  display_init();
  fill_colorList();

  if (start_sensor()) {
    display_string(0, "Found sensor");
  } else {
    display_string(0, "Did not find sensor");
    while(true);
  }
  display_update();
  loop();
  
  return 0;
}
