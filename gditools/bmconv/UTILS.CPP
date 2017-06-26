// UTILS.CPP
//
// Copyright (c) 1997-2003 Symbian Ltd.  All rights reserved.
//

#include "bmconv.h"

int BitmapUtils::ByteWidth(int aPixelWidth,int aBitsPerPixel)
	{
	int wordWidth = 0;

	switch (aBitsPerPixel)
		{
	case 1:
		wordWidth = (aPixelWidth + 31) / 32;
		break;
	case 2:
		wordWidth = (aPixelWidth + 15) / 16;
		break;
	case 4:
		wordWidth = (aPixelWidth + 7) / 8;
		break;
	case 8:
		wordWidth = (aPixelWidth + 3) / 4;
		break;
	case 12:
	case 16:
		wordWidth = (aPixelWidth + 1) / 2;
		break;
	case 24:
		wordWidth = (((aPixelWidth * 3) + 11) / 12) * 3;
		break;
	case 32:
		wordWidth = aPixelWidth;
		break;
	default:
		break;
		};

	return wordWidth * 4;
	}

