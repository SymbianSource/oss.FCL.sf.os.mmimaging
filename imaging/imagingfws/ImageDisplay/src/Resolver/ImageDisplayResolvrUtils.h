// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __IMAGEDISPLAYRESOLVRUTILS_H__
#define __IMAGEDISPLAYRESOLVRUTILS_H__

#include <e32base.h>

/** @file
	@internalComponent */

const TInt KIclPluginFrameworkVersionOne = 1;	// default resource format version number
const TInt KIclPluginFrameworkVersionMax = 1;	// current max "legal" version


const TInt KIndexForVersion = 0;		// index in opaque data for the version
const TInt KIndexForFlags = 1;			// index for flags in opaque data

class COpaqueDataParse : public CBase
	{
public:
	static COpaqueDataParse* NewL(const TDesC8& aOpaqueData);
	static COpaqueDataParse* NewLC(const TDesC8& aOpaqueData);
	~COpaqueDataParse();
	TBool OnlyUidsAvail() const;
	TBool IsOpenAgainstSuffix() const;
	TBool IsSetSourceRectSupported() const;

	void EnsureExtnsReadL();
	void EnsureMIMETypesReadL();
	TInt ExtnsCount() const;
	TInt MIMETypesCount() const;
	const TDesC8& Extn (TInt aIndex) const;
	const TDesC8& MIMEType (TInt aIndex) const;
	TInt Version() const;
	TInt Flags() const;
	TUid ImageTypeUid() const;
	TUid ImageSubTypeUid() const;
	TBool CompareUids(COpaqueDataParse* aParse) const;
	TBool CompareUids(TUid aType, TUid aSubType = KNullUid) const;
	static TBool CompareMIMETypes(const TDesC8& aMimeType1, const TDesC8& aMimeType2);
	static TBool CompareFileSuffixL(const TDesC& aFileSuffix1, const TDesC8& aFileSuffix2);
protected:
	COpaqueDataParse(const TDesC8& aOpaqueData);
	void ConstructL();
	void ReadVersion();
	void ReadFlags();
	void ReadUids();
private:
	enum TProcessState
		{
		EVersionProcessed,
		EFlagsProcessed,
		EUidsProcessed,
		EExtnsProcessed,
		EMIMETypesProcessed
		};
	enum TPluginFlags
		{
		EOpenAgainstSuffix		= 0x00000001,
		ESetSourceRectSupported = 0x00000002
		};

	const TDesC8& iOpaqueData;
	TProcessState iState;
	TInt iVersion;
	TInt iFlags;
	TUid iImageTypeUid;
	TUid iImageSubTypeUid;
	TInt iExtnCount;

	TInt iLenOfFlags;				// len of flag field (0 or 2)
	TInt iStartOfUids;				// index for uids in opaque data
	TInt iStartOfNumberOfExtns;		// index in Opaque data for the number of extensions
	
	TInt iStartOfMimeTypes;
	
	RArray<TPtrC8> iArray;
	};

// Utility class used in ImageDisplayResolver.dll
// and ImageDisplay.dll.
// Searches for a match of aClientString in aPluginString
// - aPluginString is normally from CImplementationInformation::DataType()
class CImageDisplayResolverUtils : public CBase
	{
public:
	static TBool Match(const TDesC8& aClientString, const TDesC8& aPluginString, TInt& aBytesMatched);
	};

#include "ImageDisplayResolvrUtils.inl"

#endif  // __IMAGEDISPLAYRESOLVRUTILS_H__