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

#include "tsi_icl_excludedfiles_010step.h"
#include "te_tsi_icl_excludedfiles_01suitedefs.h"

Ctsi_icl_excludedfiles_010Step::~Ctsi_icl_excludedfiles_010Step()
/**
 * Destructor
 */
	{
	delete iActiveScheduler;
	iFs.Close();
	}

Ctsi_icl_excludedfiles_010Step::Ctsi_icl_excludedfiles_010Step()
/**
 * Constructor
 */
	{
	SetTestStepName(Ktsi_icl_excludedfiles_010step);
	}

TVerdict Ctsi_icl_excludedfiles_010Step::doTestStepPreambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	iActiveScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	
	User::LeaveIfError(iFs.Connect());
	SetTestStepResult(EPass);
	return TestStepResult();
	}


TVerdict Ctsi_icl_excludedfiles_010Step::doTestStepL()
/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
	{
	if (TestStepResult()==EPass)
		{
		INFO_PRINTF1(_L("*** PDEF122601: Invalid next-ifd offset inside TIFF puts decoder into forever loop ***"));
		TPtrC sourceFilename;
		TInt decoderUid;
		if(!GetStringFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteSourceFile, sourceFilename) ||
			!GetHexFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteDecoderUid, decoderUid))
			{
			INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
			User::Leave(KErrNotFound);
			}
	
		/* The test file, KInfiniteTiff can be recognised as a virus by some scanners
		Thus, to avoid raising 'false positive' alerts on licencees, we exclude the
		file from the codeline given to licencees.
		
		Because the test passes if the factory function FileNewL() fails with KErrNotFound,
		this part of the test checks whether the file actually exists before testing.
		*/
		RFile infiniteTif;
		TInt err = KErrNone;
		err = infiniteTif.Open(iFs, sourceFilename, EFileShareReadersOrWriters);
		
		if (err != KErrNone)
			{
			// File does not exist, so this can not be an internal to Multimedia test run.
			INFO_PRINTF1(_L("c:\\te_tsi_icl_excludedfiles_01\\infinite.tifi file is not present."));
			INFO_PRINTF1(_L("This is expected behaviour if this test is not run by the Symbian Multimedia team."));
			INFO_PRINTF2(_L("Error code on attempt to open file is %d."), err);
			SetTestStepResult(EInconclusive);
			}
		else
			{
			// Continue with regular test.
			
			infiniteTif.Close();
			
			// Before the fix for this defect this function would leave with -4 (KErrNoMemory),
			// now it should leave with -1 (KErrNotFound) *NOT KErrCorrupt*.
			CImageDecoder* decoder = NULL;
			TRAP(err, decoder = CImageDecoder::FileNewL(iFs, sourceFilename));
			delete decoder;
		
			if (err != KErrNotFound)
				{
				INFO_PRINTF2(_L("Expected failure with -1, got %d"), err);
				SetTestStepResult(EFail);
				}
			else 
				{
				SetTestStepResult(EPass);
				}	
			}
		}
	
	return TestStepResult();
	}


/**
 * @return - TVerdict code
 * Override of base class virtual
 */
TVerdict Ctsi_icl_excludedfiles_010Step::doTestStepPostambleL()
	{
	return TestStepResult();
	}

/**
 * Constructor
 */
CTsi_icl_excludedfiles_011Step::CTsi_icl_excludedfiles_011Step()
	{
	SetTestStepName(Ktsi_icl_excludedfiles_011step);
	}

/**
 * Destructor
 */
CTsi_icl_excludedfiles_011Step::~CTsi_icl_excludedfiles_011Step()
	{
	delete iActiveScheduler;
	iFs.Close();
	}

/**
 * @return - TVerdict code
 * Override of base class virtual
 */
TVerdict CTsi_icl_excludedfiles_011Step::doTestStepPreambleL()
	{
	iActiveScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	
	User::LeaveIfError(iFs.Connect());
	User::LeaveIfError(RFbsSession::Connect());
	SetTestStepResult(EPass);
	return TestStepResult();
	}

/**
 * @return - TVerdict code
 * Override of base class pure virtual
 * Our implementation only gets called if the base class doTestStepPreambleL() did
 * not leave. That being the case, the current test result value will be EPass.
 */
TVerdict CTsi_icl_excludedfiles_011Step::doTestStepL()
	{
	if (TestStepResult()==EPass)
		{
		CImageDecoder* imageDecoder = NULL;
		TInt error = KErrNone;
		
		TPtrC sourceFilename, destFile;
		TInt frameNumber;
		TInt redFactor;
		if(!GetStringFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteSourceFile, sourceFilename) ||
			!GetHexFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteFrameNumber, frameNumber)||
			!GetHexFromConfig(ConfigSection(),KTe_tsi_icl_cod_5SuiteRedFactor, redFactor)||
			!GetStringFromConfig(ConfigSection(), KTe_tsi_icl_cod_5SuiteDestFile, destFile))
			{
			INFO_PRINTF1(_L("Unable to obtain data from .ini file."));
			User::Leave(KErrNotFound);
			}
		CFbsBitmap* srcBitmap = new(ELeave) CFbsBitmap;
		CleanupStack::PushL(srcBitmap);
		TRAP(error, imageDecoder = DecodeFileL(iFs, sourceFilename, *srcBitmap, frameNumber, redFactor));
		delete imageDecoder; imageDecoder = NULL;
			
		if(error != KErrNone)
			{
			INFO_PRINTF2(_L("decoding error: %d"), error);
			CleanupStack::PopAndDestroy(srcBitmap);
			SetTestStepResult(EFail);
			}	
		
		CFbsBitmap* destBitmap = new(ELeave)CFbsBitmap;
		error = destBitmap->Load(destFile);
		if(error != KErrNone)
			{
			INFO_PRINTF2(_L("Error in loading reference bitmap: %d"),error);
			CleanupStack::PopAndDestroy(srcBitmap);
			SetTestStepResult(EFail);
			}	
		Compare(*srcBitmap, *destBitmap);
		CleanupStack::PopAndDestroy(srcBitmap);
		}		
	return TestStepResult();
	}

CImageDecoder* CTsi_icl_excludedfiles_011Step::DecodeFileL(RFs& aFs, const TFileName& aFileName, CFbsBitmap& aBitmap, const TInt aFrameNumber, const TInt aReductionFactor)
	{
	CImageDecoder* decoder = CImageDecoder::FileNewL(aFs, aFileName);
	CleanupStack::PushL(decoder);
	
	const TFrameInfo& frameInfo = decoder->FrameInfo(aFrameNumber);
	TSize actualSize(frameInfo.iOverallSizeInPixels);
	TSize reducedSize(0, 0); //Size after applying reduction factor
	
	//in case of wmf file, to use same reference file across platforms
	TParsePtrC parse (aFileName);
	TPtrC extension = parse.Ext();
	if(extension.Compare(_L(".wmf")) == 0)
		{
		//approximate twips/pixel = 12. To give margin take 20, this corresponds to 5% of twips
		actualSize.SetSize((frameInfo.iFrameSizeInTwips.iWidth*5)/100,
				  	  		(frameInfo.iFrameSizeInTwips.iHeight*5)/100);
		}		
	User::LeaveIfError(decoder->ReducedSize(actualSize, aReductionFactor, reducedSize));
	User::LeaveIfError(aBitmap.Create(reducedSize, frameInfo.iFrameDisplayMode));

	CActiveListener* listener = new (ELeave) CActiveListener;
	CleanupStack::PushL(listener);
	
	// Convert a frame
	listener->InitialiseActiveListener();
	decoder->Convert(&listener->iStatus, aBitmap, aFrameNumber);
	CActiveScheduler::Start();

	User::LeaveIfError(listener->iStatus.Int());

	CleanupStack::PopAndDestroy(listener);
	CleanupStack::Pop(decoder);
	return decoder;			
	}
	
void CTsi_icl_excludedfiles_011Step::Compare(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2)
	{
	if(aBitmap1.SizeInPixels() != aBitmap2.SizeInPixels())
		{
		SetTestStepResult(EFail);
		return;
		}

	const TSize size = aBitmap1.SizeInPixels();
	
	for (TInt y = 0; y < size.iHeight; y++)
		{
		for (TInt x = 0; x < size.iWidth; x++)
			{
			TPoint pt(x,y);
			TRgb color1,color2;
			aBitmap1.GetPixel(color1,pt);
			aBitmap2.GetPixel(color2,pt);
				
			if (color1 != color2)
				{
				SetTestStepResult(EFail);
				return;
				}
			}
		} // end of comparison for-loop		
	}

/**
 * @return - TVerdict code
 * Override of base class virtual
 */
TVerdict CTsi_icl_excludedfiles_011Step::doTestStepPostambleL()
	{
	iFs.Close();
	RFbsSession::Disconnect();
	return TestStepResult();
	}
