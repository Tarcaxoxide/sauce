#pragma once
#include<_std/move.hpp>
#include<Sauce/IO/Debug/Debug.hpp>
namespace _std{
    template<typename T> void swap(T& t1, T& t2) {
        Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"strlen",_NAMESPACE_,_ALLOW_PRINT_);
        T temp = move(t1); // or T temp(_std::move(t1));
        t1 = move(t2);
        t2 = move(temp);
    }
};
