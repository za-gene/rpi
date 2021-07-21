#pragma once

struct notes_t {
	const char* name;
	int freq;
};

extern struct notes_t notes[];
extern int num_notes;
