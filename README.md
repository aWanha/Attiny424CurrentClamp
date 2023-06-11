# Attiny424CurrentClamp
attiny424 controlled DC current measuring clamp, resolution of 10mA at 750 mA range with sampling speed of ~ 100 Hz (1024x oversampling). Device output is compromise between sampling speed and resolution. Interface through buttons and serial communication.

Main focus in this project is to minimize the effect of noise in readings, which is rather difficult to filter in hardware (noise below 50Hz). Offset needs to be calibrated every time clamp is moved, any change in orientation will change the readings because of earths magnetic fields. Offset calibration is initaited by button press, process takes ~1sec (for filtered pwm signal to settle).

![schematic](https://github.com/aWanha/Attiny424CurrentClamp/blob/main/schematic.JPG)
# Test against multimeter(blue), mA vs seconds graph
![schematic](https://github.com/aWanha/Attiny424CurrentClamp/blob/main/linearity.JPG)
