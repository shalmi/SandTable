EESchema Schematic File Version 4
LIBS:Sisyphus-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Sisyphus-rescue:TMC2130_Breakout-Driver_Motor U3
U 1 1 5B717E10
P 8350 2550
F 0 "U3" H 8350 3115 50  0000 C CNN
F 1 "TMC2130_Breakout" H 8350 3024 50  0000 C CNN
F 2 "Sisyphus:TMC_2130_Footprint" H 8350 2000 50  0001 C CNN
F 3 "" H 8350 2000 50  0001 C CNN
	1    8350 2550
	1    0    0    -1  
$EndComp
$Comp
L Sisyphus-rescue:TMC2130_Breakout-Driver_Motor U4
U 1 1 5B717E73
P 8350 3750
F 0 "U4" H 8350 4315 50  0000 C CNN
F 1 "TMC2130_Breakout" H 8350 4224 50  0000 C CNN
F 2 "Sisyphus:TMC_2130_Footprint" H 8350 3200 50  0001 C CNN
F 3 "" H 8350 3200 50  0001 C CNN
	1    8350 3750
	1    0    0    -1  
$EndComp
$Comp
L New_Library:HUZZAH32 U1
U 1 1 5CB2C35E
P 4700 1900
F 0 "U1" H 4725 2865 50  0000 C CNN
F 1 "HUZZAH32" H 4725 2774 50  0000 C CNN
F 2 "Sisyphus:Huzzah32_Footprint" H 4700 2750 50  0001 C CNN
F 3 "" H 4700 2750 50  0001 C CNN
	1    4700 1900
	1    0    0    -1  
$EndComp
Text GLabel 4150 1300 0    50   Input ~ 0
3V
Text GLabel 4150 1500 0    50   Input ~ 0
GND
Text GLabel 4150 1900 0    50   Input ~ 0
A3_39
Text GLabel 4150 2000 0    50   Input ~ 0
A4_36
Text GLabel 4150 2100 0    50   Input ~ 0
A5_4
Text GLabel 4150 2200 0    50   Input ~ 0
SCK_5
Text GLabel 4150 2300 0    50   Input ~ 0
MOSI_18
Text GLabel 4150 2400 0    50   Input ~ 0
MISO_19
Text GLabel 4150 2500 0    50   Input ~ 0
RX_16
Text GLabel 4150 2600 0    50   Input ~ 0
TX_17
Text GLabel 4150 2700 0    50   Input ~ 0
21
Text GLabel 5300 1800 2    50   Input ~ 0
USB_5V
NoConn ~ 5300 1600
NoConn ~ 4150 1200
$Comp
L New_Library:MicroSD_Card_Adapter U2
U 1 1 5CB304D5
P 8200 1500
F 0 "U2" H 8258 2165 50  0000 C CNN
F 1 "MicroSD_Card_Adapter" H 8258 2074 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 8200 2050 50  0001 C CNN
F 3 "" H 8200 2050 50  0001 C CNN
	1    8200 1500
	1    0    0    -1  
$EndComp
Text GLabel 8450 1100 2    50   Input ~ 0
A5_4
Text GLabel 8450 1200 2    50   Input ~ 0
SCK_5
Text GLabel 8450 1300 2    50   Input ~ 0
MOSI_18
Text GLabel 8450 1400 2    50   Input ~ 0
MISO_19
Text GLabel 8450 1500 2    50   Input ~ 0
USB_5V
Text GLabel 8450 1600 2    50   Input ~ 0
GND
Text GLabel 8000 2350 0    50   Input ~ 0
3V
Text GLabel 8000 2250 0    50   Input ~ 0
GND
Text GLabel 8000 3550 0    50   Input ~ 0
3V
Text GLabel 8000 3450 0    50   Input ~ 0
GND
$Comp
L Device:R R1
U 1 1 5CB359C5
P 1600 1400
F 0 "R1" H 1670 1446 50  0000 L CNN
F 1 "2K" H 1670 1355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1530 1400 50  0001 C CNN
F 3 "~" H 1600 1400 50  0001 C CNN
	1    1600 1400
	0    1    1    0   
$EndComp
Text GLabel 1250 1400 0    50   Input ~ 0
A3_39
Text GLabel 1250 1950 0    50   Input ~ 0
A4_36
$Comp
L Device:R R2
U 1 1 5CB35C79
P 1600 1950
F 0 "R2" H 1670 1996 50  0000 L CNN
F 1 "2K" H 1670 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1530 1950 50  0001 C CNN
F 3 "~" H 1600 1950 50  0001 C CNN
	1    1600 1950
	0    1    1    0   
$EndComp
Text GLabel 1750 1400 2    50   Input ~ 0
GND
Text GLabel 1750 1950 2    50   Input ~ 0
GND
$Comp
L Connector:Conn_01x07_Male J1
U 1 1 5CB3D6F7
P 2400 3750
F 0 "J1" H 2508 4231 50  0000 C CNN
F 1 "Conn_01x07_Male" H 2508 4140 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x07_P2.54mm_Vertical" H 2400 3750 50  0001 C CNN
F 3 "~" H 2400 3750 50  0001 C CNN
	1    2400 3750
	1    0    0    -1  
$EndComp
Text GLabel 2600 3450 2    50   Input ~ 0
GND
Text GLabel 2600 3550 2    50   Input ~ 0
Inner_Switch
Text GLabel 2600 3650 2    50   Input ~ 0
Outer_Switch
Text GLabel 2600 3750 2    50   Input ~ 0
Theta_Motor_A
Text GLabel 2600 3850 2    50   Input ~ 0
Theta_Motor_B
Text GLabel 2600 3950 2    50   Input ~ 0
Theta_Motor_C
Text GLabel 2600 4050 2    50   Input ~ 0
Theta_Motor_D
Text GLabel 8000 2450 0    50   Input ~ 0
Theta_Motor_A
Text GLabel 8000 2550 0    50   Input ~ 0
Theta_Motor_B
Text GLabel 8000 2650 0    50   Input ~ 0
Theta_Motor_C
Text GLabel 8000 2750 0    50   Input ~ 0
Theta_Motor_D
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 5CB41E93
P 4300 3650
F 0 "J3" H 4408 3931 50  0000 C CNN
F 1 "Conn_01x04_Male" H 4408 3840 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 4300 3650 50  0001 C CNN
F 3 "~" H 4300 3650 50  0001 C CNN
	1    4300 3650
	1    0    0    -1  
$EndComp
Text GLabel 4500 3550 2    50   Input ~ 0
Radius_Motor_A
Text GLabel 4500 3650 2    50   Input ~ 0
Radius_Motor_B
Text GLabel 4500 3750 2    50   Input ~ 0
Radius_Motor_C
Text GLabel 4500 3850 2    50   Input ~ 0
Radius_Motor_D
Text GLabel 8000 3650 0    50   Input ~ 0
Radius_Motor_A
Text GLabel 8000 3750 0    50   Input ~ 0
Radius_Motor_B
Text GLabel 8000 3850 0    50   Input ~ 0
Radius_Motor_C
Text GLabel 8000 3950 0    50   Input ~ 0
Radius_Motor_D
Text GLabel 8700 2950 2    50   Input ~ 0
21
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 5CB43B21
P 3900 4950
F 0 "J2" H 4008 5131 50  0000 C CNN
F 1 "Conn_01x02_Male" H 4008 5040 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 3900 4950 50  0001 C CNN
F 3 "~" H 3900 4950 50  0001 C CNN
	1    3900 4950
	1    0    0    -1  
$EndComp
Text GLabel 4100 5050 2    50   Input ~ 0
GND
Text GLabel 4100 4950 2    50   Input ~ 0
High_Motor_Voltage
Text GLabel 8000 4150 0    50   Input ~ 0
High_Motor_Voltage
Text GLabel 8000 2950 0    50   Input ~ 0
High_Motor_Voltage
Text GLabel 8000 4050 0    50   Input ~ 0
GND
Text GLabel 8000 2850 0    50   Input ~ 0
GND
Wire Wire Line
	1450 1400 1350 1400
Wire Wire Line
	1250 1950 1350 1950
Text GLabel 1450 1200 2    50   Input ~ 0
Inner_Switch
Text GLabel 1450 1750 2    50   Input ~ 0
Outer_Switch
Wire Wire Line
	1450 1200 1350 1200
Wire Wire Line
	1350 1200 1350 1400
Connection ~ 1350 1400
Wire Wire Line
	1350 1400 1250 1400
Wire Wire Line
	1450 1750 1350 1750
Wire Wire Line
	1350 1750 1350 1950
Connection ~ 1350 1950
Wire Wire Line
	1350 1950 1450 1950
Text GLabel 5300 1900 2    50   Input ~ 0
A12_13_LED
Text GLabel 5300 2000 2    50   Input ~ 0
A11_PDR_12
Text GLabel 5300 2100 2    50   Input ~ 0
A10_27
Text GLabel 5300 2200 2    50   Input ~ 0
A9_33
Text GLabel 5300 2300 2    50   Input ~ 0
A8_15
Text GLabel 5300 2400 2    50   Input ~ 0
A7_32
Text GLabel 5300 2500 2    50   Input ~ 0
A6_14
Text GLabel 5300 2600 2    50   Input ~ 0
SCL_22
Text GLabel 5300 2700 2    50   Input ~ 0
SDA_23
Text GLabel 8700 2250 2    50   Input ~ 0
A10_27
Text GLabel 8700 2350 2    50   Input ~ 0
A9_33
Text GLabel 8700 4150 2    50   Input ~ 0
A8_15
Text GLabel 8700 3550 2    50   Input ~ 0
A7_32
Text GLabel 8700 3450 2    50   Input ~ 0
A6_14
$EndSCHEMATC
