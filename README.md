# Microkontroller-Praktikum-RTC
Using the realtime clock of the STM32 to display the date and time value and an I²C display.

## Used components
- NUCLEO-F413ZH
- [Freenove LCD1602](https://freenove.com/fnk0079/)  

## Project description
- Use the Real-Time-Clock RTC of the STM32  
- Display the time every second in the format `TT:MM - HH:MM:SS`  
- Using the UART, the time can be modified  


## RTC
Die Real-Time Clock kann über das Terminal gesetzt werden.
Real-Time Clock ist immer ein Schaltjahr.
Es wird geprüft, ob die Eingaben korrekt sind (Zahlen, Zeichen)
Es wird geprüft ob die Tage richtig sind (31.02. gibt es nicht)


## Mögliche TODOs:
- UART Interrupt, statt Polling?
- DMA ?? Kein Plan.

Rand0x: Meiner Meinung nach ist das nicht nötig, da Projekt grundsätzlich fertig.
Damit man weniger in der main stehen hat, müsste man das UART als Interrupt machen.
Das könnte etwas umständlich werden.
