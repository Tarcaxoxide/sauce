#include<Sauce/Graphics/Mouse.hpp>
#include<Sauce/IO/Debug/Console.hpp>
#include<Sauce/IO/Mouse.hpp>

uint8_t Glyphs[][13*13]={
                {
                    9,9,9,9,8,7,6,5,4,3,2,1,0,
                    9,0,0,1,2,3,4,5,6,7,8,9,0,
                    9,0,0,1,0,0,0,0,0,0,0,0,0,
                    9,1,1,0,2,0,0,0,0,0,0,0,0,
                    8,2,0,2,0,3,0,0,0,0,0,0,0,
                    7,3,0,0,3,0,4,0,0,0,0,0,0,
                    6,4,0,0,0,4,0,5,0,0,0,0,0,
                    5,5,0,0,0,0,5,0,6,0,0,0,0,
                    4,6,0,0,0,0,0,6,0,7,0,0,0,
                    3,7,0,0,0,0,0,0,7,0,8,0,0,
                    2,8,0,0,0,0,0,0,0,8,0,9,0,
                    1,9,0,0,0,0,0,0,0,0,9,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0
                }
};


namespace Sauce{
    namespace Graphics{
        void Mouse_cl::PutChar(size_t chr){
            if(Sauce::IO::Debug::FUNCTION_CALLS && Sauce::IO::Debug::MOUSE)Sauce::IO::Debug::COM1_Console.Write((char*)"[Mouse_cl::PutChar]\n\0");
            for(size_t X=0;X<13;X++){
                for(size_t Y=0;Y<13;Y++){
                    GOP_PixelStructure ThisColor{0,0,0,0xFF};
                    ThisColor.Red=ForegroundColor.Red/9;
                    ThisColor.Green=ForegroundColor.Green/9;
                    ThisColor.Blue=ForegroundColor.Blue/9;
                    ThisColor.Red*=Glyphs[chr][Sauce::ind(X-1,Y-1,13)];
                    ThisColor.Green*=Glyphs[chr][Sauce::ind(X-1,Y-1,13)];
                    ThisColor.Blue*=Glyphs[chr][Sauce::ind(X-1,Y-1,13)];
                    ThisColor.Alpha=Glyphs[chr][Sauce::ind(X-1,Y-1,13)];
                    if(!(ThisColor.Red == 0x00 && ThisColor.Blue == 0x00 && ThisColor.Green == 0x00 && ThisColor.Alpha == 0x00)){
                        PixelBuffer[Sauce::ind(X+Cursor.X,Y+Cursor.Y,PixelsPerLine)]=ThisColor;
                    }
                }
            }

        }
        Mouse_cl::Mouse_cl(Point64_t InitialPosition)
        :Terminal_cl((13*13),13,InitialPosition){
            if(Sauce::IO::Debug::FUNCTION_CALLS && Sauce::IO::Debug::MOUSE)Sauce::IO::Debug::COM1_Console.Write((char*)"[Mouse_cl::Mouse_cl]\n\0");
            Clear();
            PutChar(0);
            Sauce::IO::PS2MouseInitialize(InitialPosition);
        }
    };

};