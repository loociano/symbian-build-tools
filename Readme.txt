Symbian bmconv, petran and rcomp - SDN Release, 15 Dec 2003
-----------------------------------------------------------

This delivery contains the source for Symbian OS build tools bmconv, petran, uidcrc and
rcomp together with test code that exercises some of their functionality.

bmconv: 
	Bitmaps are built from Windows .bmps into Symbian OS .mbms and 
	(optionally) a .mbg header file containing symbolic IDs. The bmconv 
	tool handles this conversion as part of the application build process 
	(abld), but it can also be used in its own right to convert .mbms back 
	into .bmps. 
petran:
	Is the PE object format translator that processes the final object files
	to the format that the OS loader expects.
uidcrc:
	Provides a mechanism for calculating the checksum for a set of UIDs
rcomp:
	Is the Symbian OS resource compiler which builds the binary resource files
	used most commonly from application controls (localisation etc) at runtime.
	RSC files are compiled using rcomp.
	
The actual source code for the tools is in:

	\gditools\bmconv

	\e32tools\petran
	\e32tools\pefile
	\e32tools\e32uid
	\e32tools\host

	\rcomp\src
	\rcomp\src\derived

All these tools are found in all Symbian OS SDKs as executables in the \Epoc32\tools
directory and are used to postprocess the binaries and applications for deployment on
target devices. A description of this procedure and the use of these tools is presented
in chapter 7 and 14 of the  book 'Symbian OS C++ for Mobile Phones'.
(http://www.symbian.com/books/)

The Symbian source for the tools is released under the terms and conditions of the Symbian
Example Source Code License found at:
	(http://www.symbian.com/developer/techlib/codelicense.html)

In order to build the tools on Win32 you need:
a) a Win32 development environment like MSVC++ 6
b) Any Symbian OS SDK, v6.0 or newer

go into the group directory of each tool and do
bldmake clean
bldmake bldfiles
abld build

All the binaries will be installed in \Epoc32\tools. Some warnings or error may occur 
about undefined builds like narrow-code DEB/REL which is ok. 

In order to port the tools to another OS and or CPU architectures it is best to start with
the howto of the GnuPoc project (http://www.gnupoc.org/). Where you can tool by tool test
any port by replacing the WINE-linked tools with native ones (at least for Linux/x86 :-).

Other such Symbian OS porting resources can be found at :
http://epocemx.sf.net/ 	- EPOCEMX project by Eberhard Mattes (ER5 SDK for Linux and EPOC)
http://www.science-computing.de/o.flebbe/sdk/ - Olaf Flebbe's SDK for Linux
http://www.koeniglich.de/linuxsdk.html  -  Another Symbian OS SDK for Linux

 

enjoy :-)

