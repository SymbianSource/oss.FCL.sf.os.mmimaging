/*
* Copyright (c) 2005 - 2008 Nokia Corporation and/or its subsidiary(-ies).
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


#include "te_tsi_icl_dirscan_decode_01suitestepbase.h"
#include "te_tsi_icl_dirscan_decode_01suitedefs.h"

// Device driver constants

TVerdict CTe_tsi_icl_dirscan_decode_01SuiteStepBase::doTestStepPreambleL()
/**
 * @return - TVerdict
 */
	{
	SetTestStepResult(EPass);
	return TestStepResult();
	}

TVerdict CTe_tsi_icl_dirscan_decode_01SuiteStepBase::doTestStepPostambleL()
/**
 * @return - TVerdict
 */
	{
	return TestStepResult();
	}

CTe_tsi_icl_dirscan_decode_01SuiteStepBase::~CTe_tsi_icl_dirscan_decode_01SuiteStepBase()
	{
	}

CTe_tsi_icl_dirscan_decode_01SuiteStepBase::CTe_tsi_icl_dirscan_decode_01SuiteStepBase()
	{
	}
