// 2022 - ∞ (c) blurryroots innovation qanat OÜ. All rights reserved.

#include <daumen/export.h>

#include "windows-common-internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

int PerpareDaumenForExternalUse(DaumenExportData* ExportData, const DaumenBitmap* Daumen) {
	if (nullptr == ExportData || nullptr == Daumen) {
		return 0;
	}

	ZeroStructureRaw(ExportData);

	ExportData->FileSize = GetDaumenBitmapFileSize(Daumen);
	ExportData->HeaderSize = GetDaumenBitmapHeaderSize(Daumen);

	ExportData->ImageDataSize = GetDaumenBitmapImageDataSize(Daumen);
	if (0 == ExportData->ImageDataSize) {
		return 0;
	}
	
	ExportData->RawFileData = (uint8_t*)malloc(ExportData->ImageDataSize);
	if (nullptr == ExportData->RawFileData) {
		return 0;
	}
	memcpy(ExportData->RawFileData, Daumen->Bitmap.bmBits, ExportData->ImageDataSize);

	return 1;
}