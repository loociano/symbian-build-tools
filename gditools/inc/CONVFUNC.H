// CONVFUNC.H
//
// Copyright (c) 1997-2000 Symbian Ltd.  All rights reserved.
//

#ifndef __CONVFUNC_H__
#define __CONVFUNC_H__

__declspec(dllexport) int EpocBitmapToBmp(unsigned char*& aBmpPtr,int& aBmpSize,const unsigned char* aMbmPtr,int aMbmSize);
__declspec(dllexport) int BmpToEpocBitmap(unsigned char*& aMbmPtr,int& aMbmSize,int& aWidth,int& aHeight,const unsigned char* aBmpPtr,int aBmpSize);

#endif

