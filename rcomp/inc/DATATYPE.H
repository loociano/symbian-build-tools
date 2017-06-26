// DATATYPE.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#if !defined(__DATATYPE_H__)
#define __DATATYPE_H__
#if !defined(__ARRAY_H__)
#include "array.h"
#endif
#if !defined(__ASTRING_H__)
#include "astring.h"
#endif

typedef unsigned long DataType;

// RCType

class RCType : public ArrayItem
	{
public:
	RCType(DataType ElementType,const String& Name,unsigned short Size);
	virtual ~RCType();
	const String& GetName() const;
	DataType GetElementType() const;
	unsigned short GetSize() const;
	friend ostream& operator<<(ostream& os,RCType& r);
private:
	DataType iElementType;
	String iName;
	unsigned short iSize;
	};

// RCTypeArray

class RCTypeArray : protected Array
	{
public:
	RCTypeArray();
	~RCTypeArray();
	String const& GetName(DataType ElementTypeSought) const;
	unsigned short GetSize(DataType ElementTypeSought) const;
	};

// RCTypeArrayIterator

class RCTypeArrayIterator : public ArrayIterator
	{
public:
	RCTypeArrayIterator(const RCTypeArray& c);
	RCType * operator()();
	};

#endif
