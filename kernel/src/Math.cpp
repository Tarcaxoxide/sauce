
#include<Sauce/Math.hpp>
#include<Sauce/IO/Debug/Console.hpp>
#include<Sauce/Utility/Conversion.hpp>
#include<std/iostream.hpp>
#include<Sauce/IO/Debug/Debug.hpp>
#include<Sauce/Interrupts/PIT.hpp>

namespace Sauce{
	namespace Math{
		double sqrt (double number){
			if(number==2)return (double)ROOT2;
			if(number==3)return (double)ROOT3;
			bool negative=false;
			if(number<0){number=-number;negative=true;}
			double res;
			asm volatile("fsqrt" : "=t" (res) : "0" (number));
			if(negative)res=-res;
			return res;
		}
		double inverse_sqrt(double number){
			return 1/sqrt(number);
		}
		size_t index(size_t X,size_t Y,size_t MaxX){
			Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"index",_NAMESPACE_,_ALLOW_PRINT_);
			return (X + (Y * MaxX));
		}
		long double round(long double number,long double medium){
			Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"round",_NAMESPACE_,_ALLOW_PRINT_);
			int A1=(int)number;
			long double A2=number-A1;
			long double A=0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001;
			A=(long double)A1;
			if(A2 > medium)A+=1.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001;
			return (long double)A;
		}
		long double maximum(long double number1,long double number2){
			Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"maximum",_NAMESPACE_,_ALLOW_PRINT_);
			if(number1>number2)return number1;
			if(number2>number1)return number2;
			return number1;// their equal so it doesn't matter.
		}
		long double minimum(long double number1,long double number2){
			Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"minimum",_NAMESPACE_,_ALLOW_PRINT_);
			if(number1<number2)return number1;
			if(number2<number1)return number2;
			return number1;// their equal so it doesn't matter.
		}
		long double average(long double number1,long double number2){
			Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"average",_NAMESPACE_,_ALLOW_PRINT_);
			return (number1+number2)/2;
		}
		long double difference(long double number1,long double number2){
			Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"difference",_NAMESPACE_,_ALLOW_PRINT_);
			return Sauce::Math::maximum(number1,number2)/Sauce::Math::minimum(number1,number2);
			//I can't remember why i'm doing division here and not substraction.
		}
		int make_positive(int number){
			Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"make_positive",_NAMESPACE_,_ALLOW_PRINT_);
			if(number < 0)return -number;
			return number;
		}
		int make_negative(int number){
			Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"make_negative",_NAMESPACE_,_ALLOW_PRINT_);
			if(number > 0)return -number;
			return number;
		}
		size_t next = 1;
		size_t random_get(size_t max){
			Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"random_get",_NAMESPACE_,_ALLOW_PRINT_);
			next = next * 1103515245 + 12345;
			return (next/65536) % max;
		}
		void random_seed(size_t seed){
			Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"random_seed",_NAMESPACE_,_ALLOW_PRINT_);
			next = (seed?seed:(Sauce::Interrupts::PIT::GetTimeSinceBoot()*10000));
		}
		int64_t circular_add(int64_t leftHandSide,int64_t rightHandSide,int64_t circumferenceMinimal,int64_t circumferenceMaximal){
			int64_t result = leftHandSide+rightHandSide;
			int64_t range = circumferenceMaximal-circumferenceMinimal;
			while(result>=circumferenceMaximal)result-=range;
			while(result<circumferenceMinimal)result+=range;
			return result;
		}
		int64_t circular_subtract(int64_t leftHandSide,int64_t rightHandSide,int64_t circumferenceMinimal,int64_t circumferenceMaximal){
			int64_t result = leftHandSide-rightHandSide;
			int64_t range = circumferenceMaximal-circumferenceMinimal;
			while(result>=circumferenceMaximal)result-=range;
			while(result<circumferenceMinimal)result+=range;
			return result;
		}
		int64_t circular_multiply(int64_t leftHandSide,int64_t rightHandSide,int64_t circumferenceMinimal,int64_t circumferenceMaximal){
			int64_t result = leftHandSide*rightHandSide;
			int64_t range = circumferenceMaximal-circumferenceMinimal;
			while(result>=circumferenceMaximal)result-=range;
			while(result<circumferenceMinimal)result+=range;
			return result;
		}
		int64_t circular_divide(int64_t leftHandSide,int64_t rightHandSide,int64_t circumferenceMinimal,int64_t circumferenceMaximal){
			int64_t result = leftHandSide/rightHandSide;
			int64_t range = circumferenceMaximal-circumferenceMinimal;
			while(result>=circumferenceMaximal)result-=range;
			while(result<circumferenceMinimal)result+=range;
			return result;
		}
		int64_t circular_shortest_difference(int64_t leftHandSide,int64_t rightHandSide,int64_t circumferenceMinimal,int64_t circumferenceMaximal){
			int64_t counter_clockwise = circular_subtract(rightHandSide,leftHandSide,circumferenceMinimal,circumferenceMaximal);
			int64_t clockwise = circular_subtract(leftHandSide,rightHandSide,circumferenceMinimal,circumferenceMaximal);
			return counter_clockwise>clockwise?clockwise:-counter_clockwise;
		}
	};
};
