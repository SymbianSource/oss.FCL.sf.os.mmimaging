/*
* Copyright (c) 2004 - 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef TE_TSI_ICL_DIRSCAN_DECODE_01_STEP_BASE
#define TE_TSI_ICL_DIRSCAN_DECODE_01_STEP_BASE

#include <test/testexecutestepbase.h>

class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {};
	};

class CTe_tsi_icl_dirscan_decode_01SuiteStepBase : public CTestStep
	{
public:
	virtual ~CTe_tsi_icl_dirscan_decode_01SuiteStepBase();
	CTe_tsi_icl_dirscan_decode_01SuiteStepBase();
	virtual TVerdict doTestStepPreambleL(); 
	virtual TVerdict doTestStepPostambleL();

protected:
	HBufC8*		iReadData;
	HBufC8*		iWriteData;
	};

#endif // TE_TSI_ICL_DIRSCAN_DECODE_01_STEP_BASE
