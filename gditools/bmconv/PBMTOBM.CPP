// PBMTOBM.CPP
//
// Copyright (c) 1997-2003 Symbian Ltd.  All rights reserved.
//

#include "bmconv.h"

EpocLoader::EpocLoader():
	iPbmBits(NULL)
	{}

EpocLoader::~EpocLoader()
	{
	delete iPbmBits;
	}

int EpocLoader::EpocBitmapCount(char* aFileName, int& aCount, int& isRomFormat)
    {
	fstream file(aFileName,ios::in|ios::nocreate|ios::binary);
	file.setmode(filebuf::binary);
	if (file.is_open()==0)
		return Files;

	long int wordbuffer;
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4)
		return SourceFile;
	if (wordbuffer==KMultiBitmapRomImageUid)
		{
		// ROM format file - next 4 bytes are the number of bitmaps
		isRomFormat=1;
		}
	else
		{
		if (wordbuffer!=KWriteOnceFileStoreUid)
			return SourceFile;
		isRomFormat=0;
		file.read((char *)&wordbuffer,4);
		if (file.gcount()!=4 || wordbuffer!=KMultiBitmapFileImageUid)
			return SourceFile;
		file.read((char *)&wordbuffer,4);
		if (file.gcount()!=4 || wordbuffer!=0)
			return SourceFile;
		file.read((char *)&wordbuffer,4);
		if (file.gcount()!=4 || wordbuffer!=KMultiBitmapFileImageChecksum)
			return SourceFile;
		file.read((char *)&wordbuffer,4);
		if (file.gcount()!=4)
			return SourceFile;
		file.seekg(wordbuffer,ios::beg);
		}
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4)
		return SourceFile;
	aCount=wordbuffer;
	return NoError;
    }

int EpocLoader::LoadEpocBitmap(char* aFileName,int aIndex)
	{
	fstream file(aFileName,ios::in|ios::nocreate|ios::binary);
	file.setmode(filebuf::binary);
	if (file.is_open()==0)
		return Files;

	long int wordbuffer;
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4)
		return SourceFile;
	file.close();

	if (wordbuffer==KMultiBitmapRomImageUid)
		return LoadRom(aFileName,aIndex);

	return LoadFile(aFileName,aIndex);
	}

int EpocLoader::LoadFile(char* aFileName,int aIndex)
	{
	fstream file(aFileName,ios::in|ios::nocreate|ios::binary);
	file.setmode(filebuf::binary);
	if (file.is_open()==0)
		return Files;

	long int wordbuffer;
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4 || wordbuffer!=KWriteOnceFileStoreUid)
		return SourceFile;
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4 || wordbuffer!=KMultiBitmapFileImageUid)
		return SourceFile;
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4 || wordbuffer!=0)
		return SourceFile;
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4 || wordbuffer!=KMultiBitmapFileImageChecksum)
		return SourceFile;
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4)
		return SourceFile;
	file.seekg(wordbuffer,ios::beg);
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4)
		return SourceFile;

	int numsources=wordbuffer;
	if (aIndex >= numsources)
		return OutOfRange;
	file.seekg(aIndex*4,ios::cur);
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4)
		return SourceFile;
	file.seekg(wordbuffer,ios::beg);
	int ret = DoLoadFile(file);
	file.close();
	return ret;
	}

int EpocLoader::LoadRom(char* aFileName,int aIndex)
	{
	fstream file(aFileName,ios::in|ios::nocreate|ios::binary);
	file.setmode(filebuf::binary);
	if (file.is_open()==0)
		return Files;

	long int wordbuffer;
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4)
		return SourceFile;
	if (wordbuffer!=KMultiBitmapRomImageUid)
		return SourceFile;
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4)
		return SourceFile;
	if (aIndex>=wordbuffer)
		return OutOfRange;
	file.seekg(aIndex*4,ios::cur);
	file.read((char *)&wordbuffer,4);
	if (file.gcount()!=4)
		return SourceFile;
	file.seekg(wordbuffer,ios::beg);
	int ret=DoLoadRom(file);
	file.close();
	return ret;
	}

int EpocLoader::DoLoadFile(fstream& aFile)
	{
	long size = sizeof(SEpocBitmapHeader);
	aFile.read((char *)&iPbmHeader,size);
	if (aFile.gcount()!=size)
		return SourceFile;
	iOriginalPbmHeader = iPbmHeader;
	size=iPbmHeader.iBitmapSize-iPbmHeader.iStructSize;

	iPbmBits=new char[size];
	if (iPbmBits==NULL)
		return NoMemory;
	memset(iPbmBits,0xff,size);
	aFile.read(iPbmBits,size);
	aFile.close();
	if (aFile.gcount() != size)
		return SourceFile;

	if (iPbmHeader.iCompression != ENoBitmapCompression)
		{
		int byteWidth = BitmapUtils::ByteWidth(iPbmHeader.iWidthInPixels,iPbmHeader.iBitsPerPixel);
		int expandedsize = byteWidth * iPbmHeader.iHeightInPixels;
		char* newbits = new char[expandedsize];
		if (newbits == NULL)
			return NoMemory;
		memset(newbits,0xff,expandedsize);
		int ret = NoError;
		switch (iPbmHeader.iCompression)
			{
		case EByteRLECompression:
			ret = ExpandByteRLEData(newbits,expandedsize,iPbmBits,size);
			break;
		case ETwelveBitRLECompression:
			ret = ExpandTwelveBitRLEData(newbits,expandedsize,iPbmBits,size);
			break;
		case ESixteenBitRLECompression:
			ret = ExpandSixteenBitRLEData(newbits,expandedsize,iPbmBits,size);
			break;
		case ETwentyFourBitRLECompression:
			ret = ExpandTwentyFourBitRLEData(newbits,expandedsize,iPbmBits,size);
			break;
		default:
			ret = UnknownCompression;
			break;
			}
		delete iPbmBits;
		iPbmBits = newbits;
		iPbmHeader.iCompression = ENoBitmapCompression;
		iPbmHeader.iBitmapSize += expandedsize-size;
		return ret;
		}

	return NoError;
	}

int EpocLoader::DoLoadRom(fstream& aFile)
	{
	Bitmap bmp;
	long size=sizeof(Bitmap);
	aFile.read((char*)&bmp,size);
	if (aFile.gcount() != size)
		return SourceFile;

	size = bmp.iHeader.iBitmapSize - sizeof(SEpocBitmapHeader);
	iPbmBits = new char[size];
	if (iPbmBits == NULL)
		return NoMemory;
	memset(iPbmBits,0xff,size);

	aFile.read(iPbmBits,size);
	if (aFile.gcount() != size)
		return SourceFile;
	iPbmHeader = bmp.iHeader;
	iOriginalPbmHeader = iPbmHeader;
	return NoError;
	}

int EpocLoader::ExpandByteRLEData(char* aDest,int aDestSize,char* aSrce,int aSrceSize)
	{
	char* srcelimit=aSrce+aSrceSize;
	char* destlimit=aDest+aDestSize;
	while(aSrce<srcelimit && aDest<destlimit)
		{
		char count=*aSrce++;
		if (count<0)
			{
			int runLength=-count;
			memcpy(aDest,aSrce,runLength);
			aSrce+=runLength;
			aDest+=runLength;
			}
		else
			{
			char value=*aSrce++;
			while(count>=0)
				{
				*aDest++=value;
				count--;
				}
			}
		}
	if (aSrce!=srcelimit || aDest!=destlimit)
		return DecompressionError;
	return NoError;
	}

int EpocLoader::ExpandTwelveBitRLEData(char* aDest,int aDestSizeInBytes,char* aSrce,int aSrceSizeInBytes)
	{
	unsigned short* srcePtr = (unsigned short*)aSrce;
	unsigned short* destPtr = (unsigned short*)aDest;
	unsigned short* srcePtrLimit = srcePtr + (aSrceSizeInBytes / 2);
	unsigned short* destPtrLimit = destPtr + (aDestSizeInBytes / 2);

	while (srcePtr < srcePtrLimit && destPtr < destPtrLimit)
		{
		unsigned short value = *srcePtr++;
		int runLength = (value >> 12) + 1;
		value &= 0x0fff;

		for (;runLength > 0;runLength--)
			*destPtr++ = value;
		}

	if (srcePtr != srcePtrLimit || destPtr != destPtrLimit)
		return DecompressionError;

	return NoError;
	}

int EpocLoader::ExpandSixteenBitRLEData(char* aDest,int aDestSizeInBytes,char* aSrce,int aSrceSizeInBytes)
	{
	char* srcePtrLimit = aSrce + aSrceSizeInBytes;
	unsigned short* destPtr = (unsigned short*)aDest;
	unsigned short* destPtrLimit = destPtr + (aDestSizeInBytes / 2);

	while (aSrce < srcePtrLimit && destPtr < destPtrLimit)
		{
		int runLength = *aSrce++;

		if (runLength >= 0)
			{
			unsigned short value = *((unsigned short*)(aSrce));
			aSrce += 2;
			for (runLength++; runLength > 0; runLength--)
				*destPtr++ = value;
			}
		else
			{
			runLength = -runLength;
			int byteLength = runLength * 2;
			memcpy(destPtr,aSrce,byteLength);
			aSrce += byteLength;
			destPtr += runLength;
			}
		}

	if (aSrce != srcePtrLimit || destPtr != destPtrLimit)
		return DecompressionError;

	return NoError;
	}

int EpocLoader::ExpandTwentyFourBitRLEData(char* aDest,int aDestSizeInBytes,char* aSrce,int aSrceSizeInBytes)
	{
	char* srcePtrLimit = aSrce + aSrceSizeInBytes;
	char* destPtrLimit = aDest + aDestSizeInBytes;

	while (aSrce < srcePtrLimit && aDest < destPtrLimit)
		{
		int runLength = *aSrce++;

		if (runLength >= 0)
			{
			char component1 = *aSrce++;
			char component2 = *aSrce++;
			char component3 = *aSrce++;
			for (runLength++; runLength > 0; runLength--)
				{
				*aDest++ = component1;
				*aDest++ = component2;
				*aDest++ = component3;
				}
			}
		else
			{
			runLength = -runLength;
			int byteLength = runLength * 3;
			memcpy(aDest,aSrce,byteLength);
			aSrce += byteLength;
			aDest += byteLength;
			}
		}

	if (aSrce != srcePtrLimit || aDest != destPtrLimit)
		return DecompressionError;

	return NoError;
	}

TRgb EpocLoader::GetPixel(int aXCoord,int aYCoord)
	{
	unsigned char col;
	aXCoord%=iPbmHeader.iWidthInPixels;
	aYCoord%=iPbmHeader.iHeightInPixels;
	int byteWidth = BitmapUtils::ByteWidth(iPbmHeader.iWidthInPixels,iPbmHeader.iBitsPerPixel);
	int yOffset = aYCoord * byteWidth;

	switch(iPbmHeader.iBitsPerPixel)
		{
		case 1:
			col = iPbmBits[yOffset + (aXCoord / 8)];
			col >>= (aXCoord&7);
			return TRgb::Gray2(col & 1);
		case 2:
			col = iPbmBits[yOffset + (aXCoord / 4)];
			col = (unsigned char)(col>>(2*(aXCoord%4)));
			return TRgb::Gray4(col & 3);
		case 4:
			col = iPbmBits[yOffset + (aXCoord / 2)];
			if (aXCoord & 1)
				col >>= 4;
			col &= 0xf;
			if (iPbmHeader.iColor==EColorBitmap)
				return TRgb::Color16(col);
			else
				return TRgb::Gray16(col);
		case 8:
			col=iPbmBits[yOffset + aXCoord];
			if (iPbmHeader.iColor==EColorBitmap)
				return TRgb::Color256(col);
			else
				return TRgb::Gray256(col);
		case 12:
		case 16:
			{
			unsigned short* shortPtr = (unsigned short*)&iPbmBits[yOffset + (aXCoord * 2)];
			if (iPbmHeader.iBitsPerPixel == 12)
				return TRgb::Color4K(*shortPtr);
			else
				return TRgb::Color64K(*shortPtr);
			}
		case 24:
			{
			char* pixelPtr = iPbmBits + yOffset + (aXCoord * 3);
			TRgb pixelColor;
			pixelColor.iBlue = *pixelPtr++;
			pixelColor.iGreen = *pixelPtr++;
			pixelColor.iRed = *pixelPtr;
			return pixelColor;
			}
		case 32:
			{
			char* pixelPtr = iPbmBits + yOffset + (aXCoord * 4);
			TRgb pixelColor;
			pixelColor.iBlue = *pixelPtr++;
			pixelColor.iGreen = *pixelPtr++;
			pixelColor.iRed = *pixelPtr++;
			pixelColor.iSpare = *pixelPtr;
			return pixelColor;
			}
		default:
			return TRgb(0);
		}
	}

int EpocLoader::SaveBitmap(char* aFileName)
	{
	TBitmapFileHeader fileheader;
	TBitmapInfoHeader bmpHeader;
	char* bmpBits;

	bmpHeader.biSize = sizeof(TBitmapInfoHeader);
	bmpHeader.biWidth = iPbmHeader.iWidthInPixels;
	bmpHeader.biHeight = iPbmHeader.iHeightInPixels;
	bmpHeader.biPlanes = 1;
	bmpHeader.biBitCount = 24;
	bmpHeader.biCompression = 0;
	bmpHeader.biSizeImage = 0;
	bmpHeader.biXPelsPerMeter = 0;
	bmpHeader.biYPelsPerMeter = 0;
	bmpHeader.biClrUsed = 0;
	bmpHeader.biClrImportant = 0;

	long byteWidth = ((bmpHeader.biWidth * 3) + 3) & ~3;
	long destlength = bmpHeader.biHeight * byteWidth;

	fileheader.bfType = 'B'+('M'<<8);
	fileheader.bfSize = sizeof(TBitmapFileHeader)+sizeof(TBitmapInfoHeader)+destlength;
	fileheader.bfReserved1 = 0;
	fileheader.bfReserved2 = 0;
	fileheader.bfOffBits = sizeof(TBitmapFileHeader)+sizeof(TBitmapInfoHeader);

	bmpBits = new char[destlength];
	if (bmpBits == NULL)
		return NoMemory;
	memset(bmpBits,0xff,destlength);

	for(long y=0;y<bmpHeader.biHeight;y++)
		{
		char* dest=&bmpBits[(bmpHeader.biHeight-y-1)*byteWidth];
		for(long x=0;x<bmpHeader.biWidth;x++)
			{
			TRgb pixel=GetPixel(x,y);
			*dest++=pixel.iBlue;
			*dest++=pixel.iGreen;
			*dest++=pixel.iRed;
			}
		}

	fstream file(aFileName,ios::out);
	file.setmode(filebuf::binary);
	if (file.is_open()==0)
		{
		delete bmpBits;
		return DestFile;
		}

	file.write((char *)&fileheader,sizeof(TBitmapFileHeader));
	file.write((char *)&bmpHeader,sizeof(TBitmapInfoHeader));
	file.write((char *)bmpBits,destlength);
	file.close();

	delete bmpBits;
	return NoError;
	}

int EpocLoader::DupBitmap(SEpocBitmapHeader*& aPbm)
	{
	char* newPbm = new char[iPbmHeader.iBitmapSize];
	if (newPbm == NULL)
		return NoMemory;
	memcpy(newPbm, &iPbmHeader, sizeof(SEpocBitmapHeader));
	memcpy(newPbm+sizeof(SEpocBitmapHeader), iPbmBits, iPbmHeader.iBitmapSize - sizeof(SEpocBitmapHeader));
	aPbm = (SEpocBitmapHeader*)newPbm;
	return NoError;
	}
