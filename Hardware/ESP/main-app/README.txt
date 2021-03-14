This library is used to program the central MCU of the ShutTheFrontDoor Project.

GPIO List of the ESP32-SOLO-1 in ascending order
1:  TXD0 (Reserved)
2:  (in)  Cam CS
3:  RXT0 (Reserved)
4:
5:
6:  SCK
7:  SDO (out)
8:  SDI (in)
9:  SD2
10: SD3
11: CS
12: GND
13: 
14: 
15: (in) Motion Sensor Signal
16:
17:
18:
19:
NO 20
21:
22: BOOT Button
23: 
NO 24
25: (in)  Motor Fault Detect.
26: (in)  N-switch
27: (in)  Battery ADC
NO 28-31
32: (out) Motor CTRL IN 2
33: (out) Motor CTRL IN 1
34: (in)  Doorbell Signal
35: (in)  Reed Sw. Signal
36: SENSOR_VP
NO 37-38
39: SENSOR_VN

Note: Can't use ADC 2 with Wifi driver as Wifi uses ADC 2

 ADC 1           ADC 2
 CH  GPIO        CH  GPIO
 0   36          0   4
 3   39          1   0
 4   32          2   2
 5   33          3   15
 6   34          4   13
 7   35          5   12
                 6   14
                 7   27
                 8   25
                 9   26