// PE_IO.CPP
//
// Copyright (c) 1996-1999 Symbian Ltd.  All rights reserved.
//

#include <fstream.h>
#include <e32image.h>
#include <h_utl.h>
#include <io.h>
#include <string.h>

ofstream &operator<<(ofstream &os, const E32ImageFile &aImage)
//
// Output an E32ImageFile
//
	{

	os.write(aImage.iData, aImage.iSize);
	return os;
	}

ifstream &operator>>(ifstream &is, E32ImageFile &aImage)
//
// Input an E32ImageFile
//
	{

	is.read(aImage.iData, aImage.iSize);
	aImage.iHeader=(E32ImageHeader *)aImage.iData;
	aImage.iExportName=NULL;
	return is;
	}

TInt E32ImageFile::IsE32ImageFile(char *aFileName)
	{

	ifstream ifile(aFileName);
	if(!ifile.is_open())
		return FALSE;
	ifile.setmode(filebuf::binary);
	E32ImageHeader h;
	E32ImageFile f;
	f.iHeader=&h;
	ifile.read((char *)&h, sizeof(E32ImageHeader));
	ifile.close();
	return f.IsValid();
	}

TInt E32ImageFile::Open(const TText *const aFileName)
//
// Open an E32 Image file
//
	{

	_finddata_t fileinfo;
	int ret=_findfirst((char *)aFileName,&fileinfo);
	if (ret==-1) 
		{
		Print(EError,"Cannot open %s for input.\n",aFileName);
		return 1;
		}
	Adjust(fileinfo.size);
	ifstream ifile((char *)aFileName);
	if(!ifile.is_open())
		{
		Print(EError,"Cannot open %s for input.\n",aFileName);
		return 1;
		}
	ifile.setmode(filebuf::binary);
	ifile >> *this;
	ifile.close();
	if (!IsValid())
		return KErrGeneral;
	iFileName=strdup((char *)aFileName);
	if (iFileName==NULL)
		return KErrNoMemory;
	return KErrNone;
	}
