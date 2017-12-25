# Arduino Clock
## A digital clock using the Arduino Pro Mini

![Arduino Clock Wiring Diagram](https://github.com/ajithvasudevan/ArduinoCLock/raw/master/Clock%20-%20%20Wiring.png)



* Connections from D2 to D13 on the Arduino Pro Mini to the corresponding transistors are omitted in the diagram for clarity. Similarly marked points need to be connected together

* All Transistor, except T1, T2, T3, T4 are NPN type  – Part No. BC 547
 
* T1, T2, T3, T4 are PNP type Transistors                  – Part No. BC 557 

* All Resistors are 4.7 kΩ, unless specified in the diagram

* The 7-Segment LED Displays ( https://en.wikipedia.org/wiki/Seven-segment_display  ) used here are “Common Anode (+ve)” type, and work with 9V and an 820Ω resistor for limiting current.

* The cathodes (-ve) of the segments of all 7-segment displays are connected together, i.e., cathode of segment ‘A’ of all displays are connected together, cathode of segment ‘B’ are connected together, and so on. 

* Regulator IC 7805 is used to provide 5V supply to the Arduino, as the input voltage is 9V.
