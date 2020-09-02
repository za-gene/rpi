#pragma once
class FallingButton {

	public:
		FallingButton(int pin);
		bool falling();

	private:
		int _pin;
		char zone; // 0 = high state, 1 = falling, 2 = low state, 3 = rising
		bool low();
		bool high();
		unsigned long started;
};
