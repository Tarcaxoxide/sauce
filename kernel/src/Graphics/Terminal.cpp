#include<Sauce/Graphics/Terminal.hpp>
#include<Sauce/Utilities/Conversion.hpp>
#include<Sauce/Memory/Memory.hpp>
#include<Sauce/IO/Debug/Console.hpp>
namespace Sauce{
    namespace Graphics{
        GOP_PixelStructure Terminal_cl::ForegroundColor{0xFF,0xFF,0xFF,0xFF};
        GOP_PixelStructure Terminal_cl::BackgroundColor{0x00,0x00,0x00,0xFF};
        
        Terminal_cl::Terminal_cl(size_t PixelBufferTotalSize,size_t PixelsPerLine,Point64_t Offset){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::Terminal_cl",Sauce::IO::Debug::TERMINAL,true);
            this->PixelBuffer=new GOP_PixelStructure[PixelBufferTotalSize];
            this->PixelBufferTotalSize=PixelBufferTotalSize;
            this->PixelsPerLine=PixelsPerLine;
            PixelsBufferHeight=(PixelBufferTotalSize/PixelsPerLine);
            MyOffset=Offset;
            if(Sauce::IO::Debug::FUNCTION_RETURNS && Sauce::IO::Debug::TERMINAL,true)Sauce::IO::Debug::COM1_Console.Write((char*)"\t<-(\"this\")\n\0");
            Sauce::IO::Debug::Print_Spammy_Return("this",Sauce::IO::Debug::TERMINAL,true);
        }
        bool Terminal_cl::SetColor(GOP_PixelStructure ForegroundColor,GOP_PixelStructure BackgroundColor){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::SetColor",Sauce::IO::Debug::TERMINAL,true);
            this->ForegroundColor=ForegroundColor;
            this->BackgroundColor=BackgroundColor;
            Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true);
            return true;
        }
        bool Terminal_cl::SetColor(GOP_PixelStructure ForegroundColor){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::SetColor",Sauce::IO::Debug::TERMINAL,true);
            this->ForegroundColor=ForegroundColor;
            Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true);
            return true;
        }
        bool Terminal_cl::RowFill(size_t RowIndex,GOP_PixelStructure TheColor){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::RowFill",Sauce::IO::Debug::TERMINAL,true);
            if(RowIndex > PixelsPerLine){
                Sauce::IO::Debug::Print_Spammy_Return("False",Sauce::IO::Debug::TERMINAL,true);
                return false;
            }
            PixelPointer.X=RowIndex;
            for(PixelPointer.Y=0;PixelPointer.Y<PixelsBufferHeight;PixelPointer.Y++){
                PixelBuffer[Sauce::ind(PixelPointer.X,PixelPointer.Y,PixelsPerLine)]=TheColor;
            }
            Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true);
            return true;
        }
        bool Terminal_cl::ColumnFill(size_t ColumnIndex,GOP_PixelStructure TheColor){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::ColumnFill",Sauce::IO::Debug::TERMINAL,true);
            if(ColumnIndex > PixelsPerLine){
                Sauce::IO::Debug::Print_Spammy_Return("False",Sauce::IO::Debug::TERMINAL,true);
                return false;
            }
            PixelPointer.Y=ColumnIndex;
            for(PixelPointer.X=0;PixelPointer.X<PixelsPerLine;PixelPointer.X++){
                PixelBuffer[Sauce::ind(PixelPointer.X,PixelPointer.Y,PixelsPerLine)]=TheColor;
            }
            Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true);
            return true;
        }
        bool Terminal_cl::Fill(GOP_PixelStructure TheColor){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::Fill",Sauce::IO::Debug::TERMINAL,true);
            for(PixelPointer.Y=0;PixelPointer.Y<PixelsBufferHeight;PixelPointer.Y++){
                for(PixelPointer.X=0;PixelPointer.X<PixelsPerLine;PixelPointer.X++){
                    PixelBuffer[Sauce::ind(PixelPointer.X,PixelPointer.Y,PixelsPerLine)]=TheColor;
                }
            }
            Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true);
            return true;
        }
        bool Terminal_cl::RowClear(size_t RowIndex){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::RowClear",Sauce::IO::Debug::TERMINAL,true);
            bool Ret=RowFill(RowIndex,BackgroundColor);
            Ret? Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true) : Sauce::IO::Debug::Print_Spammy_Return("False",Sauce::IO::Debug::TERMINAL,true);
            return Ret;
        }
        bool Terminal_cl::ColumnClear(size_t ColumnIndex){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::ColumnClear",Sauce::IO::Debug::TERMINAL,true);
            bool Ret = ColumnFill(ColumnIndex,BackgroundColor);
            Ret? Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true) : Sauce::IO::Debug::Print_Spammy_Return("False",Sauce::IO::Debug::TERMINAL,true);
            return Ret;
        }
        bool Terminal_cl::Clear(){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::Clear",Sauce::IO::Debug::TERMINAL,true);
            bool Ret = Fill(BackgroundColor);
            Ret? Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true) : Sauce::IO::Debug::Print_Spammy_Return("False",Sauce::IO::Debug::TERMINAL,true);
            return Ret;
        }
        bool Terminal_cl::SetCursor(int64_t X,int64_t Y,int64_t Z){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::SetCursor",Sauce::IO::Debug::TERMINAL,true);
            PixelPointer={X,Y,Z};
            Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true);
            return true;
        }
        bool Terminal_cl::CopyTo(GOP_PixelStructure* OtherPixelBuffer,size_t OtherPixelBufferTotalSize,size_t OtherPixelsPerLine,Point64_t Offset){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::CopyTo",Sauce::IO::Debug::TERMINAL,true);
            Offset.X+=MyOffset.X;
            Offset.Y+=MyOffset.Y;
            Offset.Z+=MyOffset.Z;
            if(OtherPixelBufferTotalSize < PixelBufferTotalSize+(Offset.X*Offset.Y)){
                Sauce::IO::Debug::Print_Spammy_Return("False",Sauce::IO::Debug::TERMINAL,true);
                return false;
            }
            for(PixelPointer.Y=0;PixelPointer.Y<PixelsBufferHeight;PixelPointer.Y++){
                for(PixelPointer.X=0;PixelPointer.X<PixelsPerLine;PixelPointer.X++){
                    OtherPixelBuffer[Sauce::ind(PixelPointer.X+Offset.X,PixelPointer.Y+Offset.Y,OtherPixelsPerLine)]=PixelBuffer[Sauce::ind(PixelPointer.X,PixelPointer.Y,PixelsPerLine)];
                }
            }
            Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true);
            return true;
        }
        bool Terminal_cl::CopyFrom(Terminal_cl* OtherTerminal){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::CopyFrom",Sauce::IO::Debug::TERMINAL,true);
            bool Ret = OtherTerminal->CopyTo(PixelBuffer,PixelBufferTotalSize,PixelsPerLine,MyOffset);
            Ret? Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true) : Sauce::IO::Debug::Print_Spammy_Return("False",Sauce::IO::Debug::TERMINAL,true);
            return Ret;
        }
        uPoint64_t Terminal_cl::Size(){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::Size",Sauce::IO::Debug::TERMINAL,true);
            Sauce::IO::Debug::Print_Spammy_Return("Width:",Sauce::IO::Debug::TERMINAL,false);
            Sauce::IO::Debug::Print_Spammy_Return(Sauce::Convert::ToString(PixelsPerLine),Sauce::IO::Debug::TERMINAL,false);
            Sauce::IO::Debug::Print_Spammy_Return(",Height:",Sauce::IO::Debug::TERMINAL,false);
            Sauce::IO::Debug::Print_Spammy_Return(Sauce::Convert::ToString(PixelsBufferHeight),Sauce::IO::Debug::TERMINAL,true);
            return {PixelsPerLine,PixelsBufferHeight,0};
        }
        bool Terminal_cl::Move(Point64_t Offset){
            Sauce::IO::Debug::Print_Spammy_Call("Terminal_cl::Move",Sauce::IO::Debug::TERMINAL,true);
            MyOffset=Offset;
            Sauce::IO::Debug::Print_Spammy_Return("True",Sauce::IO::Debug::TERMINAL,true);
            return true;
        }
    };
};