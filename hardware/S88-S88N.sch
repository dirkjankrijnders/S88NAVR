EESchema Schematic File Version 2  date Tuesday, 2013 February 19 15:44:35
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "19 feb 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RJ45 J1
U 1 1 50FFDA68
P 2850 3450
F 0 "J1" H 3050 3950 60  0000 C CNN
F 1 "RJ45" H 2700 3950 60  0000 C CNN
F 2 "" H 2850 3450 60  0001 C CNN
F 3 "" H 2850 3450 60  0001 C CNN
	1    2850 3450
	0    -1   -1   0   
$EndComp
$Comp
L CONN_6 P1
U 1 1 50FFDA8F
P 4300 3400
F 0 "P1" V 4250 3400 60  0000 C CNN
F 1 "CONN_6" V 4350 3400 60  0000 C CNN
F 2 "" H 4300 3400 60  0001 C CNN
F 3 "" H 4300 3400 60  0001 C CNN
	1    4300 3400
	1    0    0    -1  
$EndComp
$Comp
L CONN_7 P2
U 1 1 50FFDA9E
P 4300 4300
F 0 "P2" V 4250 4300 60  0000 C CNN
F 1 "CONN_6" V 4350 4300 60  0000 C CNN
F 2 "" H 4300 4300 60  0001 C CNN
F 3 "" H 4300 4300 60  0001 C CNN
	1    4300 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3150 3800 3150
Wire Wire Line
	3800 3150 3800 4000
Wire Wire Line
	3800 4000 3950 4000
Wire Wire Line
	3950 3250 3750 3250
Wire Wire Line
	3750 3250 3750 4100
Wire Wire Line
	3750 4100 3950 4100
Wire Wire Line
	3950 3350 3700 3350
Wire Wire Line
	3700 3350 3700 4200
Wire Wire Line
	3700 4200 3950 4200
Wire Wire Line
	3950 3450 3650 3450
Wire Wire Line
	3650 3300 3650 4300
Wire Wire Line
	3650 4300 3950 4300
Wire Wire Line
	3950 3550 3600 3550
Wire Wire Line
	3600 3200 3600 4400
Wire Wire Line
	3600 4400 3950 4400
Wire Wire Line
	3950 3650 3550 3650
Wire Wire Line
	3550 3650 3550 4500
Wire Wire Line
	3550 4500 3950 4500
Wire Wire Line
	3300 3700 3800 3700
Connection ~ 3800 3700
Wire Wire Line
	3300 3600 3750 3600
Connection ~ 3750 3600
Wire Wire Line
	3300 3500 3700 3500
Connection ~ 3700 3500
Wire Wire Line
	3650 3300 3300 3300
Connection ~ 3650 3450
Wire Wire Line
	3300 3200 3600 3200
Connection ~ 3600 3550
Wire Wire Line
	3300 3800 3550 3800
Connection ~ 3550 3800
Wire Wire Line
	3950 4600 3500 4600
Wire Wire Line
	3500 4600 3500 3100
Wire Wire Line
	3500 3100 3300 3100
Wire Wire Line
	3300 3400 3750 3400
Connection ~ 3750 3400
$EndSCHEMATC
