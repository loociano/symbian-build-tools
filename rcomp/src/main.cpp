// MAIN.CPP
//
// Copyright (c) 2000 Symbian Ltd.  All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include "resource.h"
#include "rcstack.h"
#include "datatype.h"
#include "mem.h"
#include "rcbinstr.h"
#include "indextab.h"
#include "linkarra.h"
#include "numval.h"
#include "errorhan.h"
#include "fileline.h"
#include "fileacc.h"
#include "version.h"
#include "ctable.h"
#include "main.h"
#include "tokens.h"

extern NameIdMap* pResourceNameIds;
extern long CurrentId;

void WriteHeaderFile(FILE* aRSG, IndexTable& aIndex)
	{
	IndexTableIterator next(aIndex);
	IndexTableItem * p;
	while( ( p = next() ) != NULL)
		{
		ResourceHeader& r=p->Resource();
		if (r.iLabel.Length()>0 && !r.iLocal)
			{
			r.iLabel.Upper();
			if (r.iFormatAsHex)
				fprintf(aRSG, "#define %-41s 0x%x\n", r.iLabel.GetBuffer(), r.iResourceId);
			else
				fprintf(aRSG, "#define %-41s %d\n", r.iLabel.GetBuffer(), r.iResourceId);
			}
		}
	}

void WriteBitArrayOfResourcesContainingCompressedUnicode(RCBinaryStream& aRSC, const IndexTable& aIndex)
	{
	IndexTableIterator next(aIndex);
	unsigned char bitBuffer = 0;
	int numberOfBitsInBuffer = 0;
	for (;;)
		{
		IndexTableItem* const p = next();
		if (p == NULL)
			{
			if (numberOfBitsInBuffer > 0)
				{
				aRSC.Write(&bitBuffer, 1);
				}
			break;
			}
		if (p->Resource().ContainsCompressedUnicode())
			{
			bitBuffer |= (1 << numberOfBitsInBuffer);
			}
		++numberOfBitsInBuffer;
		if (numberOfBitsInBuffer == 8)
			{
			aRSC.Write(&bitBuffer, 1);
			bitBuffer = 0;
			numberOfBitsInBuffer = 0;
			}
		}
	}

void WriteBinaryResourceData(RCBinaryStream& aRSC, IndexTable& aIndex, int& aSizeOfLargestResourceWhenUncompressed)
	{
	IndexTableIterator next(aIndex);
	IndexTableItem * p;
	while( ( p = next() ) != NULL)
		{
		p->SetOffset(aRSC.GetPosition()); // record start of this resource in the index
		p->Resource().StreamOut(aRSC, aSizeOfLargestResourceWhenUncompressed); // write out binary form of resource
		}
	}

void WriteResourceFile(RCBinaryStream& aRSC, IndexTable& aIndex, LinkArray& aLinks, bool aThirdUidIsOffset)
	{
	char flags=0;
	if (aThirdUidIsOffset)
		{
		flags|=0x01;
		}
	aRSC << flags; // these flags are to be used only by a dictionary-compressing program rather than to be used directly by Bafl when reading non-dictionary-compressed resource files (as output by Rcomp)
	const int positionToOverWriteFrom=aRSC.GetPosition();
	NumericValue twoByteSizeOfLargestResourceWhenUncompressed(L_WORD);
	aRSC << twoByteSizeOfLargestResourceWhenUncompressed;
	WriteBitArrayOfResourcesContainingCompressedUnicode(aRSC, aIndex); // simply makes space for the bit-array without writing anything sensible in it (as we don't yet know which resources will contain compressed Unicode)
	int sizeOfLargestResourceWhenUncompressed=0;
	WriteBinaryResourceData(aRSC, aIndex, sizeOfLargestResourceWhenUncompressed);
	aIndex.SetIndexOffset(aRSC.GetPosition());
	aRSC << aIndex;
	aRSC.SetPosition(positionToOverWriteFrom);
	twoByteSizeOfLargestResourceWhenUncompressed=sizeOfLargestResourceWhenUncompressed;
	aRSC << twoByteSizeOfLargestResourceWhenUncompressed;
	WriteBitArrayOfResourcesContainingCompressedUnicode(aRSC, aIndex); // overwrites the bit array with correct data
	aLinks.OverwriteLinks(aRSC); // this jumps about all over the place, overwriting anywhere where there is a LINK or LLINK

	if(verbose) 
		{
		MOFF; cout << aIndex; cout << endl; MON;
		}
	}

void OutputHelp()
	{
	cerr << "Resource compiler version " << version << " (C) 1997-2001 Symbian Ltd." << endl;
	cerr << "Usage: rcomp [-vpu] [-oRSCFile] [-{uid2,uid3}] [-hHeaderFile] [-sSourceFile] [-iBaseInputFileName]" << endl;
	cerr << "\tv" << "\tverbose" << endl;
	cerr << "\tp" << "\tParser debugging" << endl;
	cerr << endl;
	cerr << "\tu" << "\tGenerate Unicode resource binary" << endl;
	cerr << endl;
	cerr << "If no source file is specified, the source will be read from standard input." << endl;
	cerr << "(-i is used to specify the file given to the preprocessor this " << endl;
	cerr << "    name is used in generating debug output.)" << endl;
	}


GlobalData *pG;
String InputBaseName;

main(int argc, char * argv[])
	{
	cout << "\n";
	int vParam=0;
	logmemorysetting = 0;
	unsigned short	pParam = 0;
	String			DataOutputFileName;
	String			HeaderOutputFileName;
	String			BasePath;
	String			SourceFileName;
	FILE *			pSourceFile;
	char			defaultUidsParameter[]="0 * ";
	char*			uidsParameter=defaultUidsParameter;
	if(argc<=1)
		{
		OutputHelp();
		exit(-1);
		}
	else
		{
		// Look through arguments for ones beginning with '-?'.
		for(int i = 1; i < argc; i++)
			{
			if(* argv[i] == '-')
				{
				char * settings = argv[i] + 1;
			
				if(strchr(settings, '?') )
					{
					OutputHelp();
					exit(-1);
					}
				}
			}

		for(i = 1; i < argc; i++)
			{
			if(* argv[i] == '-')
				{
				char * settings = argv[i] + 1;
			
				if(* settings == 'o' || * settings == 'O')
					{
					DataOutputFileName = (settings + 1);
					continue;
					}
					
				if(* settings == 'h' || * settings == 'H')
					{
					HeaderOutputFileName = (settings + 1);
					continue;
					}

				if(* settings == 'i' || * settings == 'I')
					{
					InputBaseName = (settings + 1);
					String DriveAndDirectory = FileAccess::GetDriveAndDirectory(InputBaseName);
					BasePath = FileAccess::FullPath(DriveAndDirectory);
					continue;
					}

				if(* settings == 's' || * settings == 'S')
					{
					SourceFileName = (settings + 1);
					continue;
					}

				if(* settings == '{')
					{
					uidsParameter = settings + 1;
					char* temp = strchr(uidsParameter, ',');
					if ((temp == NULL) || (temp == uidsParameter) || (strchr(temp + 1, ',') != NULL)) // check that there is *one* comma in this parameter (no more and no less), and that it is not the first thing immediately after the '{'
						{
						OutputHelp();
						exit(-1);
						}
					*temp = ' ';
					temp = strchr(uidsParameter, '}');
					if ((temp == NULL) || (temp[1] != '\0'))
						{
						OutputHelp();
						exit(-1);
						}
					*temp = ' ';
					continue;
					}

				if(strchr(settings, 'u') || strchr(settings, 'U') )
					{
					SourceCharacterSet = String::CP1252;
					TargetCharacterSet = String::Unicode;
					}

				if(strchr(settings, 'v') || strchr(settings, 'V') )
					vParam = 1;
				if(strchr(settings, 'p') || strchr(settings, 'P'))
					pParam = 1;
				}
			}
		}
	if(SourceFileName.Length() == 0)
		{
		pSourceFile = stdin;
		}
	else 
		{
		if((pSourceFile = fopen(SourceFileName.GetBuffer(), "r") ) == NULL)
			{
			cerr << "Failed to open " << SourceFileName << endl;
			exit(-2);
			}
		}
	
	verbose = vParam;

	pG = new GlobalData;
	if (pG==NULL)
		exit(-4);

	pG->FileLineHandler.SetPath(BasePath);

	int ret=ParseSourceFile(pSourceFile, pParam);
	fclose(pSourceFile);

	if (ret != 0)
		{
		cerr << "RCOMP failed with code " << ret << endl;
		exit(ret);
		}
	// A successful parse, now generate the output files

	if(DataOutputFileName.Length() != 0)
		{
		const char* uidcrcTool = "uidcrc.exe";
		
		char uidcrcUIDs[3][100];
		strcpy (uidcrcUIDs[0], "0x101f4a6b");
		strcpy (uidcrcUIDs[1], strtok (uidsParameter, " "));
		strcpy (uidcrcUIDs[2], strtok (NULL, " "));

		bool thirdUidIsOffset=false;
		char* const temp = strchr(uidcrcUIDs[2], '*');		
		if (temp != NULL)
			{
			thirdUidIsOffset=true;
			char fakeUid[20];
			const unsigned int idOfAnyResource = CurrentId; // *must* be unsigned so that when we right-shift it, the top bit doesn't get propagated if its set (i.e. "negative")
			sprintf(fakeUid, "0x%08x", idOfAnyResource >> 12); // use the 20 bits derived from the resource file's NAME as the 3rd UID
			strcpy (uidcrcUIDs[2], fakeUid);
			}

		if (verbose)
			{
			MOFF; cout << uidcrcTool << " " << uidcrcUIDs[0] << " " << uidcrcUIDs[1] << " " << uidcrcUIDs[2] << " " << DataOutputFileName.GetBuffer(); cout << endl; MON;
			}

		const int error = _spawnlp (_P_WAIT,
									uidcrcTool,
									uidcrcTool,
									uidcrcUIDs[0],
									uidcrcUIDs[1],
									uidcrcUIDs[2],
									DataOutputFileName.GetBuffer(),
									NULL);

		if(error != 0)
			{
			cerr << "Failed to write UIDs to " << DataOutputFileName << endl;
			exit(error);
			}
		RCBinaryStream RSCStream;
		RSCStream.OpenForAppend(DataOutputFileName);
		if(! RSCStream.IsOpen())
			{
			cerr << "Failed to open " << DataOutputFileName << endl;
			exit(-2);
			}
		WriteResourceFile(RSCStream, pG->Index, pG->Links, thirdUidIsOffset);
		}
	
	if(HeaderOutputFileName.Length() != 0)
		{
		FILE* RSG;
		RSG = fopen(HeaderOutputFileName.GetBuffer(), "w");
		if(RSG==NULL)
			{
			cerr << "Failed to open " << HeaderOutputFileName << endl;
			exit(-2);
			}
		WriteHeaderFile(RSG, pG->Index);
		fclose(RSG);
		}

	delete pG;

	return 0;
	}

