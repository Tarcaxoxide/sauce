#pragma once
#include<Sauce/Types.hpp>
#include<Sauce/Memory/DynamicArray.hpp>

namespace Sauce{
	namespace Math{
		size_t index(size_t X,size_t Y,size_t MaxX);
		double round(double number,double medium=0.5);
		double maximum(double number1,double number2);
		double minimum(double number1,double number2);
		double average(double number1,double number2);
		int make_positive(int number);
		int make_negative(int number);
		void make_line(Sauce::Point64_st pointA,Sauce::Point64_st pointB,Sauce::Memory::List_cl<Sauce::Point64_st> &Line);
		void make_circle(Sauce::Point64_st point,int radius,Sauce::Memory::List_cl<Sauce::Point64_st> &Circle);
		void make_rectangle(Sauce::Point64_st pointA,Sauce::Point64_st pointB,Sauce::Memory::List_cl<Sauce::Point64_st> &Rectangle);
		inline int abs(int number){return make_positive(number);}
	};
};
