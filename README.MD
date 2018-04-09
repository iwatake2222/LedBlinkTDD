# AVR LED Blink project by TDD
- Develop the very basic LED blink project by TDD.
- Unit test runs both on Host and Target device

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

# How to port Unity to tiny microcontroller
- Copy all Unity source code(./LedBlinkTDD/Unity/) except for sample code.
- Implement _output_ function which behaves like `putchar()`
	- I use `Uart0_send()` in this project
- Set compile option (project property -> C Compiler -> Symboles(-D))
	- `UNITY_OUTPUT_CHAR=outputChar`
	- `UNITY_OUTPUT_CHAR_HEADER_DECLARATION=(*outputChar)(char)`
- Define and create global function pointer variable named `outputChar`
	- `void (*outputChar)(char) = Uart0_send;`
