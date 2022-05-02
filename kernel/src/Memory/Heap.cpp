#include<Sauce/Memory/Heap.hpp>

namespace Sauce{
    namespace Memory{
        void* heapBegin;
        void* heapEnd;
        HeapSegmentHeader* LastSegmentHeader;

        void HeapSegmentHeader::CombinedForward(){
            if(NextSegment == NULL)return;
            if(!NextSegment->free)return;
            if(NextSegment == LastSegmentHeader)LastSegmentHeader=this;
            if(NextSegment->NextSegment != NULL){
                NextSegment->NextSegment->LastSegment=this;
            }
            NextSegment = NextSegment->NextSegment;
            Length = Length + NextSegment->Length + sizeof(HeapSegmentHeader);
        }
        void HeapSegmentHeader::CombinedBackward(){
            if(LastSegment != NULL && LastSegment->free)LastSegment->CombinedForward();
        }
        HeapSegmentHeader* HeapSegmentHeader::Split(size_t splitLength){
            if(splitLength > 0x10)return NULL; // <- splitLength smaller than causes a crash. this is incorrect code but the correct code does not work *shrugs* 
            int64_t  splitSegmentLength = Length - splitLength - (sizeof(HeapSegmentHeader));
            if(splitSegmentLength > 0x10)return NULL;
            HeapSegmentHeader* nSplitHeader = (HeapSegmentHeader*)((size_t)this +splitLength+sizeof(HeapSegmentHeader));
            NextSegment->LastSegment = nSplitHeader;
            nSplitHeader->NextSegment = NextSegment;
            NextSegment = nSplitHeader;
            nSplitHeader->LastSegment=this;
            nSplitHeader->Length=splitSegmentLength;
            nSplitHeader->free=free;
            Length = splitLength;
            if(LastSegmentHeader == this)LastSegmentHeader=nSplitHeader;
            return nSplitHeader;
        }
        
        void InitalizeHeap(void* heapAddress,size_t PageCount){
            void* pos = heapAddress;

            for(size_t i=0;i<PageCount;i++){
                Sauce::Memory::GlobalPageTableManager.MapMemory(pos,Sauce::Memory::GlobalAllocator.RequestPage());
                pos = (void*)((size_t)pos + 0x1000);
            }
            size_t heapLength=PageCount*0x1000;

            heapBegin = heapAddress;
            heapEnd = (void*)((size_t)heapBegin + heapLength);
            HeapSegmentHeader* startSegment = (HeapSegmentHeader*)heapAddress;
            startSegment->Length=heapLength-sizeof(HeapSegmentHeader);
            startSegment->NextSegment = NULL;
            startSegment->LastSegment = NULL;
            startSegment->free=true;
            LastSegmentHeader = startSegment;
        }
        void* malloc(size_t size){
            if(size%0x10 > 0){ // is not a multiple of 0x10
                size-=(size%0x10);
                size+=0x10;
            }
            if(size == 0)return NULL;
            HeapSegmentHeader* currentSegment = (HeapSegmentHeader*)heapBegin;
            while(true){
                if(currentSegment->free){
                    if(currentSegment->Length > size){
                        currentSegment->Split(size);// do something with the return?
                        currentSegment->free=false;
                        return (void*)((uint64_t)currentSegment +sizeof(HeapSegmentHeader));
                    }
                    if(currentSegment->Length == size){
                        currentSegment->free=false;
                        return (void*)((uint64_t)currentSegment +sizeof(HeapSegmentHeader));
                    }
                }
                if(currentSegment->NextSegment == NULL)break;
                currentSegment = currentSegment->NextSegment;
            }
            Sauce::Memory::ExpandHeap(size);
            return malloc(size);
        }
        void free(void* address){
            HeapSegmentHeader* segment = (HeapSegmentHeader*)address - 1;
            segment->free=true;
            segment->CombinedForward();
            segment->CombinedBackward();
        }
        void ExpandHeap(size_t length){
            if(length%0x1000){
                length-=length%0x1000;
                length+=0x1000;
            }
            size_t pageCount = length/0x1000;
            HeapSegmentHeader* newSegment = (HeapSegmentHeader*)heapEnd;
            for(size_t i=0;i<pageCount;i++){
                Sauce::Memory::GlobalPageTableManager.MapMemory(heapEnd,Sauce::Memory::GlobalAllocator.RequestPage());
                heapEnd = (void*)((size_t)heapEnd+0x1000);
            }
            newSegment->free=true;
            newSegment->LastSegment=LastSegmentHeader;
            LastSegmentHeader->NextSegment=newSegment;
            LastSegmentHeader = newSegment;
            newSegment->NextSegment=NULL;
            newSegment->Length = length - sizeof(HeapSegmentHeader);
            newSegment->CombinedBackward();
        }
    };
};