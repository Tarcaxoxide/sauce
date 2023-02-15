#include<Sauce/Fauxon/DataTypes/TokenDataType.hpp>
#include<_std/to_string.hpp>
#include<_std/to_double.hpp>
#include<_std/to_int.hpp>
#include<Sauce/IO/Debug/Debug.hpp>
namespace Sauce{
    namespace Fauxon{
        namespace DataTypes{
            namespace TokenSubKind_en{
                _std::string toString(TokenSubKind_en SubKind){
                    Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"TokenSubKind_en::toString",_NAMESPACE_,_ALLOW_PRINT_);
                    _std::string Result="";
                    switch(SubKind){
                        case TokenSubKind_en::__NULL:{Result="__NULL";}break;
                        case TokenSubKind_en::__WORD:{Result="__WORD";}break;
                        case TokenSubKind_en::__WHOLE_NUMBER:{Result="__WHOLE_NUMBER";}break;
                        case TokenSubKind_en::__DECIMAL_NUMBER:{Result="__DECIMAL_NUMBER";}break;
                        case TokenSubKind_en::__PLUS:{Result="__PLUS";}break;
                        case TokenSubKind_en::__MINUS:{Result="__MINUS";}break;
                        case TokenSubKind_en::__MULTIPLY:{Result="__MULTIPLY";}break;
                        case TokenSubKind_en::__DIVIDE:{Result="__DIVIDE";}break;
                        case TokenSubKind_en::__LEFT_ROUND_BRACE:{Result="__LEFT_ROUND_BRACE";}break;
                        case TokenSubKind_en::__RIGHT_ROUND_BRACE:{Result="__RIGHT_ROUND_BRACE";}break;
                        case TokenSubKind_en::__LEFT_SQUARE_BRACE:{Result="__LEFT_SQUARE_BRACE";}break;
                        case TokenSubKind_en::__RIGHT_SQUARE_BRACE:{Result="__RIGHT_SQUARE_BRACE";}break;
                        case TokenSubKind_en::__LEFT__CURLY_BRACE:{Result="__LEFT__CURLY_BRACE";}break;
                        case TokenSubKind_en::__RIGHT_CURLY_BRACE:{Result="__RIGHT_CURLY_BRACE";}break;
                        case TokenSubKind_en::__COMMA:{Result="__COMMA";}break;
                        case TokenSubKind_en::__DOT:{Result="__DOT";}break;
                        case TokenSubKind_en::__COLON:{Result="__COLON";}break;
                        case TokenSubKind_en::__SEMICOLON:{Result="__SEMICOLON";}break;
                        case TokenSubKind_en::__APOSTROPHE:{Result="__APOSTROPHE";}break;
                        case TokenSubKind_en::__QUOTE:{Result="__QUOTE";}break;
                        case TokenSubKind_en::__BACKTICK:{Result="__BACKTICK";}break;
                        case TokenSubKind_en::__BACKSLASH:{Result="__BACKSLASH";}break;
                        case TokenSubKind_en::__EQUALS:{Result="__EQUALS";}break;
                        case TokenSubKind_en::__NOT:{Result="__NOT";}break;
                        case TokenSubKind_en::__LESS:{Result="__LESS";}break;
                        case TokenSubKind_en::__MORE:{Result="__MORE";}break;
                        case TokenSubKind_en::__EQUALS_EQUALS:{Result="__EQUALS_EQUALS";}break;
                        case TokenSubKind_en::__NOT_EQUALS:{Result="__NOT_EQUALS";}break;
                        case TokenSubKind_en::__MORE_EQUALS:{Result="__MORE_EQUALS";}break;
                        case TokenSubKind_en::__LESS_EQUALS:{Result="__LESS_EQUALS";}break;
                        case TokenSubKind_en::__BIGGEST:{Result="__BIGGEST";}break;
                        case TokenSubKind_en::__SMALLEST:{Result="__SMALLEST";}break;
                        case TokenSubKind_en::__ERROR:{Result="__ERROR";}break;
                    };
                    Debugger.Print(Result);
                    return Result;
                }
            };
            TokenDataType_st::TokenDataType_st(TokenSubKind_en::TokenSubKind_en SubKind):BaseDataType_st(Kind_en::__TOKEN,(uint64_t)SubKind){
                RawKind=TokenSubKind_en::__NULL;
                Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"TokenDataType_st::TokenDataType_st(SubKind)",_NAMESPACE_,_ALLOW_PRINT_);
            }
            TokenDataType_st::TokenDataType_st(TokenSubKind_en::TokenSubKind_en SubKind,_std::string Vin):BaseDataType_st(Kind_en::__TOKEN,(uint64_t)SubKind){
                Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"TokenDataType_st::TokenDataType_st(SubKind,Word)",_NAMESPACE_,_ALLOW_PRINT_);
                RawKind=TokenSubKind_en::__WORD;
                Debugger.Print(Vin);
                Value = new _std::string(Vin);
            }
            TokenDataType_st::TokenDataType_st(TokenSubKind_en::TokenSubKind_en SubKind,long double Vin):BaseDataType_st(Kind_en::__TOKEN,(uint64_t)SubKind){
                Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"TokenDataType_st::TokenDataType_st(SubKind,Decimal)",_NAMESPACE_,_ALLOW_PRINT_);
                RawKind=TokenSubKind_en::__DECIMAL_NUMBER;
                Debugger.Print(_std::to_string(Vin));
                Value = new long double(Vin);
            }
            TokenDataType_st::TokenDataType_st(TokenSubKind_en::TokenSubKind_en SubKind,int64_t Vin):BaseDataType_st(Kind_en::__TOKEN,(uint64_t)SubKind){
                Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"TokenDataType_st::TokenDataType_st(SubKind,Number)",_NAMESPACE_,_ALLOW_PRINT_);
                RawKind=TokenSubKind_en::__WHOLE_NUMBER;
                Debugger.Print(_std::to_string(Vin));
                Value = new int64_t(Vin);
            }
            _std::string TokenDataType_st::toString(){
                Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"TokenDataType_st::toString",_NAMESPACE_,_ALLOW_PRINT_);
                _std::string Result="(";
                Result+=DataTypes::Kind_en::toString(Header.Kind);
                Result+=DataTypes::TokenSubKind_en::toString((DataTypes::TokenSubKind_en::TokenSubKind_en)Header.SubKind);
                if(Value != nullptr){
                    Result+=":";
                    if(RawKind == TokenSubKind_en::__WORD){
                        Result+=(*((_std::string*)Value));
                    }else if(RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        Result+=_std::to_string((*((long double*)Value)));
                    }else if(RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        Result+=_std::to_string((*((int64_t*)Value)));
                    }else{
                        Result+="?";
                    }
                }
                Result+=")";
                Debugger.Print(Result);
                return Result;
            }
            _std::string TokenList_st::toString(){
                Sauce::IO::Debug::Debugger_st Debugger(__FILE__,"TokenList_st::toString",_NAMESPACE_,_ALLOW_PRINT_);
                _std::string Result="[";
                for(size_t i=0;i<Size();i++){
                    Result+=(*this)[i].toString();
                }
                Result+="]";
                Debugger.Print(Result);
                return Result;
            }
/////////////////////////////////////////////////////////////////
            void TokenDataType_st::append(const TokenDataType_st& Other){
                if(Value != nullptr){
                    if(RawKind == TokenSubKind_en::__WORD){
                        if(Other.RawKind == TokenSubKind_en::__WORD){
                            (*((_std::string*)Value))+=(*((_std::string*)Other.Value));
                        }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                            (*((_std::string*)Value))+=_std::to_string((*((long double*)Other.Value)));
                        }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                            (*((_std::string*)Value))+=_std::to_string((*((int64_t*)Other.Value)));
                        }
                    }else if(RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        if(Other.RawKind == TokenSubKind_en::__WORD){
                            _std::string tmp = _std::to_string((*((long double*)Value)));
                            tmp+=(*((_std::string*)Other.Value));
                            delete ((long double*)Value);
                            Value = new _std::string(tmp);
                            RawKind=TokenSubKind_en::__WORD;
                            Header.SubKind=TokenSubKind_en::__WORD;
                        }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                            _std::string tmpOther = _std::to_string((*((long double*)Other.Value)));
                            _std::string tmp = _std::to_string((*((long double*)Value)));
                            tmp+=tmpOther;
                            (*((long double*)Value))=_std::to_double(tmp);
                        }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                            _std::string tmpOther = _std::to_string((*((int64_t*)Other.Value)));
                            _std::string tmp = _std::to_string((*((long double*)Value)));
                            tmp+=tmpOther;
                            (*((long double*)Value))=_std::to_double(tmp);
                        }
                    }else if(RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        if(Other.RawKind == TokenSubKind_en::__WORD){
                            _std::string tmp = _std::to_string((*((int64_t*)Value)));
                            tmp+=(*((_std::string*)Other.Value));
                            delete ((int64_t*)Value);
                            Value = new _std::string(tmp);
                            RawKind=TokenSubKind_en::__WORD;
                            Header.SubKind=TokenSubKind_en::__WORD;
                        }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                            _std::string tmpOther = _std::to_string((*((long double*)Other.Value)));
                            _std::string tmp = _std::to_string((*((int64_t*)Value)));
                            tmp+=tmpOther;
                            (*((long double*)Value))=_std::to_int(tmp);
                        }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                            _std::string tmpOther = _std::to_string((*((int64_t*)Other.Value)));
                            _std::string tmp = _std::to_string((*((int64_t*)Value)));
                            tmp+=tmpOther;
                            (*((long double*)Value))=_std::to_int(tmp);
                        }
                    }
                }
            }
/////////////////////////////////////////////////////////////////
            void TokenDataType_st::add(const TokenDataType_st& Other){
                if(RawKind == TokenSubKind_en::__WORD){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]+(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        _std::string tmpOther = _std::to_string((*((long double*)Other.Value)));
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = tmpOther.Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]+tmpOther[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=tmpOther[i];
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        _std::string tmpOther = _std::to_string((*((int64_t*)Other.Value)));
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = tmpOther.Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]+tmpOther[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=tmpOther[i];
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }
                }else if(RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        _std::string tmpi = _std::to_string((*((long double*)Value)));
                        size_t size = tmpi.Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=(tmpi[i]+(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=tmpi[i];
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        (*((long double*)Value))+=(*((long double*)Other.Value));
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        (*((long double*)Value))+=(*((int64_t*)Other.Value));
                    }
                }else if(RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        _std::string tmpi = _std::to_string((*((int64_t*)Value)));
                        size_t size = tmpi.Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=(tmpi[i]+(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=tmpi[i];
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        (*((int64_t*)Value))+=(*((long double*)Other.Value));
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        (*((int64_t*)Value))+=(*((int64_t*)Other.Value));
                    }
                }
            }
/////////////////////////////////////////////////////////////////
            void TokenDataType_st::subtract(const TokenDataType_st& Other){
                if(RawKind == TokenSubKind_en::__WORD){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]-(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        _std::string tmpOther = _std::to_string((*((long double*)Other.Value)));
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = tmpOther.Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]-tmpOther[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=tmpOther[i];
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        _std::string tmpOther = _std::to_string((*((int64_t*)Other.Value)));
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = tmpOther.Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]-tmpOther[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=tmpOther[i];
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }
                }else if(RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        _std::string tmpi = _std::to_string((*((long double*)Value)));
                        size_t size = tmpi.Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=(tmpi[i]-(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=tmpi[i];
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        (*((long double*)Value))-=(*((long double*)Other.Value));
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        (*((long double*)Value))-=(*((int64_t*)Other.Value));
                    }
                }else if(RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        _std::string tmpi = _std::to_string((*((int64_t*)Value)));
                        size_t size = tmpi.Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=(tmpi[i]-(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=tmpi[i];
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        (*((int64_t*)Value))-=(*((long double*)Other.Value));
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        (*((int64_t*)Value))-=(*((int64_t*)Other.Value));
                    }
                }
            }
/////////////////////////////////////////////////////////////////
            void TokenDataType_st::multiply(const TokenDataType_st& Other){
                if(RawKind == TokenSubKind_en::__WORD){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]*(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        _std::string tmpOther = _std::to_string((*((long double*)Other.Value)));
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = tmpOther.Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]*tmpOther[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=tmpOther[i];
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        _std::string tmpOther = _std::to_string((*((int64_t*)Other.Value)));
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = tmpOther.Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]*tmpOther[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=tmpOther[i];
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }
                }else if(RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        _std::string tmpi = _std::to_string((*((long double*)Value)));
                        size_t size = tmpi.Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=(tmpi[i]*(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=tmpi[i];
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        (*((long double*)Value))*=(*((long double*)Other.Value));
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        (*((long double*)Value))*=(*((int64_t*)Other.Value));
                    }
                }else if(RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        _std::string tmpi = _std::to_string((*((int64_t*)Value)));
                        size_t size = tmpi.Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=(tmpi[i]*(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=tmpi[i];
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        (*((int64_t*)Value))*=(*((long double*)Other.Value));
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        (*((int64_t*)Value))*=(*((int64_t*)Other.Value));
                    }
                }
            }
/////////////////////////////////////////////////////////////////
            void TokenDataType_st::divide(const TokenDataType_st& Other){
                if(RawKind == TokenSubKind_en::__WORD){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]/(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        _std::string tmpOther = _std::to_string((*((long double*)Other.Value)));
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = tmpOther.Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]/tmpOther[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=tmpOther[i];
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        _std::string tmpOther = _std::to_string((*((int64_t*)Other.Value)));
                        size_t size = (*((_std::string*)Value)).Size();
                        size_t sizeOther = tmpOther.Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=((*((_std::string*)Value))[i]/tmpOther[i]);
                            }else if(i<size){
                                tmp+=((*((_std::string*)Value))[i]);
                            }else if(i<sizeOther){
                                tmp+=tmpOther[i];
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }
                }else if(RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        _std::string tmpi = _std::to_string((*((long double*)Value)));
                        size_t size = tmpi.Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=(tmpi[i]/(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=tmpi[i];
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        (*((long double*)Value))/=(*((long double*)Other.Value));
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        (*((long double*)Value))/=(*((int64_t*)Other.Value));
                    }
                }else if(RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                    if(Other.RawKind == TokenSubKind_en::__WORD){
                        _std::string tmpi = _std::to_string((*((int64_t*)Value)));
                        size_t size = tmpi.Size();
                        size_t sizeOther = (*((_std::string*)Other.Value)).Size();
                        _std::string tmp="";
                        for(size_t i=0;(i<size||i<sizeOther);i++){
                            if(i<size&&i<sizeOther){
                                tmp+=(tmpi[i]/(*((_std::string*)Other.Value))[i]);
                            }else if(i<size){
                                tmp+=tmpi[i];
                            }else if(i<sizeOther){
                                tmp+=((*((_std::string*)Other.Value))[i]);
                            }
                        }
                        (*((_std::string*)Value))=tmp;
                    }else if(Other.RawKind == TokenSubKind_en::__DECIMAL_NUMBER){
                        (*((int64_t*)Value))/=(*((long double*)Other.Value));
                    }else if(Other.RawKind == TokenSubKind_en::__WHOLE_NUMBER){
                        (*((int64_t*)Value))/=(*((int64_t*)Other.Value));
                    }
                }
            }
        };
    };
};