# MCU

This repository contains the code for the MCU.

## Building

**Note**: The Makefile currently only works on Linux and macOS. Use WSL if you're on Windows.

Get the the ARM GNU Toolchain [here](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads),
or from your favorite package manager. Make sure that the binaries are on your PATH.

Clone the Gecko SDK in the project root:

```
$ git clone https://github.com/SiliconLabs/gecko_sdk.git --depth 1
```

Then compile the project using the Makefile:

```
$ make
```
