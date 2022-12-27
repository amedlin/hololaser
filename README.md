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

**NOTE: For the holgram exposure, you should cover the Arduino blue power LED to eliminate light pollution. Any LEDs that are active during exposure risk spoiling the holographic plate. Alternately, mount the Arduino in a light-protected enclosure.**

Equipment required:
* Arduino Uno or similar
* Breadboard or punchhole PCB for wiring up circuits
* Resistors as marked in circuit diagram
* [Holography red diode laser](https://www.integraf.com/shop/holography-laser)
* Holographic kit (not covered in detail here)
* Push button switch
* Servo motor
* (optional) Potentiometer

