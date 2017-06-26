// PETRAN.CPP
//
// Copyright (c) 1996-1999 Symbian Ltd.  All rights reserved.
//

//
// PE file preprocessor for E32
//

#include <io.h>
#include <string.h>
#include <strstrea.h>
#include <e32std.h>
#include <e32image.h>
#include <h_utl.h>
#include <h_ver.h>

extern int gAlignConstSection;
extern TUint gConstSectionAddressMask;

int gVerbose=FALSE;
char *gFile1=NULL;
char *gFile2=NULL;
unsigned int gStack=0;
unsigned int gHeapMin=0;
unsigned int gHeapMax=0;
TUid gUid1=KNullUid;
TUid gUid2=KNullUid;
TUid gUid3=KNullUid;
int gCallEntryPoints=TRUE;
int gFixedAddress=FALSE;
int gPriority=EPriorityForeground;
int gAllowDllData=FALSE;
TUint gDataBase=NULL;

int gSetStack=FALSE;
int gSetHeap=FALSE;
int gSetUid1=FALSE;
int gSetUid2=FALSE;
int gSetUid3=FALSE;
int gSetCallEntryPoints=FALSE;
int gSetFixedAddress=FALSE;
int gSetPriority=FALSE;

TBool gLittleEndian=ETrue;

int dotran(char *ifilename, char *ofilename)
	{
	E32ImageFile f;
	int r=f.Translate((TText *)ifilename, gDataBase, gAllowDllData);
	if (r!=KErrNone)
		return r;
	if (gSetStack)
		f.SetStackSize(gStack);
	if (gSetHeap)
		{
		f.SetHeapSizeMin(gHeapMin);
		f.SetHeapSizeMax(gHeapMax);
		}
	if (!gSetUid1)
		gUid1=TUid::Uid(f.iHeader->iUid1);
	if (!gSetUid2)
		gUid2=TUid::Uid(f.iHeader->iUid2);
	if (!gSetUid3)
		gUid3=TUid::Uid(f.iHeader->iUid3);
	f.SetUids(gUid1, gUid2, gUid3);
	if (gSetCallEntryPoints)
		f.SetCallEntryPoints(gCallEntryPoints);
	if (gSetPriority)
		{
		if (f.iHeader->iFlags&KImageDll)
			Print(EWarning,"Cannot set priority of a DLL.\n");
		else
			f.SetPriority((TProcessPriority)gPriority);
		}
	if (gSetFixedAddress)
		{
		if (f.iHeader->iFlags&KImageDll)
			Print(EWarning,"Cannot set fixed address for DLL.\n");
		else
			f.SetFixedAddress(gFixedAddress);
		}
	ofstream ofile(ofilename, ios::binary);
	if (!ofile)
		{
		Print(EError,"Cannot open %s for output.\n",ofilename);
		return 1;
		}
	ofile << f;
	ofile.close();
	if (gVerbose)
		f.Dump((TText *)ofilename);
	return KErrNone;
	}


int dodump(char *ifilename)
	{

	E32ImageFile f;
	_finddata_t fileinfo;
	int ret=_findfirst(ifilename,&fileinfo);
	if (ret==-1)
		{
		Print(EError,"Cannot open %s for input.\n",ifilename);
		return 1;
		}
	f.Adjust(fileinfo.size);

	ifstream ifile(ifilename);
	if(!ifile.is_open())
		{
		Print(EError,"Cannot open %s for input.\n",ifilename);
		return 1;
		}
	ifile.setmode(filebuf::binary);
	ifile >> f;
	ifile.close();
	f.Dump((TText *)ifilename);
	return KErrNone;
	}

int doalter(char *ifilename)
	{

	E32ImageFile f;
	_finddata_t fileinfo;
	int ret=_findfirst(ifilename,&fileinfo);
	if (ret==-1) 
		{
		Print(EError,"Cannot open %s for input.\n",ifilename);
		return 1;
		}
	f.Adjust(fileinfo.size);

	ifstream ifile(ifilename);
	if(!ifile.is_open())
		{
		Print(EError,"Cannot open %s for input.\n",ifilename);
		return 1;
		}
	ifile.setmode(filebuf::binary);
	ifile >> f;
	ifile.close();

	if (gDataBase)
		{
		Print(EWarning, "Ignoring -datalinkaddress Switch");
		}
	if (gSetStack)
		f.SetStackSize(gStack);
	if (gSetHeap)
		{
		f.SetHeapSizeMin(gHeapMin);
		f.SetHeapSizeMax(gHeapMax);
		}
	if (!gSetUid1)
		gUid1=TUid::Uid(f.iHeader->iUid1);
	if (!gSetUid2)
		gUid2=TUid::Uid(f.iHeader->iUid2);
	if (!gSetUid3)
		gUid3=TUid::Uid(f.iHeader->iUid3);
	f.SetUids(gUid1, gUid2, gUid3);
	if (gSetCallEntryPoints)
		f.SetCallEntryPoints(gCallEntryPoints);
	if (gSetPriority)
		{
		if (f.iHeader->iFlags&KImageDll)
			Print(EWarning,"Cannot set priority of a DLL.\n");
		else
			f.SetPriority((TProcessPriority)gPriority);
		}
	if (gSetFixedAddress)
		{
		if (f.iHeader->iFlags&KImageDll)
			Print(EWarning,"Cannot set fixed address for DLL.\n");
		else
			f.SetFixedAddress(gFixedAddress);
		}
	ofstream ofile(ifilename, ios::binary);
	if (!ofile)
		{
		Print(EError,"Cannot open %s for output.\n",ifilename);
		return 1;
		}
	ofile << f;
	ofile.close();
	if (gVerbose)
		f.Dump((TText *)ifilename);
	return KErrNone;
	}

int helpme(char *aStr)
	{
	Print(EAlways,"Syntax: %s [options] pefile outputfile\n",aStr);
	Print(EAlways,"        %s [options] e32imagefile\n",aStr);
	Print(EAlways,"option: [-v] [[-no]call[entrypoint]] [-priority <priority>]\n");
	Print(EAlways,"        [-stack <size>] [-heap <min> <max>] [-uid<n> <uid>]\n");
	Print(EAlways,"        [-allowdlldata] [-datalinkaddress <base>] [-fixed] [-moving]\n");
	Print(EAlways,"        [-align-const-section] [-const-section-address-mask <mask>]\n");
	return KErrArgument;
	}

int isNumber(char *aStr)
	{
	return (aStr[0]>='0') && (aStr[0]<='9');
	}

int getUIntArg(unsigned int &aVal, int argc, char *argv[], int i)
	{
	if (i>=argc)
		return KErrArgument;
	if (!isNumber(argv[i]))
		return KErrArgument;
	istrstream s(argv[i], strlen(argv[i]));
	s>>aVal;
	return KErrNone;
	}

int getPriorityArg(int &aVal, int argc, char *argv[], int i)
	{

	if (i>=argc)
		return KErrArgument;
	if (isNumber(argv[i]))
		{
		istrstream s(argv[i], strlen(argv[i]));
		s>>aVal;
		}
	else
		{
		if (stricmp(argv[i], "low")==0)
			aVal=EPriorityLow;
		else if (strnicmp(argv[i], "background",4)==0)
			aVal=EPriorityBackground;
		else if (strnicmp(argv[i], "foreground",4)==0)
			aVal=EPriorityForeground;
		else if (stricmp(argv[i], "high")==0)
			aVal=EPriorityHigh;
		else if (strnicmp(argv[i], "windowserver",3)==0)
			aVal=EPriorityWindowServer;
		else if (strnicmp(argv[i], "fileserver",4)==0)
			aVal=EPriorityFileServer;
		else if (strnicmp(argv[i], "realtime",4)==0)
			aVal=EPriorityRealTimeServer;
		else if (strnicmp(argv[i], "supervisor",3)==0)
			aVal=EPrioritySupervisor;
		else
			{
			Print(EError, "Unrecognised priority\n");
			return KErrArgument;
			}
		}
	if (aVal<EPriorityLow || aVal>EPrioritySupervisor)
		{
		Print(EError, "Priority out of range\n");
		return KErrArgument;
		}
	return KErrNone;
	}


int processCL(int argc, char *argv[])
	{

	int r=KErrNone;
	int i=1;
	while (i<argc)
		{
		if (stricmp("-v", argv[i])==0)
			gVerbose=TRUE;
		else if (stricmp("-stack", argv[i])==0)
			{
			i++;
			gSetStack=TRUE;
			r=getUIntArg(gStack, argc, argv, i);
			}
		else if (stricmp("-uid1", argv[i])==0)
			{
			i++;
			gSetUid1=TRUE;
			unsigned int id;
			r=getUIntArg(id, argc, argv, i);
			gUid1=TUid::Uid(id);
			}
		else if (stricmp("-uid2", argv[i])==0)
			{
			i++;
			gSetUid2=TRUE;
			unsigned int id;
			r=getUIntArg(id, argc, argv, i);
			gUid2=TUid::Uid(id);
			}
		else if (stricmp("-uid3", argv[i])==0)
			{
			i++;
			gSetUid3=TRUE;
			unsigned int id;
			r=getUIntArg(id, argc, argv, i);
			gUid3=TUid::Uid(id);
			}
		else if (strnicmp("-nocall", argv[i], 7)==0)
			{
			gSetCallEntryPoints=TRUE;
			gCallEntryPoints=FALSE;
			}
		else if (strnicmp("-call", argv[i], 5)==0)
			{
			gSetCallEntryPoints=TRUE;
			gCallEntryPoints=TRUE;
			}
		else if (strnicmp("-fixed", argv[i], 3)==0)
			{
			gSetFixedAddress=TRUE;
			gFixedAddress=TRUE;
			}
		else if (strnicmp("-moving", argv[i], 3)==0)
			{
			gSetFixedAddress=TRUE;
			gFixedAddress=FALSE;
			}
		else if (strnicmp("-priority", argv[i], 4)==0)
			{
			i++;
			gSetPriority=TRUE;
			r=getPriorityArg(gPriority,argc,argv,i);
			}
		else if (strnicmp("-heap", argv[i], 4)==0)
			{
			i++;
			gSetHeap=TRUE;
			r=getUIntArg(gHeapMin, argc, argv, i);
			if (r==KErrNone)
				r=getUIntArg(gHeapMax, argc, argv, ++i);
			}
		else if (strnicmp("-allowdlldata", argv[i], 6)==0)
			{
			gAllowDllData=TRUE;
			}
		else if (strnicmp("-datalinkaddress", argv[i], 5)==0)
			{
			i++;
			r=getUIntArg(gDataBase, argc, argv, i);
			}
		else if (strnicmp("-align-const-section", argv[i], 20)==0)
			{
			gAlignConstSection=TRUE;
			}
		else if (strnicmp("-const-section-address-mask", argv[i], 27)==0)
			{
			i++;
			r=getUIntArg(gConstSectionAddressMask, argc, argv, i);
			}
		else if (gFile1==NULL)
			{
			gFile1=argv[i];
			}
		else if (gFile2==NULL)
			{
			gFile2=argv[i];
			}
		else
			r=KErrArgument;
		if (r!=KErrNone)
			return r;
		i++;
		}
	return KErrNone;
	}

int main(int argc, char *argv[])
	{
	Print(EAlways,"\nPETRAN - PE file preprocessor");
  	Print(EAlways," V%02d.%02d (Build %03d)\n",MajorVersion,MinorVersion,Build);
  	Print(EAlways,Copyright);

	int r=processCL(argc, argv);
	if (r!=KErrNone)
		return helpme(argv[0]);
	if (gFile2)
		return dotran(gFile1, gFile2);
	if ((gSetStack || gSetUid1 || gSetUid2 || gSetUid3 || gSetCallEntryPoints || gSetPriority || gSetHeap) && gFile1)
		return doalter(gFile1);
	if (gFile1)
		return dodump(gFile1);
	helpme(argv[0]);
	return KErrArgument;
	}
