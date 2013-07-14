Unit tests for Strong ARM.

This compiles into a binary that can be loaded into real hardware (STM32F4DISCOVERY right now).
When the binary runs, it runs through all the unit tests, and returns results on STDOUT.

Currently, STDOUT is handled with a breakpoint in an empty function which is passed the string(s) that would normally go to STDOUT. 
This will be replaced with semihosting.


To compile for STM32F4DISCOVERY:
	* make

To compile for x86 (Cygwin's MinGW):
	* make CYGWIN_MINGW=1


To run tests on an STM32F4DISCOVERY or similiar:
	* make
	* openocd -f board/stm32f4discovery.cfg
	* make flash-and-debug
	* continue
	* GDB will hit a breakpoint when the program is trying to print a message.
	* Either the program says ALL TESTS PASSED along with some debug messages from
		the random module, or it will report an error.
