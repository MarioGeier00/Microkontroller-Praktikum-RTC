# Microkontroller-Praktikum-RTC
Using the realtime clock of the STM32 to display the date and time value and an I²C display.

## Used components
- NUCLEO-F413ZH
- [Freenove LCD1602](https://freenove.com/fnk0079/)  

## Project description
- Use the Real-Time-Clock RTC of the STM32  
- Display the time every second in the format `TT:MM - HH:MM:SS`  
- Using the UART, the time can be modified  

## Mögiche TODOs:
- UART Interrupt, statt Polling
- DMA
--> Meiner Meinung nach ist das nicht nötig.
- Überprüfen ob Schaltjahr?
- Überprüfen ob Monate mit Datum übereinstimmt (Januar hat 31 Tage, Februar 28 Tage oder 29 Tage, März hat 31 Tage, April 30 Tage usw.)
