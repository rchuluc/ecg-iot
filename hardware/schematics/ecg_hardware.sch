EESchema Schematic File Version 4
EELAYER 26 0
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
L Sensor:DHT11 U?
U 1 1 5BE72BE6
P 6150 1550
F 0 "U?" V 5783 1550 50  0000 C CNN
F 1 "DHT11" V 5874 1550 50  0000 C CNN
F 2 "" H 6300 1800 50  0001 C CNN
F 3 "http://akizukidenshi.com/download/ds/aosong/DHT11.pdf" H 6300 1800 50  0001 C CNN
	1    6150 1550
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5BE72DAB
P 6450 1850
F 0 "R?" V 6243 1850 50  0000 C CNN
F 1 "R" V 6334 1850 50  0000 C CNN
F 2 "" V 6380 1850 50  0001 C CNN
F 3 "~" H 6450 1850 50  0001 C CNN
	1    6450 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	6150 1850 6300 1850
Wire Wire Line
	6600 1850 6600 1450
Wire Wire Line
	6600 1450 6450 1450
$Comp
L Connector:Conn_01x06_Female J?
U 1 1 5BE72F53
P 7050 1450
F 0 "J?" V 6990 1062 50  0000 R CNN
F 1 "AD8232 Module" V 6899 1062 50  0000 R CNN
F 2 "" H 7050 1450 50  0001 C CNN
F 3 "~" H 7050 1450 50  0001 C CNN
	1    7050 1450
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
