/* just test compilation */

#include <stdio.h>

#include "notes.h"

int main() {
	for(int i = 0; i < num_notes; i++) {
		struct notes_t n = notes[i];
		printf("%3d %4.4s %5d\n", i, n.name, n.freq);
	}
	return 0;
}
