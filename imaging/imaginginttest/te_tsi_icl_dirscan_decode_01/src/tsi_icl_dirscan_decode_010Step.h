/*
* Copyright (c) 2002 - 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/

#ifndef TSI_ICL_DIRSCAN_DECODE_010_STEP_H
#define TSI_ICL_DIRSCAN_DECODE_010_STEP_H

#include <test/testexecutestepbase.h>
#include "te_tsi_icl_dirscan_decode_01suitestepbase.h"
#include "te_tsi_icl_dirscan_decode_01suitedefs.h"

class Ctsi_icl_dirscan_decode_010Step : public CTe_tsi_icl_dirscan_decode_01SuiteStepBase
	{
public:
	// Used to identify the output reference images
	enum TDirScanOperations
		{
		EDirScanNormalDecode, // = 0
		EDirScanCropped,
		EDirScanRotated90Degrees,
		EDirScanMirrorHorizontalAxis,
		EDirScanScaleToQuarterSize,
		EDirScanCroppedRotated270DegreesScaleToHalfSize
		};

public:
	Ctsi_icl_dirscan_decode_010Step();
	~Ctsi_icl_dirscan_decode_010Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	void StartNewConvertL(const TEntry& aEntry, TDirScanOperations aOperation);

private:
	RFs iFs;
	CDirScan* iDirScan;
	CActiveScheduler* iActiveScheduler;
	CActiveListener* iActiveListener;
	CImageDecoder* iDecoder;
	CFbsBitmap* iDestBitmap;
	TFrameInfo iFrameInfo;
	// If type is JPEG, then attempt extension operations
	TUid iImageType;
	TUid iImageSubType;
	// For unexpected leaves/errors: if > 0 then the test will report as inconclusive
	TInt iInconclusiveCount;
	};

_LIT(Ktsi_icl_dirscan_decode_010Step,"tsi_icl_dirscan_decode_010Step");

#endif // TSI_ICL_DIRSCAN_DECODE_010_STEP_H
