# Group 4 - Digital Instrumentation
## Assignemnt 1
### Joystick pins used
| Joystick  | LSM9DS1     | 
| :-------- | ----------: | 
| UP        | PA4         | 
| DOWN      | PB0         |
| LEFT	    | PB5         | 
| RIGHT     | PC0         | 
| CENTER    | PC1         | 

### LED pin used
| LED       | LSM9DS1     | 
| :-------- | ----------: | 
| BLUE      | PA9         | 
| RED       | PB4         |
| GREEN	    | PC7         | 

## Assignment 4
**connections (using SPI2)**
| LSM9DS1             | STM32F302   | STM32F302 pin |
| :------------------ | :---------: | ------------: |
| GND                 | GND         | GND           |
| VDD                 | 3v3         | 3v3           |
| SDA	                | MOSI / D11  | PB15          |
| SCL                 | SCK / D13   | PB13          |
| SDO M + SDO AG      | MISO / D12  | PB14          |
| CS AG               | CS / D10    | PB6           |
| CS M                | D6          | PB10          |


## Assignment 6
* PA5 used for reading signal input
