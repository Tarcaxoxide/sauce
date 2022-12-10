#include<Sauce/Storage/FileSystem/FAT.hpp>
#include<Sauce/Utility/Conversion.hpp>

namespace Sauce{
    namespace Storage{
        namespace FileSystem{
            namespace FAT{
                uint32_t FAT32_FileSystemFileObject_st::ClusterNumberOfEntry(size_t EntryIndex){
                    Sauce::IO::Debug::Debugger_st Debugger("FAT32_FileSystemFileObject_st::ClusterNumberOfEntry",_NAMESPACE_,_ALLOW_PRINT_);
                    return (((uint32_t)(*((uint16_t*)DirectoryEntries[EntryIndex].CLUSTER_HIGH))) << 16) | (*((uint16_t*)DirectoryEntries[EntryIndex].CLUSTER_LOW));
                }
                uint32_t FAT32_FileSystemFileObject_st::SectorNumberOfEntry(size_t EntryIndex){
                    Sauce::IO::Debug::Debugger_st Debugger("FAT32_FileSystemFileObject_st::SectorNumberOfEntry",_NAMESPACE_,_ALLOW_PRINT_);
                    return Dist->DataStart + Dist->SectorsPerCluster * (ClusterNumberOfEntry(EntryIndex) - 2);
                }
                uint32_t FAT32_FileSystemFileObject_st::OffsetOfEntry(size_t EntryIndex){
                    Sauce::IO::Debug::Debugger_st Debugger("FAT32_FileSystemFileObject_st::OffsetOfEntry",_NAMESPACE_,_ALLOW_PRINT_);
                    return (ClusterNumberOfEntry(EntryIndex) * 4) % 512;
                }
                uint32_t FAT32_FileSystemFileObject_st::NextClusterOfEntry(size_t EntryIndex){
                    Sauce::IO::Debug::Debugger_st Debugger("FAT32_FileSystemFileObject_st::OffsetOfEntry",_NAMESPACE_,_ALLOW_PRINT_);
                    uint32_t FatSector=SectorNumberOfEntry(EntryIndex);
                    uint32_t FatOffset=OffsetOfEntry(EntryIndex);
                    Sauce::Memory::List_cl<uint8_t> buf;
                    Sauce::Global::AHCIDriver->Read(Dist->Port,FatSector,1,buf);
                    return *((uint32_t*)&buf.Raw()[FatOffset]) & 0x0FFFFFFF;
                }
                Sauce::string FAT32_FileSystemFileObject_st::NameOfEntry(size_t EntryIndex){
                    Sauce::IO::Debug::Debugger_st Debugger("FAT32_FileSystemFileObject_st::NameOfEntry",_NAMESPACE_,_ALLOW_PRINT_);
                    char _name[9]{0x00};
                    for(size_t i=0;i<8;i++){_name[i]=DirectoryEntries[EntryIndex].NAME[i];}
                    Debugger.Print(_name);
                    return Sauce::string(_name);
                }
                Sauce::string FAT32_FileSystemFileObject_st::ListEntries(){
                    Sauce::IO::Debug::Debugger_st Debugger("FAT32_FileSystemFileObject_st::ListEntries",_NAMESPACE_,_ALLOW_PRINT_);
                    Sauce::string Result;
                    for(size_t i=0;i<DirectoryEntries.Size();i++){
                        Result+=NameOfEntry(i).Raw();
                        Result+="\n\r";
                    }
                    return Result;
                }
                void FAT32_FileSystemFileObject_st::ReadEntries(){
                    Sauce::IO::Debug::Debugger_st Debugger("FAT32_FileSystemFileObject_st::ReadEntries",_NAMESPACE_,_ALLOW_PRINT_);
                    if(Directories.Size()){Debugger.Print("Entries Already read.");return;}//Already read
                    
                    for(size_t i=0;i<DirectoryEntries.Size();i++){
                        if(NameOfEntry(i) == new const char*[]{".       ","..      ",nullptr}){
                            Debugger.Print("Skipping infinitely recursive directories.");
                            continue;
                        }
                        if(*((uint8_t*)DirectoryEntries[i].ATTRIB) == ENTRY_TYPE_DIRECTORY){
                            Debugger.Print("Reading Entry.");
                            Directories+=new FAT32_FileSystemFileObject_st(ClusterNumberOfEntry(i),Dist,&DirectoryEntries[i]);
                        }
                        else if(*((uint8_t*)DirectoryEntries[i].ATTRIB) == ENTRY_TYPE_ARCHIVE){
                           Directories+=new FAT32_FileSystemFileObject_st(ClusterNumberOfEntry(i),Dist,&DirectoryEntries[i]);
                        }
                        else{
                            Directories+=(FAT32_FileSystemFileObject_st*)nullptr;
                        }
                    }
                }
                void FAT32_FileSystemFileObject_st::ReadDirectory(){
                    Sauce::IO::Debug::Debugger_st Debugger("FAT32_FileSystemFileObject_st::ReadDirectory",_NAMESPACE_,_ALLOW_PRINT_);
                    size_t offset=0;
                    // Read the first sector of the cluster
                    uint32_t SectorToRead=Dist->DataStart+(Dist->SectorsPerCluster*(ClusterNumber-2));
                    Sauce::Global::AHCIDriver->Read(Dist->Port,SectorToRead,Dist->SectorsPerCluster,Data);
                    //fill out the entries in this clusters entry array
                    Sauce::string EntryString("123456789ABCDEF");
                    EntryString.Clear();
                    for(size_t i=0;i<16;i++){
                        EntryString="[";
                        EntryString+=Sauce::Utility::Conversion::ToString(i);
                        EntryString+="]";
                        DirectoryEntry_st DirectoryEntry;
                        char _name[9]{0x00};
                        for(size_t z=0;z<8;z++){DirectoryEntry.NAME[z]=Data[z+offset];_name[z]=Data[z+offset];}offset+=8;
                        EntryString+=_name;
                        if(_name[0] == 0x00){EntryString+="[ABSENT]";Debugger.Print(EntryString.Raw());continue;}
                        LastEntryIndex=i;
                        EntryString+=",[";
                        uint8_t ext[4]{0x00};
                        for(size_t z=0;z<3;z++){DirectoryEntry.EXT[z]=Data[z+offset];/*increasing moves numbers left?*/ext[z+1]=Data[z+offset];}offset+=3;
                        EntryString+=Sauce::Utility::Conversion::HexToString(*((uint32_t*)ext));
                        EntryString+=",";
                        for(size_t z=0;z<1;z++){DirectoryEntry.ATTRIB[z]=Data[z+offset];}offset+=1;
                        EntryString+=Sauce::Utility::Conversion::HexToString(*((uint8_t*)DirectoryEntry.ATTRIB));
                        if(*((uint8_t*)DirectoryEntry.ATTRIB) == ENTRY_TYPE_READ_ONLY)EntryString+="(READ_ONLY)";
                        if(*((uint8_t*)DirectoryEntry.ATTRIB) == ENTRY_TYPE_HIDDEN)EntryString+="(HIDDEN)";
                        if(*((uint8_t*)DirectoryEntry.ATTRIB) == ENTRY_TYPE_SYSTEM)EntryString+="(SYSTEM)";
                        if(*((uint8_t*)DirectoryEntry.ATTRIB) == ENTRY_TYPE_VOLUME_ID)EntryString+="(VOLUME_ID)";
                        if(*((uint8_t*)DirectoryEntry.ATTRIB) == ENTRY_TYPE_DIRECTORY)EntryString+="(DIRECTORY)";
                        if(*((uint8_t*)DirectoryEntry.ATTRIB) == ENTRY_TYPE_ARCHIVE)EntryString+="(ARCHIVE)";
                        if(*((uint8_t*)DirectoryEntry.ATTRIB) == ENTRY_TYPE_LFN)EntryString+="(LFN)";
                        EntryString+=",";
                        for(size_t z=0;z<1;z++){DirectoryEntry.USER_ATTRIB[z]=Data[z+offset];}offset+=1;
                        EntryString+=Sauce::Utility::Conversion::HexToString(*((uint8_t*)DirectoryEntry.USER_ATTRIB));
                        EntryString+=",";
                        for(size_t z=0;z<1;z++){DirectoryEntry.UNDELETE[z]=Data[z+offset];}offset+=1;
                        for(size_t z=0;z<2;z++){DirectoryEntry.CREATE_TIME[z]=Data[z+offset];}offset+=2;
                        for(size_t z=0;z<2;z++){DirectoryEntry.CREATE_DATE[z]=Data[z+offset];}offset+=2;
                        for(size_t z=0;z<2;z++){DirectoryEntry.ACCESS_DATE[z]=Data[z+offset];}offset+=2;
                        for(size_t z=0;z<2;z++){DirectoryEntry.CLUSTER_HIGH[z]=Data[z+offset];}offset+=2;
                        for(size_t z=0;z<2;z++){DirectoryEntry.MODIFIED_TIME[z]=Data[z+offset];}offset+=2;
                        for(size_t z=0;z<2;z++){DirectoryEntry.MODIFIED_DATE[z]=Data[z+offset];}offset+=2;
                        for(size_t z=0;z<2;z++){DirectoryEntry.CLUSTER_LOW[z]=Data[z+offset];}offset+=2;
                        EntryString+=Sauce::Utility::Conversion::HexToString((((uint32_t)(*((uint16_t*)DirectoryEntry.CLUSTER_HIGH))) << 16) | (*((uint16_t*)DirectoryEntry.CLUSTER_LOW)));
                        EntryString+=",";
                        for(size_t z=0;z<4;z++){DirectoryEntry.FILE_SIZE[z]=Data[z+offset];}offset+=4;
                        EntryString+=Sauce::Utility::Conversion::HexToString(*((uint32_t*)DirectoryEntry.FILE_SIZE));
                        EntryString+="]";
                        DirectoryEntries+=DirectoryEntry;
                        Debugger.Print(EntryString.Raw());
                    }
                    ReadEntries();
                }
                void FAT32_FileSystemFileObject_st::ReadFile(){
                    uint32_t SectorToRead=Dist->DataStart+(Dist->SectorsPerCluster*(ClusterNumber-2));
                    Sauce::Global::AHCIDriver->Read(Dist->Port,SectorToRead,Dist->SectorsPerCluster,Data);
                    if(*((uint32_t*)ThisEntry->FILE_SIZE) > (Dist->SectorsPerCluster*Dist->BytesPerSector)){
                        FAT32_FileSystemFileObject_st* tmpDirectoryEntry=new FAT32_FileSystemFileObject_st(ClusterNumberOfEntry(ClusterNumber+1),Dist,ThisEntry);
                        for(size_t i=0;i<tmpDirectoryEntry->Data.Size();i++){
                            Data+=tmpDirectoryEntry->Data[i];
                        }
                        delete tmpDirectoryEntry;
                    }
                    
                }
                FAT32_FileSystemFileObject_st::FAT32_FileSystemFileObject_st(size_t ClusterNumber,DistilledInformation_st* Dist,DirectoryEntry_st* ThisEntry){
                    Sauce::IO::Debug::Debugger_st Debugger("FAT32_FileSystemFileObject_st::FAT32_FileSystemFileObject_st",_NAMESPACE_,_ALLOW_PRINT_);
                    this->Dist=Dist;
                    this->ThisEntry=ThisEntry;
                    this->ClusterNumber=ClusterNumber;
                    if(ThisEntry == nullptr){
                        ReadDirectory();
                    }else{
                        Sauce::string Name((char*)ThisEntry->NAME);
                        if(*((uint8_t*)ThisEntry->ATTRIB) == ENTRY_TYPE_DIRECTORY){
                            Debugger.Print("Directory Initializing");
                            Debugger.Print(Name.Raw());
                            ReadDirectory();
                        }
                        else if(*((uint8_t*)ThisEntry->ATTRIB) == ENTRY_TYPE_ARCHIVE){
                           Debugger.Print("File Initializing");
                           Debugger.Print(Name.Raw());

                        }
                    }
                }
                FAT32_FileSystemFileObject_st::~FAT32_FileSystemFileObject_st(){
                    for(size_t i=0;i<Directories.Size();i++){
                        if(Directories[i] != (FAT32_FileSystemFileObject_st*)nullptr){
                            delete Directories[i];
                        }
                    }
                    Directories.Clear();
                    DirectoryEntries.Clear();
                }
                FAT32Driver_st::FAT32Driver_st(size_t Port,uint32_t PartitionOffset){
                    Sauce::IO::Debug::Debugger_st Debugger("FAT32Driver_st::FAT32Driver_st",_NAMESPACE_,_ALLOW_PRINT_);
                    Dist.Port=Port;
                    Dist.PartitionOffset=PartitionOffset;
                    size_t CurrentByte=0;
                    Sauce::string debugString;
                    debugString="Boot_Record.NOP: ";
                    Sauce::Memory::List_cl<uint8_t> tmpArray;
                    for(size_t i=0;i<3;i++){
                        Boot_Record.NOP[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        tmpArray << Boot_Record.NOP[i];
                    }
                    tmpArray.Flip();
                    
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.OEM_IDENTIFIER: ";
                    for(size_t i=0;i<8;i++){
                        Boot_Record.OEM_IDENTIFIER[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.OEM_IDENTIFIER[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.NUMBER_OF_BYTES_PER_SECTOR: ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.NUMBER_OF_BYTES_PER_SECTOR[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.NUMBER_OF_BYTES_PER_SECTOR[i]);
                    }
                    Dist.BytesPerSector=*((uint16_t*)Boot_Record.NUMBER_OF_BYTES_PER_SECTOR);
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.NUMBER_OF_SECTORS_PER_CLUSTER: ";
                    for(size_t i=0;i<1;i++){
                        Boot_Record.NUMBER_OF_SECTORS_PER_CLUSTER[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.NUMBER_OF_SECTORS_PER_CLUSTER[i]);
                    }
                    Dist.SectorsPerCluster=*((uint8_t*)Boot_Record.NUMBER_OF_SECTORS_PER_CLUSTER);
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.NUMBER_OF_RESERVED_SECTORS: ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.NUMBER_OF_RESERVED_SECTORS[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                    }
                    debugString+=Sauce::Utility::Conversion::HexToString(*((uint16_t*)Boot_Record.NUMBER_OF_RESERVED_SECTORS));
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.NUMBER_OF_FATS: ";
                    for(size_t i=0;i<1;i++){
                        Boot_Record.NUMBER_OF_FATS[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.NUMBER_OF_FATS[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.NUMBER_OF_FATS: ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.NUMBER_OF_ROOT_DIRECTORY_ENTRIES[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.NUMBER_OF_ROOT_DIRECTORY_ENTRIES[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.TOTAL_SECTORS_IN_LOGICAL_VOLUME: ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.TOTAL_SECTORS_IN_LOGICAL_VOLUME[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.TOTAL_SECTORS_IN_LOGICAL_VOLUME[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.MEDIA_DESCRIPTOR_TYPE: ";
                    for(size_t i=0;i<1;i++){
                        Boot_Record.MEDIA_DESCRIPTOR_TYPE[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.MEDIA_DESCRIPTOR_TYPE[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.NUMBER_OF_SECTORS_PER_FAT_(12/16): ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.NUMBER_OF_SECTORS_PER_FAT_12[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.NUMBER_OF_SECTORS_PER_FAT_12[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.NUMBER_OF_SECTORS_PER_TRACK: ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.NUMBER_OF_SECTORS_PER_TRACK[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.NUMBER_OF_SECTORS_PER_TRACK[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.NUMBER_OF_(HEAD/SIDES): ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.NUMBER_OF_HEADS[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.NUMBER_OF_HEADS[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.NUMBER_OF_HIDDEN_SECTORS: ";
                    for(size_t i=0;i<4;i++){
                        Boot_Record.NUMBER_OF_HIDDEN_SECTORS[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.NUMBER_OF_HIDDEN_SECTORS[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.LARGE_SECTOR_COUNT: ";
                    for(size_t i=0;i<4;i++){
                        Boot_Record.LARGE_SECTOR_COUNT[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.LARGE_SECTOR_COUNT[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.NUMBER_OF_SECTORS_PER_FAT_32: ";
                    for(size_t i=0;i<4;i++){
                        Boot_Record.NUMBER_OF_SECTORS_PER_FAT_32[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.NUMBER_OF_SECTORS_PER_FAT_32[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.FLAGS: ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.FLAGS[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.FLAGS[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.FAT_VERSION: ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.FAT_VERSION[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.FAT_VERSION[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.CLUSTER_NUMBER_OF_ROOT_DIRECTORY: ";
                    for(size_t i=0;i<4;i++){
                        Boot_Record.CLUSTER_NUMBER_OF_ROOT_DIRECTORY[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.CLUSTER_NUMBER_OF_ROOT_DIRECTORY[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.SECTOR_NUMBER_OF_THE_FSINFO_STRUCTURE: ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.SECTOR_NUMBER_OF_THE_FSINFO_STRUCTURE[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.SECTOR_NUMBER_OF_THE_FSINFO_STRUCTURE[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.SECTOR_NUMBER_OF_BACKUP_BOOT_SECTOR: ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.SECTOR_NUMBER_OF_BACKUP_BOOT_SECTOR[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.SECTOR_NUMBER_OF_BACKUP_BOOT_SECTOR[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.RESERVED0: ";
                    for(size_t i=0;i<12;i++){
                        Boot_Record.RESERVED0[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.RESERVED0[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.DRIVE_NUMBER: ";
                    for(size_t i=0;i<1;i++){
                        Boot_Record.DRIVE_NUMBER[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.DRIVE_NUMBER[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.WINDOWS_FLAGS: ";
                    for(size_t i=0;i<1;i++){
                        Boot_Record.WINDOWS_FLAGS[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.WINDOWS_FLAGS[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.SIGNATURE: ";
                    for(size_t i=0;i<1;i++){
                        Boot_Record.SIGNATURE[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.SIGNATURE[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.VOLUME_ID: ";
                    for(size_t i=0;i<4;i++){
                        Boot_Record.VOLUME_ID[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.VOLUME_ID[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.VOLUME_LABEL: ";
                    for(size_t i=0;i<11;i++){
                        Boot_Record.VOLUME_LABEL[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.VOLUME_LABEL[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.SYSTEM_IDENTIFIER: ";
                    for(size_t i=0;i<8;i++){
                        Boot_Record.SYSTEM_IDENTIFIER[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.SYSTEM_IDENTIFIER[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.BOOT_CODE: ";
                    for(size_t i=0;i<420;i++){
                        Boot_Record.BOOT_CODE[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.BOOT_CODE[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="Boot_Record.BOOT_MAGIC: ";
                    for(size_t i=0;i<2;i++){
                        Boot_Record.BOOT_MAGIC[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(Boot_Record.BOOT_MAGIC[i]);
                    }
                    Debugger.Print(debugString.Raw());

                    CurrentByte=((uint16_t)*Boot_Record.SECTOR_NUMBER_OF_THE_FSINFO_STRUCTURE)*(*((uint16_t*)Boot_Record.NUMBER_OF_BYTES_PER_SECTOR));
                    debugString="FSINFO_Structure.LEAD_SIGNATURE: ";
                    for(size_t i=0;i<4;i++){
                        FSINFO_Structure.LEAD_SIGNATURE[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(FSINFO_Structure.LEAD_SIGNATURE[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="FSINFO_Structure.RESERVED0: ";
                    for(size_t i=0;i<480;i++){
                        FSINFO_Structure.RESERVED0[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(FSINFO_Structure.RESERVED0[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="FSINFO_Structure.ANOTHER_SIGNATURE: ";
                    for(size_t i=0;i<4;i++){
                        FSINFO_Structure.ANOTHER_SIGNATURE[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(FSINFO_Structure.ANOTHER_SIGNATURE[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="FSINFO_Structure.LAST_KNOWN_FREE_CLUSTER: ";
                    for(size_t i=0;i<4;i++){
                        FSINFO_Structure.LAST_KNOWN_FREE_CLUSTER[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(FSINFO_Structure.LAST_KNOWN_FREE_CLUSTER[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="FSINFO_Structure.START_SEARCHING_FOR_AVAILABLE_CLUSTERS_HERE: ";
                    for(size_t i=0;i<4;i++){
                        FSINFO_Structure.START_SEARCHING_FOR_AVAILABLE_CLUSTERS_HERE[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(FSINFO_Structure.START_SEARCHING_FOR_AVAILABLE_CLUSTERS_HERE[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="FSINFO_Structure.RESERVED1: ";
                    for(size_t i=0;i<12;i++){
                        FSINFO_Structure.RESERVED1[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(FSINFO_Structure.RESERVED1[i]);
                    }
                    Debugger.Print(debugString.Raw());
                    debugString="FSINFO_Structure.TRAIL_SIGNATURE: ";
                    for(size_t i=0;i<4;i++){
                        FSINFO_Structure.TRAIL_SIGNATURE[i]=Sauce::Global::AHCIDriver->Read(Port,CurrentByte++);
                        debugString+=Sauce::Utility::Conversion::HexToString(FSINFO_Structure.TRAIL_SIGNATURE[i]);
                    }
                    Debugger.Print(debugString.Raw());

                    //void AHCIDriver_cl::Read(size_t portNumber,size_t startingSector,size_t sectorCount,Sauce::Memory::List_cl<uint8_t> &Bufferr)

                    Dist.FatStart=Dist.PartitionOffset+(*((uint16_t*)Boot_Record.NUMBER_OF_RESERVED_SECTORS));
                    Dist.FatSize=(*((uint32_t*)Boot_Record.NUMBER_OF_SECTORS_PER_FAT_32));
                    Dist.DataStart=Dist.FatStart+Dist.FatSize*(*((uint8_t*)Boot_Record.NUMBER_OF_FATS));
                    Dist.SectorOfRootDirectoryEntry=Dist.DataStart+((*((uint8_t*)Boot_Record.NUMBER_OF_SECTORS_PER_CLUSTER))*((*((uint32_t*)Boot_Record.CLUSTER_NUMBER_OF_ROOT_DIRECTORY))-2));
                    
                    RootDirectory = new FAT32_FileSystemFileObject_st(*((uint32_t*)Boot_Record.CLUSTER_NUMBER_OF_ROOT_DIRECTORY),&Dist);
                }
            };
        };
    };
};