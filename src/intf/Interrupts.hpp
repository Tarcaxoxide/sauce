#pragma once

#include<stdint.h>
#include<stddef.h>
#include "IO.hpp"
#include "Keyboard.hpp"

namespace Sauce{
    namespace Interrupts{
		struct IDT64{
			uint16_t offset_low;
			uint16_t selector;
			uint8_t ist;
			uint8_t types_attr;
			uint16_t offset_mid;
			uint32_t offset_high;
			uint32_t zero;
		};
		void InitializeIDT();
		void MapIDT(size_t index,uint64_t (*This_isr));
	};
};

extern void NotifyKernelOfKeyPress(Sauce::Keyboard::KeyboardKey _Key);