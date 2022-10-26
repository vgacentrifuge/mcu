# MCU

This repository contains the code for the MCU.

## Building

Use [Simplicity Studio](https://www.silabs.com/developers/simplicity-studio).

## Flashing

Use Simplicity Studio, or download J-Link [from here](https://www.segger.com/downloads/jlink/)
and use J-Flash (Lite?) to program the MCU.

Remember to set the development board to AEM before trying to flash.

### Flashing the MCU on the PCB
This is done through a Giant Gecko Starter Kit.
The general steps are described [here
(https://community.silabs.com/s/article/debugging-an-efm32-product-on-a-custom-board?language=en_US), and go like follows:
 - Use the Giant Gecko with a large 20-pin connector. The box is marked "Programmer" with a yellow Post-it.
 - Power on the PCB and the Giant Gecko Starter Kit (by connecting it to a PC).
 - Connect them to each other using the large 20-pin to small 20-pin debug connector.
 - Make sure the Giant Gecko Starter Kit is set to AEM (DBG might work as well). 
 - Open Simplicity Commander, connect to the Starter Kit. 
 - In the "Kit" menu, select *Debug Mode*: *OUT*.
 - Go to the Flash menu, and try flashing.
 - If everything went well, go to the "SWO Terminal" and press *Start* to see debug output from the PCB MCU.

### Linux
Even if you use Simplicity Studio, you may still need to download the JLink
tar and do as its readme says (copy some udev rules file) to get JLink to work.

