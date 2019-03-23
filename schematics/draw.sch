EESchema Schematic File Version 4
LIBS:draw-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 6500 3250 700  1450
U 58815CE4
F0 "MCU" 60
F1 "mcu.sch" 60
F2 "I2C0-SDA" I L 6500 3600 60 
F3 "I2C0_SCL" I L 6500 3800 60 
F4 "3.3V" I L 6500 4000 60 
F5 "GND" I L 6500 4200 60 
F6 "GPIO_PB11" I L 6500 4400 60 
$EndSheet
$Sheet
S 4350 3250 800  1450
U 5881B945
F0 "Current Sense Amplifier" 60
F1 "measure.sch" 60
F2 "GND" I R 5150 4200 60 
F3 "3.3V" I R 5150 4000 60 
F4 "I2C_SDA" I R 5150 3600 60 
F5 "I2C_SCL" I R 5150 3800 60 
F6 "ALERT" I R 5150 4400 60 
$EndSheet
$Comp
L Device:R_Small R1
U 1 1 588316A9
P 5800 3600
F 0 "R1" V 5700 3650 50  0000 L CNN
F 1 "0R" V 5800 3550 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5800 3600 50  0001 C CNN
F 3 "" H 5800 3600 50  0000 C CNN
	1    5800 3600
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R2
U 1 1 58831855
P 5800 3800
F 0 "R2" V 5700 3850 50  0000 L CNN
F 1 "0R" V 5800 3750 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5800 3800 50  0001 C CNN
F 3 "" H 5800 3800 50  0000 C CNN
	1    5800 3800
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R3
U 1 1 58831931
P 5800 4000
F 0 "R3" V 5700 4050 50  0000 L CNN
F 1 "0R" V 5800 3950 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5800 4000 50  0001 C CNN
F 3 "" H 5800 4000 50  0000 C CNN
	1    5800 4000
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R4
U 1 1 588319B9
P 5800 4200
F 0 "R4" V 5700 4250 50  0000 L CNN
F 1 "0R" V 5800 4150 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5800 4200 50  0001 C CNN
F 3 "" H 5800 4200 50  0000 C CNN
	1    5800 4200
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R5
U 1 1 58831A3B
P 5800 4400
F 0 "R5" V 5700 4450 50  0000 L CNN
F 1 "0R" V 5800 4350 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5800 4400 50  0001 C CNN
F 3 "" H 5800 4400 50  0000 C CNN
	1    5800 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	6500 3600 5900 3600
Wire Wire Line
	5700 3600 5150 3600
Wire Wire Line
	5150 3800 5700 3800
Wire Wire Line
	5900 3800 6500 3800
Wire Wire Line
	5150 4000 5700 4000
Wire Wire Line
	5900 4000 6500 4000
Wire Wire Line
	5150 4400 5700 4400
Wire Wire Line
	5900 4400 6500 4400
Wire Wire Line
	5900 4200 6500 4200
Wire Wire Line
	5700 4200 5150 4200
$EndSCHEMATC
