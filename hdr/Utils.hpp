#pragma once

#include <stdint.h>
#include <stddef.h>
#include <Memory.hpp>


namespace Sauce{
    namespace Utils{
        size_t String_Length(char* string);
        int String_Compare(const char* s1, const char* s2);
        bool String_Compare_ReturnBool(char* StringA,char* StringB);
        char* Combined_Strings(char* StringA,char* StringB);
        size_t Split(char* str, char d, char** into);
        char *String_Tokenizer(char *str, char *delimiter);
        void Move_By_Factor(uint64_t* Value,uint64_t ByValue);
    };
};