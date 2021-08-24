#include <Memory.hpp>

extern uint8_t MemoryRegionCount;
extern uint16_t MemoryMapAddress;

extern "C" uint64_t GetFreeStack(); // Get the remaining space of the kernel stack
extern "C" uint64_t GetMaxStack(); // Get the total size of the kernel stack

namespace Sauce{
    namespace Memory{
        void PrintMemoryMap(MemoryMapEntry* MemoryMap){
            Sauce::Terminal::String("[Base:");
            Sauce::Terminal::String(Sauce::Convert::To_String::From_Integer(MemoryMap->BaseAddress));
            Sauce::Terminal::String(";");
            Sauce::Terminal::String("Length:");
            Sauce::Terminal::String(Sauce::Convert::To_String::From_Integer(MemoryMap->Region_Length));
            Sauce::Terminal::String(";");
            Sauce::Terminal::String("Type:");
            Sauce::Terminal::String(Sauce::Convert::To_String::From_Integer(MemoryMap->Region_Type));
            Sauce::Terminal::String(";");
            Sauce::Terminal::String("Attributes:");
            Sauce::Terminal::String(Sauce::Convert::To_String::From_Integer(MemoryMap->ExtendedAttributes));
            Sauce::Terminal::String("]");
        }
        void PrintMemoryMap_All(){
            for (uint8_t A = 0; A < MemoryRegionCount;A++){
                Sauce::Memory::MemoryMapEntry* memMap = (Sauce::Memory::MemoryMapEntry*)MemoryMapAddress;
                memMap += A;
                Sauce::Memory::PrintMemoryMap(memMap);
                Sauce::Terminal::String("\n\r");
            }
        }
        MemoryMapEntryMap GetMemoryRegions(uint32_t Type){
            MemoryMapEntry* MemoryRegions[24];
            MemoryMapEntryMap ReturnValue ={MemoryRegions,0};
            uint8_t B=0;
            for (uint8_t A = 0; A < MemoryRegionCount;A++){
                Sauce::Memory::MemoryMapEntry* memMap = (Sauce::Memory::MemoryMapEntry*)MemoryMapAddress;
                memMap += A;
                if(memMap->Region_Type == Type){
                    ReturnValue.MemoryMapEntries[ReturnValue.MemoryMapEntryCount] = memMap;
                    ReturnValue.MemoryMapEntryCount++;
                }
            }
            return ReturnValue;
        }
    };
    namespace Memory{
        MemorySegmentHeader* FirstFreeMemorySegment;
        void InitializeHead(uint64_t HeapAddress,uint64_t HeapLength){
            FirstFreeMemorySegment = (MemorySegmentHeader*)HeapAddress+1;
            FirstFreeMemorySegment->MemoryLength=HeapLength-sizeof(MemorySegmentHeader);
            FirstFreeMemorySegment->NextSegment=0;
            FirstFreeMemorySegment->PreviousSegment=0;
            FirstFreeMemorySegment->NextFreeSegment=0;
            FirstFreeMemorySegment->PreviousFreeSegment=0;
            FirstFreeMemorySegment->Free=true;
        }
        void* malloc(uint64_t size){
            uint64_t remainder = size % 8;
            size -= remainder;
            size+=(8*(remainder != 0));

            MemorySegmentHeader* currentMemorySegment = FirstFreeMemorySegment;
            
            while(true){
                if(currentMemorySegment->MemoryLength >= size){
                    if(currentMemorySegment->MemoryLength > size + sizeof(MemorySegmentHeader)){
                        MemorySegmentHeader* newSegmentHeader = (MemorySegmentHeader*)((uint64_t)currentMemorySegment + sizeof(MemorySegmentHeader)+size);
                        newSegmentHeader->Free=true;
                        newSegmentHeader->MemoryLength=((uint64_t)currentMemorySegment->MemoryLength - (sizeof(MemorySegmentHeader)+size));
                        newSegmentHeader->NextFreeSegment = currentMemorySegment->NextFreeSegment;
                        newSegmentHeader->NextSegment = currentMemorySegment->NextSegment;
                        newSegmentHeader->PreviousSegment = currentMemorySegment;
                        newSegmentHeader->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;
                        currentMemorySegment->NextFreeSegment = newSegmentHeader;
                        currentMemorySegment->NextSegment = newSegmentHeader;
                        currentMemorySegment->MemoryLength=size;
                    }
                    if(currentMemorySegment == FirstFreeMemorySegment){
                        FirstFreeMemorySegment = currentMemorySegment->NextFreeSegment;
                    }
                    currentMemorySegment->Free=false;

                    if(currentMemorySegment->PreviousFreeSegment != 0)currentMemorySegment->PreviousFreeSegment->NextFreeSegment = currentMemorySegment->NextFreeSegment;
                    if(currentMemorySegment->NextFreeSegment != 0)currentMemorySegment->NextFreeSegment->PreviousFreeSegment =  currentMemorySegment->PreviousFreeSegment;
                    if(currentMemorySegment->PreviousSegment != 0)currentMemorySegment->PreviousSegment->NextFreeSegment = currentMemorySegment->NextFreeSegment;
                    if(currentMemorySegment->NextSegment != 0)currentMemorySegment->NextSegment->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;
                    return currentMemorySegment+1;
                }
                if(currentMemorySegment->NextFreeSegment == 0){
                    return 0;
                }
                currentMemorySegment = currentMemorySegment->NextFreeSegment;
            }
            return 0;
        }
        void free(void* address){
            MemorySegmentHeader* currentMemorySegment = ((MemorySegmentHeader*)address)-1;
            currentMemorySegment->Free=true;
            if(currentMemorySegment < FirstFreeMemorySegment)FirstFreeMemorySegment = currentMemorySegment;

            if(currentMemorySegment->NextFreeSegment != 0){
                if(currentMemorySegment->NextFreeSegment->PreviousFreeSegment < currentMemorySegment)currentMemorySegment->NextFreeSegment->PreviousFreeSegment = currentMemorySegment;
            }
            if(currentMemorySegment->PreviousFreeSegment != 0){
                if(currentMemorySegment->PreviousFreeSegment->NextFreeSegment > currentMemorySegment)currentMemorySegment->PreviousFreeSegment->NextFreeSegment = currentMemorySegment;
            }
            if(currentMemorySegment->NextSegment != 0){
                currentMemorySegment->NextSegment->PreviousSegment=currentMemorySegment;
                if(currentMemorySegment->NextSegment->Free)CombinedSegments(currentMemorySegment,currentMemorySegment->NextSegment);
            }
            if(currentMemorySegment->PreviousSegment != 0){
                currentMemorySegment->PreviousSegment->NextSegment=currentMemorySegment;
                if(currentMemorySegment->PreviousSegment->Free)CombinedSegments(currentMemorySegment,currentMemorySegment->PreviousSegment);
            }
        }
        void CombinedSegments(MemorySegmentHeader* a,MemorySegmentHeader* b){
            if(a == 0 || b == 0)return;
            if(a < b){
                a->MemoryLength+=b->MemoryLength + sizeof(MemorySegmentHeader);
                a->NextSegment = b->NextSegment;
                a->NextFreeSegment = b->NextFreeSegment;
                b->NextSegment->PreviousSegment = a;
                b->NextSegment->PreviousFreeSegment = a;
                b->NextFreeSegment->PreviousFreeSegment = a;
            }else{
                b->MemoryLength+=a->MemoryLength + sizeof(MemorySegmentHeader);
                b->NextSegment = a->NextSegment;
                b->NextFreeSegment = a->NextFreeSegment;
                a->NextSegment->PreviousSegment = b;
                a->NextSegment->PreviousFreeSegment = b;
                a->NextFreeSegment->PreviousFreeSegment = b;
            }
        }
    };
};

