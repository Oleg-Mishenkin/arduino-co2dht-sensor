# arduino uno CO2, temperature and humidity sensor
## Scheme
![Scheme](/images/scheme.png)

## Components
* Display: MT-16S2H (16x2 pixels) 
* Temperature & humidity: DHT22
* CO2: MH-Z19
* Resistor 10k Ohm

## Features
Based on CO2 concentration there is LCD backlight control. When PPM >= 800 and PPM <= 1200 is starts to illuminate. When it's greater than 1200, LED starts to 	
periodically blind.
