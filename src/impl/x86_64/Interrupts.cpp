#include "Interrupts.hpp"

extern Sauce::Interrupts::IDT64 _idt[256];
extern uint64_t isr1;
extern "C" void loadIDT();



namespace Sauce{
    namespace Interrupts{
        void MapIDT(size_t index,uint64_t &This_isr){
            _idt[index].offset_low = ((uint16_t)(((uint64_t)&This_isr & 0x000000000000ffff)));
			_idt[index].selector = 0x08;
			_idt[index].ist = 0;
			_idt[index].types_attr = 0x8e;
			_idt[index].offset_mid  = ((uint16_t)(((uint64_t)&This_isr & 0x00000000ffff0000) >> 16));
			_idt[index].offset_high = ((uint32_t)(((uint64_t)&This_isr & 0xffffffff00000000) >> 32));
			_idt[index].zero = 0;
        }
        void InitializeIDT(){
            MapIDT(1,isr1);
            Sauce::IO::RemapPic();
            Sauce::IO::outb(0x21,0xfd);
            Sauce::IO::outb(0xa1,0xff);
            loadIDT();
        }

        extern "C" void isr1_handler(){
            uint8_t input = 0;
            do {
              if(Sauce::IO::inb(0x60) != input) {
                input = Sauce::IO::inb(0x60);

                if(input > 0) {
                    uint16_t Xinput = Sauce::Keyboard::Translate_KeyCode(input);
                    if(Xinput != NULL){
                        Sauce::Keyboard::KeyboardKey Xkey = Sauce::Keyboard::CodeToKey(Xinput);
                        if(Xkey.Key != NULL)NotifyKernelOfKeyPress(Xkey);
                    }
                }
              }
            } while(input != 0);

            Sauce::IO::outb(0x20,0x20);
            Sauce::IO::outb(0xa0,0x20);
        }
    };
};