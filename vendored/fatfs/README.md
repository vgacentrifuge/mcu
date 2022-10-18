# FatFS

This vendored library was taken from the [FatFS website](http://elm-chan.org/fsw/ff/00index_e.html)'s 
[FatFs sample projects for various platforms](http://elm-chan.org/fsw/ff/ffsample.zip), last updated April 17, 2021.

Among these examples, the `generic/` folder was used. The following changes were made to adapt the
media layer to use the Gecko SDK's SPIDRV api, all in the file `sdmm.c`:
 - Remove macros for manually setting DI, DO and CK high and low
 - Keep the CS_INIT(), CS_L() and CS_H() macros, since the code wants to 
   manually send a byte with CS HIGH to deselect the SD card.
   - However, asking SPISRV to let the application control CS causes
     init to hang for some reason. We therefore let SPIDRV handle it,
     and implement CS_L() and CS_H() as NO-OPs.
     This means the deselect() function no longer works,
     but since the SD card is alone on its SPI bus, that's fine.
 - Implement `dly_us`, using `sl_udelay` from the Gecko SDK.
 - Update `xmit_mmc` and `rcvr_mmc` to use SPIDRV from the Gecko SDK.
 - Update `disk_initalize()`
   - Set `dummyTxValue` to `0xFF`, since that was default for recieves.
   - Remove INIT macros for pins handled by SPIDRV
   
Finally, `ffconf.h` was changed like so:
```
 - #define FF_USE_LFN		0
 + #define FF_USE_LFN		1
```
Which tells FatFs to support long file names (longer than 8+3), and use global
memory to store them. This makes the library non-reentrant (single threaded only),
but that's fine for our use case.
It also required the addition of `ffunicode.c` from the FatFs zip file.

The new max size is 256 characters, which makes file entries much much larger,
so **place them in global memory**, either using a global variable, or a function `static` variable.
 