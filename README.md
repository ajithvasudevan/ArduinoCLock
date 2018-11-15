# Arduino Clock
## A digital clock using the Arduino Pro Mini

This is an Arduino mini based digital clock which displays time on a set of four 7-segment displays. It uses a I2C crystal oscillator (DS 3231 - https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231.html ) to keep time accurate to 2 ppm, which is about 30 seconds in an year.

![Arduino Clock Wiring Diagram](https://github.com/ajithvasudevan/ArduinoCLock/raw/master/Clock%20-%20%20Wiring.png)



* Connections from D2 to D13 on the Arduino Pro Mini to the corresponding transistors are omitted in the diagram for clarity. Similarly marked points need to be connected together

* All Transistor, except T1, T2, T3, T4 are NPN type  – Part No. BC 547
 
* T1, T2, T3, T4 are PNP type Transistors                  – Part No. BC 557 

* All Resistors are 4.7 kΩ, unless specified in the diagram

* The 7-Segment LED Displays ( https://en.wikipedia.org/wiki/Seven-segment_display  ) used here are “Common Anode (+ve)” type, and work with 9V and an 820Ω resistor for limiting current.

* The cathodes (-ve) of the segments of all 7-segment displays are connected together, i.e., cathode of segment ‘A’ of all displays are connected together, cathode of segment ‘B’ are connected together, and so on. 

* Regulator IC 7805 is used to provide 5V supply to the Arduino, as the input voltage is 9V.

* The DS 3231 is  a I2C Crystal Oscillator. It keeps time and provides the Arduino with the necessary clock signal.


An image of the PCB for this Clock is shown below:

![Arduino Clock PCB](https://github.com/ajithvasudevan/ArduinoCLock/raw/master/IMG_20181013_164144.jpg)
