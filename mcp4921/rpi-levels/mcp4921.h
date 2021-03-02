#pragma once

#include <stdint.h>

class mcp4921 {
	public:
		mcp4921();
		~mcp4921();
		void write(uint16_t value);
};

