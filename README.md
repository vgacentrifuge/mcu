# MCU

This repository contains the code for the MCU.

## Building

Use [Simplicity Studio](https://www.silabs.com/developers/simplicity-studio).

## Flashing

Use Simplicity Studio, or download J-Link [from here](https://www.segger.com/downloads/jlink/)
and use J-Flash (Lite?) to program the MCU.

Remember to set the development board to AEM before trying to flash.

### Linux
Even if you use Simplicity Studio, you may still need to download the JLink
tar and do as its readme says (copy some udev rules file) to get JLink to work.