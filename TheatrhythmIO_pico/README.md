# FF Theatrhythm IO ~ pico edition ~

This is a PoC of the firmware running on Rpi pico

## Known issues

Because of pico being a 3.3v microcontroller while rgb lights are 5v, lights remain a dim red.

Please use mosfets and adapt code to fix the issue.

## Build Instruction (using pico sdk and MinGW)

- mkdir build
- cd build
- cmake .. -G "MinGW Makefiles"
- make
