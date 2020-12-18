#include <gpio.h>

#define LED PC13

class foo {
	public:
	       	foo() {};
		~foo() {};
};



int main()
{

	gpio_mode(LED, OUTPUT);

	while (1) {
	}

	return 0;
}

