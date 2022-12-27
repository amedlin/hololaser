# hololaser
Arduino code for controlling laser for exposing holograms

Based on Arduino Uno. This project is designed to give you the software and circuit diagrams which, along with the Arduino Uno and some additional hardware and holographic equipment, will allow you to expose holographic plates to laser light in a controlled manner.

This project comprises 2 power supply circuits and 2 control circuits.

Power supply circuits:
* Power to red diode laser, regulated down to 3.9 V
* Power to servo motor (used to open an close aperture)

Control circuits:
* Manual switch to trigger laser exposure cycle
* Potentiometer to test opening and closing of aperture. (When an exposure cycle is kicked off, the aperture will close before turning on the laser.)

