#pragma once

#include<Sauce/Types.hpp>
#include<Sauce/Memory/Memory.hpp>
#include<Sauce/Memory/Paging.hpp>
#include<Sauce/Memory/PageMapIndexer.hpp>
#include<Sauce/Memory/PageFrameAllocator.hpp>

namespace Sauce{
    namespace Memory{
        class PageTableManager_cl{
            public:
            PageTableManager_cl(PageTable* PML4Address);
            PageTable* PML4Address=nullptr;
            void MapMemory(void* virtualMemory,void* physicalMemory);
        };
        
    };
};