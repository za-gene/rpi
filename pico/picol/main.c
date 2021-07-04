#include "pico/stdlib.h"

#include "picol.h"

int main() {
	stdio_init_all();
	struct picolInterp interp;
	picolInitInterp(&interp);
	picolRegisterCoreCommands(&interp);
	while(1) {
		char clibuf[1024];
		int retcode;
		printf("picol> "); fflush(stdout);
		if (picolEchoedFgets(clibuf,1024,stdin) == NULL) return 0;
		retcode = picolEval(&interp,clibuf);
		if (interp.result[0] != '\0')
			printf("[%d] %s\n", retcode, interp.result);
	}
	return 0;
}
