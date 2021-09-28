extern _idt
extern _ZN5Sauce10Interrupts28Interrupt_SubRoutine_HandlerEm

%macro PUSHALL 0
    push rax
    push rcx
    push rdx
    push r8
    push r9
    push r10
    push r11
%endmacro

%macro POPALL 0
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdx
    pop rcx
    pop rax
%endmacro


%macro isr 1
    isr%1:
        PUSHALL
        mov rdi, %1
        call _ZN5Sauce10Interrupts28Interrupt_SubRoutine_HandlerEm
        POPALL
        iretq
    GLOBAL isr%1
%endmacro

idtDescriptor:
    dw 4095
    dq _idt


isr 0
isr 1

Load_Interrupt_Descriptor_Table:
    lidt[idtDescriptor]
    sti
    ret
    GLOBAL Load_Interrupt_Descriptor_Table