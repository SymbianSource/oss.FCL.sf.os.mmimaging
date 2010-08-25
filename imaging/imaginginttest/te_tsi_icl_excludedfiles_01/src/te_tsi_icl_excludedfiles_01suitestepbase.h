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


#ifndef TE_TSI_ICL_EXCLUDEDFILES_01_STEP_BASE
#define TE_TSI_ICL_EXCLUDEDFILES_01_STEP_BASE
#include <test/testexecuteserverbase.h>
#include <icl/icl_uids.hrh>
#include <imageconversion.h>
#include <f32file.h>

class CTe_tsi_icl_excludedfiles_01SuiteStepBase : public CTestStep
	{
public:
	virtual ~CTe_tsi_icl_excludedfiles_01SuiteStepBase();
	CTe_tsi_icl_excludedfiles_01SuiteStepBase();
	virtual TVerdict doTestStepPreambleL(); 
	virtual TVerdict doTestStepPostambleL();

protected:
	HBufC8*		iReadData;
	HBufC8*		iWriteData;
	};

#endif // TE_TSI_ICL_EXCLUDEDFILES_01_STEP_BASE
