/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TSI_ICL_DIRSCAN_DECODE_020_STEP_H
#define TSI_ICL_DIRSCAN_DECODE_020_STEP_H

#include <test/testexecutestepbase.h>
#include "te_tsi_icl_dirscan_decode_01suitestepbase.h"
#include "te_tsi_icl_dirscan_decode_01suitedefs.h"

class Ctsi_icl_dirscan_decode_020Step : public CTe_tsi_icl_dirscan_decode_01SuiteStepBase
	{

public:
	Ctsi_icl_dirscan_decode_020Step();
	~Ctsi_icl_dirscan_decode_020Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	void doDecodeAndConvertL(CDir* aDir, TInt aIndex);

private:
	RFs iFs;
	CDirScan* iDirScan;
	CActiveScheduler* iActiveScheduler;
	CActiveListener* iActiveListener;
	CFbsBitmap* iDestBitmap;
	TFrameInfo iFrameInfo;
	CBufferedImageDecoder* iDecoder;
	};

_LIT(Ktsi_icl_dirscan_decode_020Step,"tsi_icl_dirscan_decode_020Step");

#endif // TSI_ICL_DIRSCAN_DECODE_020_STEP_H
