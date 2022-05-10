#pragma once


#include<stddef.h>
#include<stdint.h>
#include<Sauce/IO/ACPI/ACPI.hpp>


namespace Sauce{
    namespace IO{
        struct PCIDeviceHeader{
            uint16_t VendorID;
            uint16_t DeviceID;
            uint16_t Command;
            uint16_t Status;
            uint8_t RevisionID;
            uint8_t ProgIF;//IF InterFace
            uint8_t Subclass;
            uint8_t Class;
            uint8_t CacheLineSize;
            uint8_t LatencyTimer;
            uint8_t HeaderType;
            uint8_t BIST;
        }__attribute__((packed));
        void EnemerateFunction(uint64_t deviceAddress,uint64_t function);
        void EnumerateDevice(uint64_t busAddress,uint64_t device);
        void EnumerateBus(uint64_t baseAddress,uint64_t bus);
        void EnumeratePCI(Sauce::IO::ACPI::MCFGHeader* mcfg);
        extern const char* DeviceClasses[];
       const char* GetVenderName(uint16_t VendorID);
       const char* GetDeviceName(uint16_t VendorID,uint16_t DeviceID);
       const char* GetSubClassName(uint8_t ClassCode,uint8_t SubClassCode);
       const char* GetProgIFName(uint8_t ClassCode,uint8_t SubClassCode,uint8_t ProgIFCode);
    };
};