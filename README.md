# Microkontroller-Praktikum-RTC
Using the realtime clock of the STM32 to display the date and time value and an I²C display.

## Project description
- Use the Real-Time-Clock RTC of the STM32  
- Display the time every second in the format `TT:MM - HH:MM:SS`  
- Using the UART, the time can be modified  

## Used components
- NUCLEO-F413ZH
- [Freenove LCD1602](https://freenove.com/fnk0079/)  

## Tasks

### RTC & UART
- [x] sets the RTC with UART
- [x] checks the time input from UART
- [x] checks the date input from UART
- [x] uses interrupts to receive input
- [ ] ~does not require to type the symbols `:` and `-`~
- [x] considers leap years
- [ ] document and beautify RTC code

### Sync timer
- [x] uses interrupt to synchronize RTC value to the display
- [x] only synchronize data every second
- [x] use RTC alarm interrupt instead of timer interrupt

### I²C display
- [x] initializes display
- [x] use the I²C-2 and set to 100kHz clock frequency (max rating of display)
- [x] connect `PB -> SDA`, `PB9 -> SCL`, GND and a supply voltage of 5V
- [x] I²C communication error lights up the red onboard led
- [ ] check and document initialisation and transfer code

#### Reduce HAL_I2C_Transmit overhead
HAL_I2C_Transmit is expensive because of decreased I²C speed of the display!
- [x] send all characters to the display in one HAL_I2C_Transmit call
- [ ] only transfer changed chars
- [ ] use DMA Controller to transfer the data