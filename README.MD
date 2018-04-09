# AVR LED Blink project by TDD
- Develop the very basic LED blink project by TDD.
- Unit test runs both on host PC and the target device

# Environment
- Target
	- Device: ATmega328P (not arduino)
	- IDE: Atmel Studio 7.0
- Host
	- Editor: VSCode (can be any)
	- System: MinGW on Windows10 (can be any)
	- Build tool: CMake
- Unit Test Framework
	- Unity (https://github.com/ThrowTheSwitch/Unity)

# How to run unit test on host
```sh:shellOnHost
cd LedBlinkTDD/BuildHost
cmake .. -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=DEBUG
make
./bin/LedBlinkTestHost.exe  -v
```

# How to run unit test on target(AVR)
- Unit test runs at the beginning of main
- It may be better to have nice compile switch (currently, it is `#if 1`)
	- When unit test is disabled(change compile switch to `#if 0`), Unity library is not linked

# How to port Unity to tiny microcontroller
- Copy all Unity source code(./LedBlinkTDD/Unity/) except for sample code.
- Implement _output_ function which behaves like `putchar()`
	- I use `Uart0_send()` in this project
- Set compile option (project property -> C Compiler -> Symboles(-D))
	- `UNITY_OUTPUT_CHAR=outputChar`
	- `UNITY_OUTPUT_CHAR_HEADER_DECLARATION=(*outputChar)(char)`
- Define and create global function pointer variable named `outputChar`
	- `void (*outputChar)(char) = Uart0_send;`
