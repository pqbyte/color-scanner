# Color Scanner
This program is built for the ChipKIT Uno32 board (with basic I/O shield), and the Adafruit TCS34725
RGB Color Sensor. What this program does is it scans the color of an object
that is near the sensor, and shows the name of the color on the display.

We are using the XKDC color database, but it can be replaced with any database
using the same format.

This project was created for the course IS1500 in KTH, by Toni Karppi and Carl
Leijonberg.

## Attributions
Some boilerplate code was borrowed/adapted from the [course repository](https://github.com/is1200-example-projects).

The official Arduino library for the TCS34725 was also helpful in creating the structure
for retrieving the colors from the sensor.
