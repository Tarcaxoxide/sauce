#include<Sauce/Kernel.hpp>
#include<Sauce/Utilities/Conversion.hpp>
#include<Sauce/Memory/efiMemory.hpp>
#include<Sauce/Memory/efiMemory.h>
#include<Sauce/Memory/Memory.hpp>
#include<Sauce/Memory/Bitmap.hpp>
#include<Sauce/Memory/PageFrameAllocator.hpp>
#include<Sauce/Memory/PageMapIndexer.hpp>
#include<Sauce/Memory/Paging.hpp>
#include<Sauce/IO/Panic.hpp>
#include<Sauce/IO/IO.hpp>
#include<Sauce/IO/ACPI.hpp>
#include<Sauce/IO/PCI.hpp>
#include<Sauce/Math.hpp>
#include<Sauce/Memory/Heap.hpp>
#include<Sauce/Interrupts/PIT.hpp>
#include<Sauce/Memory/DynamicArray.hpp>
#include<Sauce/UserLand/VirtualMachine.hpp>
#include<Sauce/IO/Debug/Serial.hpp>
#include<Sauce/Graphics/Terminal.hpp>
#include<Sauce/Graphics/Shell.hpp>
#include<Sauce/Graphics/Font.hpp>
#include<Sauce/Global/Global.hpp>

namespace Sauce{
    Kernel_cl* Kernel_cl::Self=NULL; // pointer to the active kernel to be used by the kernel 
                            //when being updated by the hardware (Example: interrupts)
    bool ReadyToDraw=true;
    Kernel_cl::Kernel_cl(DataStructure* DFBL){
        Sauce::IO::Debug::SetNestLevel(0);
        asm volatile("cli");
        Sauce::IO::Debug::Print_Call("Kernel_cl::Kernel_cl",Sauce::IO::Debug::KERNEL,true);
        this->DFBL=DFBL;
        if(Self == NULL)Self=this;
        
        Prep_GlobalAllocator();
        Prep_VirtualAddresses();
        Prep_GDT();
        
        Sauce::Interrupts::PIT::SetDivisor(65535/6);
        Sauce::Memory::InitalizeHeap((void*)0x0000100000000000,0x10);

        Prep_Interrupts();
        asm volatile("sti");
        Prep_IO();
        asm volatile("cli");
        Sauce::Global::Terminal=new Sauce::Graphics::Terminal_cl((size_t)(DFBL->FrameBuffer->Height*DFBL->FrameBuffer->Width),(size_t)DFBL->FrameBuffer->PixelsPerScanLine);
        Sauce::Global::Shell=new Sauce::Graphics::Shell_cl({DFBL->FrameBuffer->PixelsPerScanLine,DFBL->FrameBuffer->Height,0},{0,0,0});
        Sauce::Global::Mouse=new Sauce::Graphics::Mouse_cl({DFBL->FrameBuffer->PixelsPerScanLine/2,DFBL->FrameBuffer->Height/2,0});
        Sauce::IO::outb(PIC1_DATA,0b11111000);
        Sauce::IO::outb(PIC2_DATA,0b11101111);
        Sauce::Global::Terminal->Clear();
        Prep_ACPI();
        PreLoop();
        MainLoop();
        Sauce::IO::Debug::Print_Return("this",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::PreLoop(){
        Sauce::IO::Debug::SetNestLevel(0);
        Sauce::IO::Debug::Print_Call("Kernel_cl::PreLoop",Sauce::IO::Debug::KERNEL,true);
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::MainLoop(){
        Sauce::IO::Debug::SetNestLevel(0);
        Sauce::IO::Debug::Print_Call("Kernel_cl::MainLoop",Sauce::IO::Debug::KERNEL,true);
        while(true){
            AcceptingInterrupts(100);
            DrawUI();
        }
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::Prep_GlobalAllocator(){
        Sauce::IO::Debug::SetNestLevel(0);
        Sauce::IO::Debug::Print_Call("Kernel_cl::Prep_GlobalAllocator",Sauce::IO::Debug::KERNEL,true);
        Sauce::Global::PageFrameAllocator = Sauce::Memory::PageFrameAllocator_cl();
        mMapEntries = DFBL->mMapSize/DFBL->mDescriptorSize;
        Sauce::Global::PageFrameAllocator.ReadEfiMemoryMap((Sauce::Memory::EFI_MEMORY_DESCRIPTOR*)DFBL->mMap,DFBL->mMapSize,DFBL->mDescriptorSize);
        kernelSize = ((uint64_t)&_KernelEndRef)-((uint64_t)&_KernelStartRef);
        kernelPages = (uint64_t)kernelSize/4096 +1;
        Sauce::Global::PageFrameAllocator.LockPages(&_KernelStartRef,kernelPages);
        PML4 = (Sauce::Memory::PageTable*)Sauce::Global::PageFrameAllocator.RequestPage();
        Sauce::Memory::memset(PML4,0,0x1000);
        Sauce::Global::PageTableManager = Sauce::Memory::PageTableManager_cl(PML4);
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::Prep_VirtualAddresses(){
        Sauce::IO::Debug::SetNestLevel(0);
        Sauce::IO::Debug::Print_Call("Kernel_cl::Prep_VirtualAddresses",Sauce::IO::Debug::KERNEL,true);
        for(uint64_t t=0;t<Sauce::Memory::GetMemorySize((Sauce::Memory::EFI_MEMORY_DESCRIPTOR*)DFBL->mMap,mMapEntries,DFBL->mDescriptorSize);t+=0x1000){
            Sauce::Global::PageTableManager.MapMemory((void*)t,(void*)t);
        }
        DFBL->fbBase = (uint64_t)DFBL->FrameBuffer->BaseAddress;
        DFBL->fbSize = (uint64_t)DFBL->FrameBuffer->BufferSize + 0x1000;
        Sauce::Global::PageFrameAllocator.LockPages((void*)DFBL->fbBase,DFBL->fbSize/0x1000 +1);
        for(uint64_t t=DFBL->fbBase;t<DFBL->fbBase+DFBL->fbSize;t+=0x1000){
            Sauce::Global::PageTableManager.MapMemory((void*)t,(void*)t);
        }
        asm volatile("mov %0, %%cr3" : : "r" (PML4));
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::Prep_GDT(){
        Sauce::IO::Debug::SetNestLevel(0);
        Sauce::IO::Debug::Print_Call("Kernel_cl::Prep_GDT",Sauce::IO::Debug::KERNEL,true);
        gdtDescriptor.Size= sizeof(Sauce::GDT::GDT_st)-1;
        gdtDescriptor.Offset= (uint64_t)&Sauce::GDT::DefaultGDT;
        LoadGDT(&gdtDescriptor);
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::Add_Interrupt(void* Interrupt_Handler,uint8_t Interrupt_Number,uint8_t type_attr,uint8_t selector){
        Sauce::IO::Debug::Print_Call("Kernel_cl::Add_Interrupt",Sauce::IO::Debug::KERNEL,true);
        Sauce::Interrupts::IDTDescriptorEntry* Interrupt = (Sauce::Interrupts::IDTDescriptorEntry*)(idtr.Offset + Interrupt_Number * sizeof(Sauce::Interrupts::IDTDescriptorEntry));
        Interrupt->SetOffset((uint64_t)Interrupt_Handler);
        Interrupt->type_attr = type_attr;
        Interrupt->selector=selector;
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::Prep_Interrupts(){
        Sauce::IO::Debug::SetNestLevel(0);
        Sauce::IO::Debug::Print_Call("Kernel_cl::Prep_Interrupts",Sauce::IO::Debug::KERNEL,true);
        idtr.Limit = 0x0FFF;
        idtr.Offset= (uint64_t)Sauce::Global::PageFrameAllocator.RequestPage();

        Add_Interrupt((void*)&Sauce::Interrupts::PageFault_handler,0xE,IDT_TA_InterruptGate,0x08);
        Sauce::IO::Debug::Print_Detail("PageFault_handler",Sauce::IO::Debug::KERNEL,true);
        Add_Interrupt((void*)&Sauce::Interrupts::DoubleFault_handler,0x8,IDT_TA_InterruptGate,0x08);
        Sauce::IO::Debug::Print_Detail("DoubleFault_handler",Sauce::IO::Debug::KERNEL,true);
        Add_Interrupt((void*)&Sauce::Interrupts::GeneralProtectionFault_handler,0xD,IDT_TA_InterruptGate,0x08);
        Sauce::IO::Debug::Print_Detail("GeneralProtectionFault_handler",Sauce::IO::Debug::KERNEL,true);
        Add_Interrupt((void*)&Sauce::Interrupts::KeyboardInterrupt_handler,0x21,IDT_TA_InterruptGate,0x08);
        Sauce::IO::Debug::Print_Detail("KeyboardInterrupt_handler",Sauce::IO::Debug::KERNEL,true);
        Add_Interrupt((void*)&Sauce::Interrupts::MouseInterrupt_handler,0x2C,IDT_TA_InterruptGate,0x08);
        Sauce::IO::Debug::Print_Detail("MouseInterrupt_handler",Sauce::IO::Debug::KERNEL,true);
        Add_Interrupt((void*)&Sauce::Interrupts::PITInterrupt_handler,0x20,IDT_TA_InterruptGate,0x08);
        Sauce::IO::Debug::Print_Detail("PITInterrupt_handler",Sauce::IO::Debug::KERNEL,true);

        asm volatile("lidt %0" : : "m" (idtr));
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::Prep_IO(){
        Sauce::IO::Debug::SetNestLevel(0);
        Sauce::IO::Debug::Print_Call("Kernel_cl::Prep_IO",Sauce::IO::Debug::KERNEL,true);
        Sauce::Interrupts::RemapPic();
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::Prep_ACPI(){
        Sauce::IO::Debug::SetNestLevel(0);
        Sauce::IO::Debug::Print_Call("Kernel_cl::Prep_ACPI",Sauce::IO::Debug::KERNEL,true);
        Sauce::IO::ACPI::SDTHeader* xsdt = (Sauce::IO::ACPI::SDTHeader*)DFBL->rsdp->XSDT_Address;
        Sauce::IO::ACPI::MCFGHeader* mcfg = (Sauce::IO::ACPI::MCFGHeader*)Sauce::IO::ACPI::FindTable(xsdt,(char*)"MCFG");
        Sauce::IO::EnumeratePCI(mcfg);
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::oNotify_Of_KeyPress(Sauce::IO::Keyboard_st xKeyboard){
        Sauce::IO::Debug::Print_Call("Kernel_cl::oNotify_Of_KeyPress",Sauce::IO::Debug::KERNEL,true);
        ReadyToDraw=false;
        if(xKeyboard.Press){
            switch(xKeyboard.Key){
                case 0xD6:{
                    Sauce::Global::Shell->PutChar('\n');
                    Sauce::Global::Shell->PutChar('\r');
                }break;
                case 0x1C:{
                    Sauce::Global::Shell->PutChar('\b');
                }break;
                default:{   
                    if(xKeyboard.visible){
                        Sauce::Global::Shell->PutChar(xKeyboard.Display);
                    }else{
                       Sauce::IO::Debug::Print_Detail(Sauce::Convert::HexToString(xKeyboard.Key),Sauce::IO::Debug::KERNEL,true);
                    }
                }break;
            }
        }
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    int testcount=0;
    Point64_t CurrentMouseCursorPosition{0,0,0};
    void Kernel_cl::oNotify_Of_Mouse(Sauce::IO::Mouse_st* xMouse){
        Sauce::IO::Debug::Print_Call("Kernel_cl::oNotify_Of_Mouse",Sauce::IO::Debug::KERNEL,true);
        ReadyToDraw=false;
        
        if(xMouse->Position->X > (DFBL->FrameBuffer->PixelsPerScanLine-Sauce::Global::Mouse->Size().X))xMouse->Position->X=(DFBL->FrameBuffer->PixelsPerScanLine-Sauce::Global::Mouse->Size().X);
        if(xMouse->Position->X < 0)xMouse->Position->X=0;
        if(xMouse->Position->Y > (DFBL->FrameBuffer->Height-Sauce::Global::Mouse->Size().Y))xMouse->Position->Y=(DFBL->FrameBuffer->Height-Sauce::Global::Mouse->Size().Y);
        if(xMouse->Position->Y < 0)xMouse->Position->Y=0;

        if(CurrentMouseCursorPosition.X != xMouse->Position->X || CurrentMouseCursorPosition.Y != xMouse->Position->Y){
            CurrentMouseCursorPosition = Point64_t{xMouse->Position->X,xMouse->Position->Y,xMouse->Position->Z};
            Sauce::Global::Mouse->Move(CurrentMouseCursorPosition);
        }
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::DrawUI(){
        Sauce::IO::Debug::Print_Call("Kernel_cl::DrawUI",Sauce::IO::Debug::KERNEL,true);
        if(!ReadyToDraw){
            ReadyToDraw=true;
            Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
            return;
        }
        
        Sauce::Global::Terminal->CopyFrom(Sauce::Global::Shell);
        Sauce::Global::Terminal->CopyFrom(Sauce::Global::Mouse);
        Sauce::Global::Terminal->CopyTo(DFBL->FrameBuffer->BaseAddress,(size_t)(DFBL->FrameBuffer->Height*DFBL->FrameBuffer->Width),(size_t)DFBL->FrameBuffer->PixelsPerScanLine);
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::AcceptingInterrupts(size_t TimeSpan){
        Sauce::IO::Debug::Print_Spammy_Call("Kernel_cl::AcceptingInterrupts",Sauce::IO::Debug::KERNEL,true);
        asm volatile("sti");
        Sauce::Interrupts::PIT::Sleep(TimeSpan);
        asm volatile("cli");
        Sauce::IO::Debug::Print_Spammy_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::Notify(Sauce::Interrupts::InterruptDataStruct InterruptData){
        Sauce::IO::Debug::Print_Spammy_Call("Kernel_cl::Notify",Sauce::IO::Debug::KERNEL,true);
        asm volatile("cli");
        switch(InterruptData.TypeCode){
            case Sauce::Interrupts::InterruptTypeCode::ITC__Mouse:{
                Sauce::IO::Debug::Print_Spammy_Detail("ITC__Mouse",Sauce::IO::Debug::KERNEL,true);
                Sauce::IO::HandlePS2Mouse(InterruptData.RawInterruptData);
                Self->oNotify_Of_Mouse(Sauce::IO::ProcessMousePacket());
            }break;
            case Sauce::Interrupts::InterruptTypeCode::ITC__Keyboard:{
                Sauce::IO::Debug::Print_Spammy_Detail("ITC__Keyboard",Sauce::IO::Debug::KERNEL,true);
                Self->oNotify_Of_KeyPress(Sauce::IO::Code_To_Key(Sauce::IO::Translate_KeyCode(InterruptData.RawInterruptData)));
            }break;
            case Sauce::Interrupts::InterruptTypeCode::ITC__NULL:{
                Sauce::IO::Debug::Print_Spammy_Detail("ITC__NULL",Sauce::IO::Debug::KERNEL,true);
            }break;
            case Sauce::Interrupts::InterruptTypeCode::ITC__Time:{
                Sauce::IO::Debug::Print_Spammy_Detail("ITC__Time",Sauce::IO::Debug::KERNEL,true);
            }break;
        }
        Sauce::IO::Debug::Print_Spammy_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
    void Kernel_cl::Stop(bool ClearInterrupts){
        Sauce::IO::Debug::Print_Call("Kernel_cl::Stop",Sauce::IO::Debug::KERNEL,true);
        while(true){
            if(ClearInterrupts)asm volatile("cli");
            asm volatile("hlt");
        }
        Sauce::IO::Debug::Print_Return("void",Sauce::IO::Debug::KERNEL,true);
    }
};