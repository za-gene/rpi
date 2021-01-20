#include "../../lua-5.4.2/src/lua.h"
#include "../../lua-5.4.2/src/lauxlib.h"
#include "../../lua-5.4.2/src/lualib.h"


#include <uart0.h>
#include <lfb.h>

void lua_stuff()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	// Work with lua API
	char * code = "print('Hello from lua')";
	if (luaL_loadstring(L, code) == LUA_OK) {
		if (lua_pcall(L, 0, 1, 0) == LUA_OK) {
			// If it was executed successfuly we
			// remove the code from the stack
			lua_pop(L, lua_gettop(L));
		}
	}

	lua_close(L);
}

void kernel_main()
{
	char msg[] = "I'll now echo back everything you type 1";

	lfb_init();
	fbputs(msg);

	// display an ASCII string on screen with PSF
	//lfb_print(80, 80, "Hello World!");

	// display a UTF-8 string on screen with SSFN
	//lfb_proprint(80, 120, "Hello 多种语言 Многоязычный többnyelvű World!");


	uart0_init();
	uart0_puts(msg);

	lua_stuff();

	// echo everything back
	while(1) {
		int c = uart0_getc();
		fbputchar(c);
		uart0_putchar(c);
	}
}
