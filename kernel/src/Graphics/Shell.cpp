#include<Sauce/Graphics/Shell.hpp>
#include<Sauce/IO/Debug/Console.hpp>
#include<Sauce/Math/Types.hpp>
#include<Sauce/Math/Functions.hpp>
#include<Sauce/Utility/Conversion.hpp>
#include<Sauce/Global.hpp>
#include<Sauce/Utility/NeuralNetwork.hpp>

/*experimentation*/#include <Sauce/Storage/FileSystem/FAT.hpp>


namespace Sauce{
    namespace Graphics{
        Shell_cl::Shell_cl(Sauce::Point64_st Size,Sauce::Point64_st Offset)
        :Terminal_cl((Size.X*Size.Y),Size.X,Offset){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::Shell_cl",_NAMESPACE_,_ALLOW_PRINT_);
            ShellClear(true);
        }
        void Shell_cl::PutChar(char chr,bool AddToBuffer){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::PutChar",_NAMESPACE_,_ALLOW_PRINT_);
            size_t chrindex = (size_t)chr;

            switch(chrindex){
                case '\n':{
                    if(AddToBuffer){
                        if(!CharBuffer.Size())break;
                        CharBuffer.AddLast(chr);
                        ParseAndRunCommand();
                    }
                    GoDown();
                }break;
                case '\b':{
                    if(AddToBuffer){
                        if(!CharBuffer.RemoveLast())break;
                    }
                    if(!GoLeft()){
                        if(GoUp()){
                            GoFarRight();
                        }
                    }else{
                        PutChar(' ',false);
                        GoLeft();
                    }
                }break;
                case '\r':{
                    GoFarLeft();
                    if(AddToBuffer)CharBuffer.Clear();
                }break;
                default:{
                    if(AddToBuffer)CharBuffer.AddLast(chr);
                    for(size_t X=2;X<Sauce::Graphics::SauceFont::GlyphWidth;X++){
                        for(size_t Y=2;Y<Sauce::Graphics::SauceFont::GlyphHeight;Y++){
                            PixelBuffer[Sauce::Math::index(X+Cursor.X,Y+Cursor.Y,PixelsPerLine)]=Blend(ForegroundColor,BackgroundColor,Sauce::Graphics::SauceFont::Glyphs[chrindex][Sauce::Math::index(X-1,Y-1,Sauce::Graphics::SauceFont::GlyphWidth)]);
                        }
                    }
                    if(!GoRight()){
                        if(GoDown()){
                            GoFarLeft();
                        }
                    }
                }break;
            }
        }
        void Shell_cl::PutString(Sauce::string str,bool AddToBuffer){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::PutString",_NAMESPACE_,_ALLOW_PRINT_);
            for(size_t i=0;i<str.Size();i++){
                PutChar(str[i],AddToBuffer);
            }
        }
        bool Shell_cl::GoDown(size_t amount){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::GoDown",_NAMESPACE_,_ALLOW_PRINT_);
            if((Cursor.Y+(Sauce::Graphics::SauceFont::GlyphHeight-2)*amount) > (PixelsBufferHeight-(Sauce::Graphics::SauceFont::GlyphHeight-2)*amount) ){
                return false;
            }
            Cursor.Y+=(Sauce::Graphics::SauceFont::GlyphHeight-2)*amount;
            return true;
        }
        bool Shell_cl::GoUp(size_t amount){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::GoUp",_NAMESPACE_,_ALLOW_PRINT_);
            if((Cursor.Y-(Sauce::Graphics::SauceFont::GlyphHeight-2)*amount) < 0){
                return false;
            }
            Cursor.Y-=(Sauce::Graphics::SauceFont::GlyphHeight-2)*amount;
            return true;
        }
        bool Shell_cl::GoRight(size_t amount){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::GoRight",_NAMESPACE_,_ALLOW_PRINT_);
            if((Cursor.X+(Sauce::Graphics::SauceFont::GlyphWidth-2)*amount) > (PixelsPerLine-(Sauce::Graphics::SauceFont::GlyphWidth-2)*amount) ){
                return false;
            }
            Cursor.X+=(Sauce::Graphics::SauceFont::GlyphWidth-2)*amount;
            return true;
        }
        bool Shell_cl::GoLeft(size_t amount){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::GoLeft",_NAMESPACE_,_ALLOW_PRINT_);
            if((Cursor.X-(Sauce::Graphics::SauceFont::GlyphWidth-2)*amount) < 0){
                return false;
            }
            Cursor.X-=(Sauce::Graphics::SauceFont::GlyphWidth-2)*amount;
            return true;
        }
        void Shell_cl::GoFarDown(){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::GoFarDown",_NAMESPACE_,_ALLOW_PRINT_);
            Cursor.Y=PixelsBufferHeight-(Sauce::Graphics::SauceFont::GlyphHeight-2);
        }
        void Shell_cl::GoFarUp(){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::GoFarUp",_NAMESPACE_,_ALLOW_PRINT_);
            Cursor.Y=0;
            
        }
        void Shell_cl::GoFarRight(){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::GoFarRight",_NAMESPACE_,_ALLOW_PRINT_);
            Cursor.X=PixelsPerLine-(Sauce::Graphics::SauceFont::GlyphWidth-2);
            
        }
        void Shell_cl::GoFarLeft(){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::GoFarLeft",_NAMESPACE_,_ALLOW_PRINT_);
            Cursor.X=0;
            
        }
        void Shell_cl::RunCommand(Sauce::Memory::List_cl<Sauce::string*>& Arg){
            if(*(Arg.First()) == Sauce::string("color")){Arg.RemoveFirst();
                ReverseColor();
                return;
            }
            if(*(Arg.First()) == Sauce::string("ahci")){Arg.RemoveFirst();
                if(*(Arg.First()) == Sauce::string("list_ports")){
                    PutString(Sauce::Global::AHCIDriver->ListPorts(),false);
                    PutString("\n\r",false);
                    return;
                }
                return;
            }
            if(*(Arg.First()) == Sauce::string("fat32")){Arg.RemoveFirst();
                uint64_t Pn=Sauce::Utility::Conversion::ToUint64(Arg[0]->Raw());
                Sauce::Storage::FileSystem::FAT::FAT32Driver_st testFatDriver(Pn);
                testFatDriver.RootDirectory->ReadEntries();
                Sauce::string R=testFatDriver.RootDirectory->ListEntries();
                PutString(R,false);
                return;
            }
            if(*(Arg.First()) == Sauce::string("math")){Arg.RemoveFirst();
                Sauce::string Result=Sauce::Math::simple_equation(*(Arg[0]),*(Arg[1]),*(Arg[2]));
                PutString(Result,false);
                PutString("\n\r",false);
                return;
            }
            if(*(Arg.First()) == Sauce::string("ElfTentProblem")){Arg.RemoveFirst();
                const uint8_t X=1,A=1,Rock=1;
                const uint8_t Y=2,B=2,Paper=2;
                const uint8_t Z=2,C=2,Scissors=3;
                struct It_st{uint8_t In;uint8_t Out;};
                Sauce::Memory::List_cl<It_st> Strategy;
                Strategy << It_st{A,Y};
                Strategy << It_st{B,X};
                Strategy << It_st{C,Z};
                size_t Score=0;
                It_st It;
                while(Strategy >> It){
                    uint8_t Added=3+((It.In-It.Out)*3);
                    Score+=It.Out+Added;
                }
                PutString(Sauce::Utility::Conversion::ToString(Score),false);
                PutString("\n\r",false);
                return;
            }
            if(*(Arg.First()) == Sauce::string("DynamicArray")){Arg.RemoveFirst();
                Sauce::Memory::List_cl<uint8_t> test;
                test << 'a';
                test << '0';
                test << '1';
                test << '2';
                uint8_t az;
                while(test >> az){
                    PutString(Sauce::Utility::Conversion::HexToString(az),false);
                    PutString("\n\r",false);
                }
            }
        }
        void Shell_cl::ParseAndRunCommand(){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::ParseAndRunCommand",_NAMESPACE_,_ALLOW_PRINT_);
            Sauce::Memory::List_cl<Sauce::string*> ArgBuffer;
            size_t CrawlerVal=0;
            for(size_t i=0;i<CharBuffer.Size();i++){
                if(CharBuffer[i] == ' ' || CharBuffer[i] == '\n'){
                    Sauce::string* str = new Sauce::string;
                    for(size_t a=CrawlerVal;a<i;a++){
                        if(!(CharBuffer[a] == ' ' || CharBuffer[a] == '\n')){
                            str->AddLast((char)CharBuffer[a]);
                        }
                    }
                    ArgBuffer.AddLast(str);
                    delete[] str;
                    CrawlerVal=i;
                }
            }
            ShellClear(false);
            PutString("\n\r",false);
            RunCommand(ArgBuffer);            
        }
        void Shell_cl::ShellClear(bool ClearScreen){
            Sauce::IO::Debug::Debugger_st Debugger("Shell_cl::ShellClear",_NAMESPACE_,_ALLOW_PRINT_);
            CharBuffer.Clear();
            if(ClearScreen){
                Cursor.X=0;
                Cursor.Y=0;
                Cursor.Z=0;
                Clear();
            }
        }
    };
};
