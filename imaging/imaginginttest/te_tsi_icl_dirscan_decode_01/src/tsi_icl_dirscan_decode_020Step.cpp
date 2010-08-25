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

#include "tsi_icl_dirscan_decode_020Step.h"
#include "te_tsi_icl_dirscan_decode_01suitedefs.h"

Ctsi_icl_dirscan_decode_020Step::~Ctsi_icl_dirscan_decode_020Step()
	{
	delete iDirScan;
	iFs.Close();
	RFbsSession::Disconnect();
	delete iActiveListener;
	delete iActiveScheduler;
	}

Ctsi_icl_dirscan_decode_020Step::Ctsi_icl_dirscan_decode_020Step()
	{
	SetTestStepName(Ktsi_icl_dirscan_decode_020Step);
	}

TVerdict Ctsi_icl_dirscan_decode_020Step::doTestStepPreambleL()
	{
	// Get the list of directories/files
	TInt err = iFs.Connect();
	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("There was a problem calling RFs::Connect(): error = %d"), err);
		User::Leave(err);
		}
	
	iDirScan = CDirScan::NewL(iFs);
	iDirScan->SetScanDataL(KImageFileDirectory, KEntryAttDir, ESortByName);
	INFO_PRINTF1(_L("Successfully scanned the directory"));
		
	iActiveScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(iActiveScheduler);
	
	iActiveListener = new (ELeave) CActiveListener();
	
	User::LeaveIfError(RFbsSession::Connect());
	
	SetTestStepResult(EPass);
	return TestStepResult();
	}

// This test tries to decode images using CBufferedImageDecoder by passing image data to the decoder, chunk-by-chunk.
TVerdict Ctsi_icl_dirscan_decode_020Step::doTestStepL()
	{
	if(TestStepResult() == EPass)
		{
		CDir* dir = NULL;
		iDirScan->NextL(dir);
		
		while(dir)
			{
			CleanupStack::PushL(dir);
			
			TInt entryCount = dir->Count();
			for(TInt i = 0; i < entryCount; i++)
				{
				TRAPD(err,doDecodeAndConvertL(dir, i));
				if(err != KErrNone)
					{
					INFO_PRINTF2(_L("doDecodeAndConvertL() failed with %d"), err);
					}
				}
			CleanupStack::PopAndDestroy(dir);
			iDirScan->NextL(dir);
			}
			SetTestStepResult(EPass);	
		}	
	return TestStepResult();
	}

void Ctsi_icl_dirscan_decode_020Step::doDecodeAndConvertL(CDir* aDir, TInt aIndex)
	{
	TEntry entry = (*aDir)[aIndex];
	if(!entry.IsDir())
		{
		TFileName filename;
		filename.Copy(iDirScan->FullPath());
		filename.Append(entry.iName);
		
		INFO_PRINTF2(_L("Attempting to decode %S"), &filename);
									
		const TInt KChunkSize = 32;
		TInt err;

		// Open the file containing the image 
		RFile file;
		TUint flags = EFileShareReadersOnly | EFileStream | EFileRead;
		err = file.Open(iFs, filename, flags);
		if(err != KErrNone)
			{
			INFO_PRINTF1(_L("Failed to open file"));
			User::Leave(err);
			}
		CleanupClosePushL(file);

		// Read the file size
		TInt fileSize = 0;
		err = file.Size(fileSize);
		if(err != KErrNone)
			{
			INFO_PRINTF1(_L("Failed to retrieve file size"));
			User::Leave(err);
			}		

		// Allocate the buffer for the file read in chunks
		HBufC8* buffer = HBufC8::NewLC(KChunkSize);
		TPtr8 bufferPtr(buffer->Des());

		// Read the first chunk from the file
		err = file.Read(bufferPtr, KChunkSize);
		if(err != KErrNone)
			{
			INFO_PRINTF1(_L("File read error encountered"));
			User::Leave(err);
			}

		// Create the decoder passing the File Server session and attempt to open the decoder
		TRAP(err, iDecoder = CBufferedImageDecoder::NewL(iFs));
		if(err != KErrNone)
			{
			INFO_PRINTF1(_L("CBufferedImageDecoder::NewL() failed"));
			User::Leave(err);
			}
		CleanupStack::PushL(iDecoder);

		TRAP(err, iDecoder->OpenL(bufferPtr));
		if (err != KErrNone)
			{
			INFO_PRINTF1(_L("CBufferedImageDecoder::OpenL() failed"));
			User::Leave(err);
			}

		while (!iDecoder->ValidDecoder())
			{
			err = file.Read(bufferPtr, KChunkSize);
			if(err != KErrNone || 0 == bufferPtr.Length())
				{
				INFO_PRINTF1(_L("File read error encountered"));
				User::Leave(err);
				}
			TRAP(err, iDecoder->AppendDataL(bufferPtr));
			if(err != KErrNone)
				{
				INFO_PRINTF1(_L("CBufferedImageDecoder::AppendDataL() failed"));
				User::Leave(err);
				}
			TRAP(err, iDecoder->ContinueOpenL());
			if (err != KErrNone)
				{
				INFO_PRINTF1(_L("CBufferedImageDecoder::ContinueOpenL() failed"));
				User::Leave(err);
				}
			}
		
		// Make sure the header has been fully processed.
		while (!iDecoder->IsImageHeaderProcessingComplete())
			{
			err = file.Read(bufferPtr, KChunkSize);
			if(err != KErrNone ||  0 == bufferPtr.Length())
				{
				INFO_PRINTF1(_L("File read error encountered"));
				User::Leave(err);
				}
			TRAP(err, iDecoder->AppendDataL(bufferPtr));
			if(err != KErrNone)
				{
				INFO_PRINTF1(_L("CBufferedImageDecoder::AppendDataL() failed"));
				User::Leave(err);
				}
			TRAP(err, iDecoder->ContinueProcessingHeaderL());
			if(err != KErrNone)
				{
				INFO_PRINTF1(_L("CBufferedImageDecoder::ContinueProcessingHeaderL() failed"));
				User::Leave(err);
				}	
			}
		
		const TFrameInfo frameInfo = iDecoder->FrameInfo(0);

		// Create the destination bitmap
		CFbsBitmap* bitmap = NULL; 
		TRAP(err, bitmap = new (ELeave) CFbsBitmap());
		if(err != KErrNone)
			{
			INFO_PRINTF1(_L("Failed to create CFbsBitmap object"));
			User::Leave(err);
			}
		CleanupStack::PushL(iDestBitmap);

		err = bitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode);
		if(err != KErrNone)
			{
			INFO_PRINTF1(_L("Failed to create decoder. On hardware, this could be due to lack of sufficient memory"));
			User::Leave(err);
			}
		// Convert the first frame using Convert() function and use ContinueConvert() function 
		//for the remaining image data.
		iActiveListener->InitialiseActiveListener();
		iDecoder->Convert(&iActiveListener->iStatus, *bitmap, 0);
		CActiveScheduler::Start();
		if((err = iActiveListener->iStatus.Int()) != KErrNone && err != KErrUnderflow)
			{			
			INFO_PRINTF1(_L("convert failed!"));
			User::Leave(err);
			}
					
		// Feed the image data chunk-by-chunk to decoder while the error code is KErrUnderflow
		// until it becomes KErrNone (or any other system error code).
		while ((err = iActiveListener->iStatus.Int()) == KErrUnderflow)
			{
			if ((err != KErrNone) && (err != KErrUnderflow))
				{
				User::Leave(err);
				}

			err = file.Read(bufferPtr, KChunkSize);
			if(err != KErrNone)
				{
				INFO_PRINTF1(_L("File read error encountered"));
				User::Leave(err);
				}
			TRAP(err, iDecoder->AppendDataL(bufferPtr));
			if(err != KErrNone)
				{
				INFO_PRINTF1(_L("CBufferedImageDecoder::AppendDataL() failed"));
				User::Leave(err);
				}
			iActiveListener->InitialiseActiveListener();
			iDecoder->ContinueConvert(&iActiveListener->iStatus);
			CActiveScheduler::Start();
			}
		INFO_PRINTF2(_L("%S decoded successfully!"), &filename);
		CleanupStack::PopAndDestroy(4); // file, buffer, decoder and bitmap 
		}
	}

TVerdict Ctsi_icl_dirscan_decode_020Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 */
	{
	return TestStepResult();
	}
