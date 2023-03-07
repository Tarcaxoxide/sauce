#ifndef __Sauce_Graphics_Basic_Frame
#define __Sauce_Graphics_Basic_Frame
#include<Sauce/Common.hpp>
#include<std/memcpy.hpp>
#include<Sauce/Types.hpp>
#include<std/memcpy.hpp>
#include<Sauce/Math.hpp>
namespace Sauce{
	namespace Graphics{
		namespace Basic{
			struct Frame_st{
				GOP_PixelStructure* PixelBuffer=nullptr;
				int64_t PixelBufferTotalSize,PixelsPerLine,PixelsBufferHeight;
				GOP_PixelStructure ForegroundColor{0xFF,0xFF,0xFF,0xFF},BackgroundColor{0x00,0x00,0x00,0x00};
				Sauce::Point64_st PixelPointer{0,0,0};
				Sauce::Point64_st Offset{0,0,0};
				Frame_st(int64_t PixelBufferTotalSize,int64_t PixelsPerLine,GOP_PixelStructure* Buffer=nullptr){
					//if you don't provide a buffer we create one, else we use the one you provide and we assume your not inaccurate about it's dimensions :)
					if(Buffer == nullptr){PixelBuffer=new GOP_PixelStructure[PixelBufferTotalSize];}else{PixelBuffer=Buffer;}
					this->PixelBufferTotalSize=PixelBufferTotalSize;
					this->PixelsPerLine=PixelsPerLine;
					PixelsBufferHeight=(PixelBufferTotalSize/PixelsPerLine);
					this->Offset=Offset;
				}
				inline bool SetPointer(Sauce::Point64_st pointer){
					if(pointer.X>PixelsPerLine || pointer.X < 0)return false;
					if(pointer.Y>PixelsBufferHeight || pointer.Y < 0)return false;
					PixelPointer=pointer;
					return true;
				}
				inline bool Is_Over(Sauce::Point64_st Location){
					bool Vertical=(Location.Y > Offset.Y/*below of my top?*/) && (Location.Y < Offset.Y+PixelsBufferHeight/*above of my bottom?*/);
					bool Horizontal=(Location.X > Offset.X/*right of my left*/) && (Location.X < Offset.X+PixelsPerLine/*left of my right*/);
					/*
						# = top left and bottom right corners of myself.
						1/0 = is over/is not over.
						000000000000
						0#1111111110
						011111111110
						011111111110
						011111111110
						0111111111#0
						000000000000
					*/
					return Vertical && Horizontal;
				}
				inline Sauce::Point64_st Size(){
					return {PixelsPerLine,PixelsBufferHeight,0};
				}
				inline bool Move(Sauce::Point64_st Location){
					Offset=Location;
					return true;
				}
				inline bool SetColor(GOP_PixelStructure ForegroundColor,GOP_PixelStructure BackgroundColor){
					this->ForegroundColor=ForegroundColor;
					this->BackgroundColor=BackgroundColor;
					return true;
				}
				inline bool SetColorForeground(GOP_PixelStructure ForegroundColor){
					this->ForegroundColor=ForegroundColor;
					return true;
				}
				inline bool SetColorBackground(GOP_PixelStructure BackgroundColor){
					this->BackgroundColor=BackgroundColor;
					return true;
				}
				inline bool ReverseColor(){
					GOP_PixelStructure tmp=ForegroundColor;
					ForegroundColor=BackgroundColor;
					BackgroundColor=tmp;
					return true;
				}
				inline bool RowFill(int64_t RowIndex,GOP_PixelStructure TheColor){
					if(RowIndex > PixelsPerLine){
						return false;
					}
					PixelPointer.X=RowIndex;
					for(PixelPointer.Y=0;PixelPointer.Y<PixelsBufferHeight;PixelPointer.Y++){
						PixelBuffer[Sauce::Math::index(PixelPointer.X,PixelPointer.Y,PixelsPerLine)]=TheColor;
					}
					return true;
				}
				inline bool ColumnFill(int64_t ColumnIndex,GOP_PixelStructure TheColor){
					if(ColumnIndex > PixelsPerLine){
						return false;
					}
					PixelPointer.Y=ColumnIndex;
					for(PixelPointer.X=0;PixelPointer.X<PixelsPerLine;PixelPointer.X++){
						PixelBuffer[Sauce::Math::index(PixelPointer.X,PixelPointer.Y,PixelsPerLine)]=TheColor;
					}
					return true;
				}
				inline bool Fill(GOP_PixelStructure TheColor){
					if(!RowFill(0,TheColor))return false;
					PixelPointer.X=0;
					for(PixelPointer.Y=1;PixelPointer.Y<PixelsBufferHeight;PixelPointer.Y++){
						std::memcpy(PixelBuffer+Sauce::Math::index(PixelPointer.X,PixelPointer.Y-1,PixelsPerLine),PixelBuffer+Sauce::Math::index(PixelPointer.X,PixelPointer.Y,PixelsPerLine),PixelsPerLine);
					}
					return true;
				}
				inline bool Clear(){
					if(!Fill(BackgroundColor))return false;
					PixelPointer.X=0;
					PixelPointer.Y=0;
					PixelPointer.Z=0;
					return true;
				}
				inline bool RowClear(int64_t RowIndex){
					return RowFill(RowIndex,BackgroundColor);
				}
				inline bool ColumnClear(int64_t ColumnIndex){
					return ColumnFill(ColumnIndex,BackgroundColor);
				}
				inline bool PutPixel(Sauce::Point64_st Location,GOP_PixelStructure TheColor){
					if(Location.X > PixelsPerLine||Location.Y > PixelsBufferHeight)return false;
					if(Location.X < 0||Location.Y < 0)return false;
					PixelBuffer[Sauce::Math::index(Location.X,Location.Y,PixelsPerLine)]=TheColor;
					return true;
				}
				inline bool PullPixel(Sauce::Point64_st Location,GOP_PixelStructure& ThatColor){
					if(Location.X > PixelsPerLine||Location.Y > PixelsBufferHeight)return false;
					if(Location.X < 0||Location.Y < 0)return false;
					ThatColor=PixelBuffer[Sauce::Math::index(Location.X,Location.Y,PixelsPerLine)];
					return true;
				}
				inline bool CopyTo(GOP_PixelStructure* OtherPixelBuffer,int64_t OtherPixelBufferTotalSize,int64_t OtherPixelsPerLine,Sauce::Point64_st OtherOffset){
					OtherOffset.X+=Offset.X;
					OtherOffset.Y+=Offset.Y;
					OtherOffset.Z+=Offset.Z;
					if(OtherPixelBufferTotalSize < PixelBufferTotalSize+(OtherOffset.X*OtherOffset.Y))return false;
					PixelPointer.X=0;
					for(PixelPointer.Y=0;PixelPointer.Y<PixelsBufferHeight;PixelPointer.Y++){
						std::memcpy(PixelBuffer+Sauce::Math::index(PixelPointer.X,PixelPointer.Y,PixelsPerLine),OtherPixelBuffer+Sauce::Math::index(PixelPointer.X+OtherOffset.X,PixelPointer.Y+OtherOffset.Y,OtherPixelsPerLine),PixelsPerLine);
						// changed to memcpy, it's a wee bit janky around the edges but eh at least it's faster right :)
					}
					return true;
				}
				inline bool CopyTo(Frame_st* Other){
					return CopyTo(Other->PixelBuffer,Other->PixelBufferTotalSize,Other->PixelsPerLine,Other->Offset);
				}
				inline bool CopyFrom(Frame_st* Other){
					return Other->CopyTo(PixelBuffer,PixelBufferTotalSize,PixelsPerLine,Offset);
				}
				inline bool Resize(Sauce::Point64_st nSize){
					Frame_st tmp(nSize.X*nSize.Y,nSize.X);
					if(!tmp.SetColor(ForegroundColor,BackgroundColor))return false;
					for(tmp.PixelPointer.X=0;tmp.PixelPointer.X<tmp.PixelsPerLine;tmp.PixelPointer.X++){
						for(tmp.PixelPointer.Y=0;tmp.PixelPointer.Y<tmp.PixelsBufferHeight;tmp.PixelPointer.Y++){
							__float128 intermediateX=((__float128)tmp.PixelPointer.X)/((__float128)tmp.PixelsPerLine);
							__float128 intermediateY=((__float128)tmp.PixelPointer.Y)/((__float128)tmp.PixelsBufferHeight);
							int64_t sourceX=intermediateX*PixelsPerLine;
							int64_t sourceY=intermediateY*PixelsBufferHeight;
							GOP_PixelStructure Pixel;
							if(!PullPixel({sourceX,sourceY,0},Pixel))return false;
							if(!tmp.PutPixel({tmp.PixelPointer.X,tmp.PixelPointer.Y,0},Pixel))return false;
						}
					}
					delete PixelBuffer;
					PixelBuffer=tmp.PixelBuffer;
					PixelBufferTotalSize=tmp.PixelBufferTotalSize;
					PixelsPerLine=tmp.PixelsPerLine;
					PixelsBufferHeight=tmp.PixelsBufferHeight;
					SetPointer({0,0,0});
					return true;
				}
			};
		};
	};
};
#endif