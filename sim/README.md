# MCU Simulator

This is a program that simulates various pieces of hardware on the PCB,
to allow development of the machine's menus on a laptop.

Thh way it works is that a few of the `.c` files from
the mcu codebase is compiled together with a bunch of `.c` files from
this folder's `src/`. The files are compiled using headers both from `inc/` here,
and from the `../inc/` and `../vendored/` folders.

We include the `.c`-files from the mcu codebase that contain busines logic,
such as menus.
You only need to add headers in `/sim/inc/`, if the c files use
Gecko SDK headers. The functions from the SDK we want to mock are in `src/mocks.c`.

For almost all hardware use, we don't want to mock the Gecko SDK, but instead
emulate a higher level API defined in the mcu codebase, such as `sd_card.c`, `lcd.c` etc.
This makes mocking much easier, and we make our own versions of these files in the `sim/src/` folder.

## Dependencies
The simulator uses raylib, which is vendored, and should be compiled by the Makefile.
You do however need the `zig` compiler, version 0.10.0, which is the easiest cross platform way
to compile C, and also raylib's build system.

## Build and run

``` sh
make
./build/mcu_sim # Or .exe if windows
```

