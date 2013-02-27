Unit tests for Strong ARM.

This compiles into a binary that can be loaded into real hardware (STM32F4DISCOVERY right now).
When the binary runs, it runs through all the unit tests, and returns results on STDOUT.

Currently, STDOUT is handled with a breakpoint in an empty function which is passed the string(s) that would normally go to STDOUT. 
This will be replaced with semihosting.
