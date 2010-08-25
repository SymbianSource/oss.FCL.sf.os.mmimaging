/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TSI_ICL_EXCLUDEDFILES_010_STEP_H
#define TSI_ICL_EXCLUDEDFILES_010_STEP_H

#include <test/testexecuteserverbase.h>
#include "te_tsi_icl_excludedfiles_01suitestepbase.h"

/**
 * Listener that stop the ActiveScheduler when signalled
 *
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {};
	};

class Ctsi_icl_excludedfiles_010Step : public CTe_tsi_icl_excludedfiles_01SuiteStepBase
	{
public:
	Ctsi_icl_excludedfiles_010Step();
	~Ctsi_icl_excludedfiles_010Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

private:
	CActiveScheduler* iActiveScheduler;
	RFs iFs;
	};

_LIT(Ktsi_icl_excludedfiles_010step,"tsi_icl_excludedfiles_010Step");

class CTsi_icl_excludedfiles_011Step : public CTe_tsi_icl_excludedfiles_01SuiteStepBase
	{
public:
	CTsi_icl_excludedfiles_011Step();
	~CTsi_icl_excludedfiles_011Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	CImageDecoder* DecodeFileL(RFs& aFs, const TFileName& aFileName, CFbsBitmap& aBitmapconst, const TInt aFrameNumber = 0, const TInt aReductionFactor = 0);
	void Compare(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2);

private:
	CActiveScheduler* iActiveScheduler;
	RFs iFs;
	};

_LIT(Ktsi_icl_excludedfiles_011step,"tsi_icl_excludedfiles_011Step");

#endif // TSI_ICL_EXCLUDEDFILES_010_STEP_H
