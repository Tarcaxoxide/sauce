#include <Graphics.hpp>

namespace Sauce{
    namespace Graphics{
        uint8_t* GraphicsAddress = (uint8_t*)0xA0000;
        Gchar GraphicCharacter={
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
        const size_t MAX_X=40;
        const size_t MAX_Y=25;
        static size_t Xoffset=0;
        static size_t Yoffset=0;


        void Store_GChar(Gchar Val){
            for(size_t A=0;A<8;A++){
                for(size_t B=0;B<8;B++){
                    GraphicCharacter[A][B]=Val[A][B];
                }
            }
        }

        void Place_String(char* Str,uint16_t color){
            for(size_t I=0;I<Sauce::Utils::StringLength(Str);I++){
                Place_Char(Str[I],color);
            }
        }

        void Place_Char(char Char,uint16_t color){
            switch(Char){
                default:{
                    Gchar GG={
                                {0, 0, 0, 0, 0, 0, 0, 0},
                                {0,-2,-2,-2,-2,-2,-2, 0},
                                {0,-2, 0, 0, 0, 0,-2, 0},
                                {0,-2, 0,-3,-3, 0,-2, 0},
                                {0,-2, 0,-3,-3, 0,-2, 0},
                                {0,-2, 0, 0, 0, 0,-2, 0},
                                {0,-2,-2,-2,-2,-2,-2, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0}
                                };
                    Store_GChar(GG);
                }break;
                case 'A': case 'a':{
                    Gchar GG={
                                {0,1,1,1,1,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,1,1,1,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'B': case 'b':{
                    Gchar GG={
                                {0,1,1,1,1,0,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,1,1,1,0,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,1,1,1,0,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'C':case 'c':{
                    Gchar GG={
                                {0,1,1,1,1,1,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,1,1,1,1,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'D':case 'd':{
                    Gchar GG={
                                {0,1,1,1,1,0,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,1,1,1,0,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'E':case 'e':{
                    Gchar GG={
                                {0,1,1,1,1,1,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,1,1,1,1,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,1,1,1,1,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'F':case 'f':{
                    Gchar GG={
                                {0,1,1,1,1,1,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,1,1,1,1,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'G':case 'g':{
                    Gchar GG={
                                {0,1,1,1,1,1,1,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,1,1,1,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,1,1,1,1,1,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'H':case 'h':{
                    Gchar GG={
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,1,1,1,1,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'I':case 'i':{
                    Gchar GG={
                                {0,1,1,1,1,1,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,1,1,1,1,1,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'J':case 'j':{
                    Gchar GG={
                                {0,1,1,1,1,1,1,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,1,0,0,1,0,0,0},
                                {0,1,1,1,1,0,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'K':case 'k':{
                    Gchar GG={
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,1,0,0,0},
                                {0,1,0,1,0,0,0,0},
                                {0,1,1,0,0,0,0,0},
                                {0,1,0,1,0,0,0,0},
                                {0,1,0,0,1,0,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'L':case 'l':{
                    Gchar GG={
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,1,1,1,1,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'M':case 'm':{
                    Gchar GG={
                                {1,1,1,1,1,1,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'N':case 'n':{
                    Gchar GG={
                                {1,0,0,0,0,0,1,0},
                                {1,1,0,0,0,0,1,0},
                                {1,0,1,0,0,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,0,0,1,0,1,0},
                                {1,0,0,0,0,1,1,0},
                                {1,0,0,0,0,0,1,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'O':case 'o':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {0,1,1,1,1,1,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,1,1,1,1,1,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'P':case 'p':{
                    Gchar GG={
                                {0,1,1,1,1,0,0,0},
                                {0,1,0,0,1,0,0,0},
                                {0,1,0,0,1,0,0,0},
                                {0,1,1,1,1,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'Q':case 'q':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {1,1,1,1,1,1,0,0},
                                {1,0,0,0,0,1,0,0},
                                {1,0,0,0,0,1,0,0},
                                {1,0,0,0,0,1,0,0},
                                {1,0,0,0,1,1,0,0},
                                {1,1,1,1,1,1,0,0},
                                {0,0,0,0,0,0,1,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'R':case 'r':{
                    Gchar GG={
                                {0,1,1,1,1,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,1,1,1,1,0,0},
                                {0,1,0,0,1,0,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,0,1,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'S':case 's':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {0,1,1,1,1,1,1,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,1,1,1,1,1,0},
                                {0,0,0,0,0,0,1,0},
                                {0,0,0,0,0,0,1,0},
                                {0,1,1,1,1,1,1,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'T':case 't':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {1,1,1,1,1,1,1,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                 case 'U':case 'u':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,1,1,1,1,1,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'V':case 'v':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {1,0,0,0,0,0,1,0},
                                {1,0,0,0,0,0,1,0},
                                {1,0,0,0,0,0,1,0},
                                {1,0,0,0,0,0,1,0},
                                {0,1,0,0,0,1,0,0},
                                {0,0,1,0,1,0,0,0},
                                {0,0,0,1,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'W':case 'w':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,1,0,1,0,1,0},
                                {0,1,0,0,0,1,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'X':case 'x':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {1,0,0,0,0,0,1,0},
                                {0,1,0,0,0,1,0,0},
                                {0,0,1,0,1,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,1,0,1,0,0,0},
                                {0,1,0,0,0,1,0,0},
                                {1,0,0,0,0,0,1,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'Y':case 'y':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {1,0,0,0,0,0,1,0},
                                {0,1,0,0,0,1,0,0},
                                {0,0,1,0,1,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case 'Z':case 'z':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {1,1,1,1,1,1,1,0},
                                {0,0,0,0,0,1,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,1,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {1,1,1,1,1,1,1,0}
                                };
                    Store_GChar(GG);
                }break;
                case ' ':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case '!':{
                    Gchar GG={
                                {0,0,1,0,0,0,0,0},
                                {0,0,1,0,0,0,0,0},
                                {0,0,1,0,0,0,0,0},
                                {0,0,1,0,0,0,0,0},
                                {0,0,1,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,1,0,0,0,0,0},
                                {0,0,1,0,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;

                case '1':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,1,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,0,1,0,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case '2':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {0,0,1,1,1,0,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,0,0,0,0,1,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,1,0,0,0,0},
                                {0,0,1,0,0,0,0,0},
                                {0,1,1,1,1,1,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case '3':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {0,1,1,1,1,1,0,0},
                                {0,0,0,0,0,1,0,0},
                                {0,0,0,0,0,1,0,0},
                                {0,1,1,1,1,1,0,0},
                                {0,0,0,0,0,1,0,0},
                                {0,0,0,0,0,1,0,0},
                                {0,1,1,1,1,1,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case '4':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {0,1,0,0,1,0,0,0},
                                {0,1,0,0,1,0,0,0},
                                {0,1,0,0,1,0,0,0},
                                {0,1,1,1,1,0,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case '5':{
                    Gchar GG={
                                {0,1,1,1,1,1,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,1,1,1,0,0,0},
                                {0,0,0,0,0,1,0,0},
                                {0,0,0,0,0,1,0,0},
                                {0,0,0,0,0,1,0,0},
                                {0,1,1,1,1,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case '6':{
                    Gchar GG={
                                {0,0,1,1,1,0,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,0,0,0,0,0,0},
                                {0,1,1,1,1,0,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,0,0,0,1,0,0},
                                {0,1,1,1,1,1,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case '7':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {1,1,1,1,1,0,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0,0},
                                {0,0,0,0,1,0,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case '8':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {0,0,1,1,1,1,0,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,0,1,1,1,1,0,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,0,1,1,1,1,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case '9':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {0,0,1,1,1,1,0,0},
                                {0,1,0,0,0,0,1,0},
                                {0,1,0,0,0,0,1,0},
                                {0,0,1,1,1,1,0,0},
                                {0,0,0,0,0,0,1,0},
                                {0,0,0,0,0,0,1,0},
                                {0,0,1,1,1,1,0,0}
                                };
                    Store_GChar(GG);
                }break;
                case '0':{
                    Gchar GG={
                                {0,0,0,0,0,0,0,0},
                                {1,1,1,1,1,1,1,0},
                                {1,0,0,0,0,1,1,0},
                                {1,0,0,0,1,0,1,0},
                                {1,0,0,1,0,0,1,0},
                                {1,0,1,0,0,0,1,0},
                                {1,1,0,0,0,0,1,0},
                                {1,1,1,1,1,1,1,0}
                                };
                    Store_GChar(GG);
                }break;
            }

            for(size_t A=0;A<8;A++){
                for(size_t B=0;B<8;B++){
                    if(GraphicCharacter[A][B]){
                        if(GraphicCharacter[A][B] >0){
                            Place_Pixel((Xoffset*8)+B,(Yoffset*8)+A,GraphicCharacter[A][B]*color);
                        }else{
                            Place_Pixel((Xoffset*8)+B,(Yoffset*8)+A,(GraphicCharacter[A][B]-GraphicCharacter[A][B]-GraphicCharacter[A][B]));
                        }
                    }
                }
            }
            if(Xoffset < MAX_X){
                Xoffset++;
            }else{
                if(Yoffset < MAX_Y){
                    Yoffset++;
                }else{
                    for(size_t row = 1;(row < MAX_Y);row++){
                        for(size_t col = 0;(col < MAX_X);col++){
                            //SetCharacterAt(col,(row-1),GetCharacterAt(col,row));
                        }
                    }
                }
                Xoffset=0;
            }
        }
        void Place_Pixel(uint16_t X,uint16_t Y,uint16_t color){
            GraphicsAddress[(MAX_Xpos*Y)+X]=color;
            Sauce::Terminal::String("Pixel Placed: ");
            Sauce::Terminal::String(Sauce::Convert::To_String::From_uint16(((MAX_Xpos*Y)+X)));
            Sauce::Terminal::String(" = ");
            Sauce::Terminal::String(Sauce::Convert::To_String::From_uint16(color));
            Sauce::Terminal::String(" \n\r");
        }
        void Place_Rectangle(uint16_t startX,uint16_t startY,uint16_t stopX,uint16_t stopY,uint16_t color){
            for(uint16_t iX=startX;iX<stopX;iX++){
                for(uint16_t iY=startY;iY<stopY;iY++){
                    Place_Pixel(iX,iY,color);
                }
            }
        }
        void Place_Vector(_Vector Vector,uint16_t color){
            uint16_t X,Y,StopX,StopY,StartX,StartY;

            StopX=Vector.StopX;
            StartX=Vector.StartX;
            X=Vector.StartX;
            StopY=Vector.StopY;
            StartY=Vector.StartY;
            Y=Vector.StartY;

            while(1){
                Place_Pixel(X,Y,color);

                if(StartX<StopX && X<StopX)X++;
                if(StartY<StopY && Y<StopY)Y++;

                if(StartX>StopX && X>StopX)X--;
                if(StartY>StopY && Y>StopY)Y--;

                if( (Y==StopY) && (X==StopX) )break;
            }
        }
        void Place_EmptySquare(uint16_t Xs,uint16_t Ys,uint16_t Xp,uint16_t Yp,uint16_t color){
            Place_Vector({Xp+(1*Xs),Yp+(1*Ys),Xp+(1*Xs),Yp+(2*Ys)},color);
            Place_Vector({Xp+(1*Xs),Yp+(2*Ys),Xp+(2*Xs),Yp+(2*Ys)},color);
            Place_Vector({Xp+(2*Xs),Yp+(2*Ys)+1,Xp+(2*Xs),Yp+(1*Ys)},color);
            Place_Vector({Xp+(2*Xs),Yp+(1*Ys),Xp+(1*Xs),Yp+(1*Ys)},color);
        }
        
    };
};