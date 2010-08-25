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

#ifndef TE_TSI_ICL_DIRSCAN_DECODE_01_SUITEDEFS_H
#define TE_TSI_ICL_DIRSCAN_DECODE_01_SUITEDEFS_H

#include <f32file.h>
#include <imageconversion.h>
#include <bautils.h>
#include <icl/imageconversionextension.h>
#include <fbs.h>

// For test step panics
_LIT(KTe_tsi_icl_dirscan_decode_01SuitePanic,"te_tsi_icl_dirscan_decode_01suite");

_LIT(KImageFileDirectory, "c:\\te_tsi_icl_dirscan_decode_01\\*");
_LIT(KImageFileOutputDirectory, "c:\\te_tsi_icl_dirscan_decode_01_output");
_LIT(KMbmExtension, ".mbm");

#endif // TE_TSI_ICL_DIRSCAN_DECODE_01_SUITEDEFS_H
