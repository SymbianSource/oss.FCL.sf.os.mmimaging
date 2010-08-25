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

#include "tsi_icl_dirscan_decode_010Step.h"
#include "te_tsi_icl_dirscan_decode_01suitedefs.h"

Ctsi_icl_dirscan_decode_010Step::~Ctsi_icl_dirscan_decode_010Step()
	{
	delete iDestBitmap;
	iDestBitmap = NULL;						
	delete iDecoder;
	iDecoder = NULL;
	delete iDirScan;
	iFs.Close();
	RFbsSession::Disconnect();
	delete iActiveListener;
	delete iActiveScheduler;
	}

Ctsi_icl_dirscan_decode_010Step::Ctsi_icl_dirscan_decode_010Step()
	{
	SetTestStepName(Ktsi_icl_dirscan_decode_010Step);
	}

TVerdict Ctsi_icl_dirscan_decode_010Step::doTestStepPreambleL()
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


TVerdict Ctsi_icl_dirscan_decode_010Step::doTestStepL()
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
				TEntry entry = (*dir)[i];
				if(!entry.IsDir())
					{
					TFileName filename;
					filename.Copy(iDirScan->FullPath());
					filename.Append(entry.iName);
								
					INFO_PRINTF2(_L("Attempting to decode %S"), &filename);
					
					//Cleanup
					delete iDecoder; iDecoder = NULL;
					delete iDestBitmap; iDestBitmap = NULL;
					
					TRAPD(err, iDecoder = CImageDecoder::FileNewL(iFs, filename));		
					if(err != KErrNone)
						{
						INFO_PRINTF2(_L("\tFailed to create decoder with %d"), err);
						continue;
						}
					INFO_PRINTF1(_L("\tDecoder created successfully"));
					
					iFrameInfo = iDecoder->FrameInfo(0);

					TRAP(err, StartNewConvertL(entry, EDirScanNormalDecode));
					if(err != KErrNone)						
						{
						INFO_PRINTF2(_L("\tStartNewConvertL() left with %d"), err);
						}
					
					// For JPEG images, attempt to try some extension operations
					iDecoder->ImageType(0, iImageType, iImageSubType);
					if(iImageType == KImageTypeJPGUid)
						{
						INFO_PRINTF1(_L("\tAttempt to crop image"));
						TRect requiredClippingRect(TSize(iFrameInfo.iOverallSizeInPixels.iWidth/4, iFrameInfo.iOverallSizeInPixels.iHeight/4));
						TRAP(err,iDecoder->SetClippingRectL(&requiredClippingRect));
						if(err == KErrNone)
							{			
							TRAP(err, StartNewConvertL(entry, EDirScanCropped));
							if(err == KErrNone)
								{
								INFO_PRINTF1(_L("\tAttempt to crop image succeeded"));
								}
							else
								{
								INFO_PRINTF2(_L("\tStartNewConvertL() left with %d"), err);
								}
							}
						else
							{
							// We know that SetClippingRectL() IS supported, so has failed for unknown reason
							INFO_PRINTF2(_L("\tSetClippingRectL() unexpectedly left with %d"), err);
							iInconclusiveCount++;
							// Continue test anyway
							}
						
						TRAP_IGNORE(iDecoder->SetClippingRectL(NULL)); // reset to none
						
						INFO_PRINTF1(_L("\tAttempt to rotate image 90 degrees"));
						TImageConvOperation* operationExt = NULL;
			    		TRAP(err, operationExt = iDecoder->OperationL());
						if(err == KErrNone)
							{
							TRAP(err, operationExt->AddOperationL(TImageConvOperation::ERotation90DegreesClockwise));
							if(err == KErrNone)
								{
								TRAP(err, StartNewConvertL(entry, EDirScanRotated90Degrees));
								if(err == KErrNone)
									{
									INFO_PRINTF1(_L("\tAttempt to rotate image 90 degrees succeeded"));
									}
								else
									{
									INFO_PRINTF2(_L("\tStartNewConvertL() left with %d"), err);
									}
								}
							else
								{
								// We know that AddOperationL() IS supported, so has failed for unknown reason
								INFO_PRINTF2(_L("\tAddOperationL(ERotation90DegreesClockwise) unexpectedly left with %d"), err);
								iInconclusiveCount++;
								// Continue test anyway
								}						
							}
						else
							{
							// We know that OperationL() IS supported, so has failed for unknown reason
							INFO_PRINTF2(_L("\tOperationL() unexpectedly left with %d"), err);
							iInconclusiveCount++;
							// Continue test anyway
							}
						
						operationExt->ClearOperationStack(); // Ready for next operation
						INFO_PRINTF1(_L("\tAttempt to mirror over the horizontal axis"));
						TRAP(err, operationExt->AddOperationL(TImageConvOperation::EMirrorHorizontalAxis));
						if(err == KErrNone)
							{
							TRAP(err, StartNewConvertL(entry, EDirScanMirrorHorizontalAxis));
							if(err == KErrNone)
								{
								INFO_PRINTF1(_L("\tAttempt to mirror over the horizontal axis succeeded"));
								}
							else
								{
								INFO_PRINTF2(_L("\tStartNewConvertL() left with %d"), err);
								}
							}
						else
							{
							// We know that AddOperationL() IS supported, so has failed for unknown reason
							INFO_PRINTF2(_L("\tAddOperationL(EMirrorHorizontalAxis) unexpectedly left with %d"), err);
							iInconclusiveCount++;
							// Continue test anyway
							}
						operationExt->ClearOperationStack(); // Ready for next operation
						
						TImageConvScaler* scalerExt = NULL;
						INFO_PRINTF1(_L("\tAttempt to scale image to 1/4 size"));
						TRAP(err, scalerExt = iDecoder->ScalerL());
						if(err == KErrNone)
							{
							TRAP(err, scalerExt->SetScalingL(-3, TImageConvScaler::EMaximumQuality)); // -3 == 1/4
							if(err == KErrNone)
								{
								TRAP(err, StartNewConvertL(entry, EDirScanScaleToQuarterSize));
								if(err == KErrNone)
									{
									INFO_PRINTF1(_L("\tAttempt to scale image to 1/4 size succeeded"));
									}
								else
									{
									INFO_PRINTF2(_L("\tStartNewConvertL() left with %d"), err);
									}
								}
							else
								{
								// We know that SetScalingL() IS supported, so has failed for unknown reason
								INFO_PRINTF2(_L("\tSetScalingL() unexpectedly left with %d"), err);
								iInconclusiveCount++;
								// Continue test anyway
								}
							}
						else
							{
							// We know that ScalerL() IS supported, so has failed for unknown reason
							INFO_PRINTF2(_L("\tScalerL() unexpectedly left with %d"), err);
							iInconclusiveCount++;
							// Continue test anyway
							}
						
						INFO_PRINTF1(_L("\tAttempt to crop, rotate 270 degrees and scale by 1/2"));
						
						TRAP(err,iDecoder->SetClippingRectL(&requiredClippingRect));
						if(err != KErrNone)
							{
							// We know that SetClippingRectL() IS supported, so has failed for unknown reason
							INFO_PRINTF2(_L("\tSetClippingRectL() unexpectedly left with %d"), err);
							iInconclusiveCount++;
							// Continue test anyway
							}
						
						// Rotate
						TRAP(err, operationExt = iDecoder->OperationL());
						if(err == KErrNone)
							{
							TRAP(err, operationExt->AddOperationL(TImageConvOperation::ERotation270DegreesClockwise));
							if(err != KErrNone)								
								{
								// We know that AddOperationL() IS supported, so has failed for unknown reason
								INFO_PRINTF2(_L("\tAddOperationL(ERotation270DegreesClockwise) unexpectedly left with %d"), err);
								iInconclusiveCount++;
								// Continue test anyway
								}
							}
						else
							{
							// We know that OperationL() IS supported, so has failed for unknown reason
							INFO_PRINTF2(_L("\tOperationL() unexpectedly left with %d"), err);
							iInconclusiveCount++;
							// Continue test anyway
							}
						
						// Scale
						TRAP(err, scalerExt = iDecoder->ScalerL());
						if(err == KErrNone)
							{
							TRAP(err, scalerExt->SetScalingL(-2, TImageConvScaler::EMaximumQuality)); // -2 == 1/2
							if(err != KErrNone)
								{
								// We know that SetScalingL() IS supported, so has failed for unknown reason
								INFO_PRINTF2(_L("\tSetScalingL() unexpectedly left with %d"), err);
								iInconclusiveCount++;
								// Continue test anyway
								}
							}
						else
							{
							// We know that ScalerL() IS supported, so has failed for unknown reason
							INFO_PRINTF2(_L("\tScalerL() unexpectedly left with %d"), err);
							iInconclusiveCount++;
							// Continue test anyway
							}
						
						TRAP(err, StartNewConvertL(entry, EDirScanCroppedRotated270DegreesScaleToHalfSize));
						if(err == KErrNone)
							{
							INFO_PRINTF1(_L("\tAttempt to crop, rotate 270 degrees and scale by 1/2 succeeded"));
							}
						else
							{
							INFO_PRINTF2(_L("\tStartNewConvertL() left with %d"), err);
							}
						}
					}
				}
			CleanupStack::PopAndDestroy(dir);
			
			iDirScan->NextL(dir);
			}
		
		if(iInconclusiveCount == 0)
			{
			SetTestStepResult(EPass);	
			}
		else
			{
			INFO_PRINTF1(_L("\tAn unexpected leave/error has occurred.  Investigation required."));
			SetTestStepResult(EInconclusive);
			}
		}
	return TestStepResult();
	}

void Ctsi_icl_dirscan_decode_010Step::StartNewConvertL(const TEntry& aEntry, TDirScanOperations aOperation)
	{
	TSize requiredDestinationSize(iFrameInfo.iOverallSizeInPixels);
	if(iImageType == KImageTypeJPGUid)
		{
		// Reset size if extension is supported
		TInt err = iDecoder->GetDestinationSize(requiredDestinationSize, 0);
		if(err != KErrNone)
			{
			INFO_PRINTF2(_L("\tGetDestinationSize() unexpectedly returned %d"), err);
			User::Leave(err);
			}
		}	
	
	/*
	CFbsBitmap::Resize() clearly has issues when attempting to resize very large
	bitmaps.  Thus, to work around that here we simply delete and new a bitmap.
	*/
	delete iDestBitmap; iDestBitmap = NULL;
	iDestBitmap = new (ELeave) CFbsBitmap();
	TInt err = iDestBitmap->Create(requiredDestinationSize, iFrameInfo.iFrameDisplayMode);
	if(err != KErrNone)
		{
		// -4's are likely on hardware due to limited memory conditions
		INFO_PRINTF2(_L("\tCFbsBitmap->Create() returned %d.  Not an issue as unrelated to ICL."), err);
		User::Leave(err);
		}
	
	iActiveListener->InitialiseActiveListener();
	iDecoder->Convert(&iActiveListener->iStatus, *iDestBitmap, 0);
	CActiveScheduler::Start();
	if(iActiveListener->iStatus.Int() != KErrNone)
		{
		INFO_PRINTF2(_L("\tCImageDecoder::Convert() failed with %d"), iActiveListener->iStatus.Int());
		}

#ifdef GENERATE_REFERENCE_FILES
	TFileName dirName;
	dirName.Copy(KImageFileOutputDirectory());
	dirName.Append(KPathDelimiter);
	if(!BaflUtils::FolderExists(iFs, dirName))
		{
		err = iFs.MkDir(dirName);
		if(err != KErrNone && err != KErrAlreadyExists)
			{
			INFO_PRINTF2(_L("\tThere was a problem creating the output directory, error = %d"), err);
			User::Leave(err);
			}
		}
	
	TFileName saveFilename;
	saveFilename.Copy(KImageFileOutputDirectory());
	saveFilename.Append(iDirScan->AbbreviatedPath());					
	saveFilename.Append(aEntry.iName);
	TInt locate = saveFilename.LocateReverseF('.');
	saveFilename.Replace(locate, KMbmExtension().Length(), KMbmExtension);
	
	switch(aOperation)
		{
		case EDirScanNormalDecode:
			saveFilename.Insert(locate, _L("_normal_decode"));
			break;
		case EDirScanCropped:
			saveFilename.Insert(locate, _L("_cropped"));
			break;
		case EDirScanRotated90Degrees:
			saveFilename.Insert(locate, _L("_rotated_90_degrees"));
			break;
		case EDirScanMirrorHorizontalAxis:
			saveFilename.Insert(locate, _L("_mirror_horizontal_axis"));
			break;
		case EDirScanScaleToQuarterSize:
			saveFilename.Insert(locate, _L("_scale_to_quarter_size"));
			break;
		case EDirScanCroppedRotated270DegreesScaleToHalfSize:
			saveFilename.Insert(locate, _L("_cropped_rotated_270_degrees_scale_to_half_size"));
			break;
		default:
			INFO_PRINTF1(_L("Unknown operation"));
			User::Leave(KErrNotFound);
		}
	
	err = iDestBitmap->Save(saveFilename);
	if(err != KErrNone)
		{
		INFO_PRINTF2(_L("\tFailed to save output image (with %d)"), err);
		}
	else
		{
		INFO_PRINTF2(_L("\tSaving decoded image as %S"), &saveFilename);
		}
#else
	// To keep compiler quiet on unused arguments
	(void)aEntry; // do nothing
	(void)aOperation; // do nothing

#endif // GENERATE_REFERENCE_FILES
	}

TVerdict Ctsi_icl_dirscan_decode_010Step::doTestStepPostambleL()
/**
 * @return - TVerdict code
 */
	{
	return TestStepResult();
	}
