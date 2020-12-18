#include <functional>
#include <string>


using namespace std;

#include <gpio.h>

//function<int()> life = []() { return 42; }; // this doesn't work

#define LED PB13

void delay() { delaysecsish(1); }

class foo {
	public:
	       	foo() { gpio_write(LED,1);  delay(); };
		~foo() { gpio_write(LED, 0); delay(); };
};



int main()
{
	//string foo = "hello";  // seems to cause compilation errors

	gpio_mode(LED, OUTPUT);


	while (1) {
		foo x; // test of destructors and constructors
	}

	return 0;
}

