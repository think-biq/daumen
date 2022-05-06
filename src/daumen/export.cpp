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
	
	ExportData->RawFileData = (uint8_t*)malloc(ExportData->FileSize);
	if (nullptr == ExportData->RawFileData) {
		return 0;
	}

	char* ExportRawFileDataHead = (char*)ExportData->RawFileData;
	char* FileHeaderDataHead = CharPtr(Daumen->FileHeader);
	memcpy(ExportRawFileDataHead, FileHeaderDataHead, sizeof(Daumen->FileHeader));
	ExportRawFileDataHead += sizeof(Daumen->FileHeader);

	size_t InfoHeaderSize = (size_t)Daumen->InfoHeaderType;
	char* InfoHeaderDataHead = nullptr;
	switch (Daumen->InfoHeaderType) {
	case (InfoHeaderType::IHT_BITMAPCOREHEADER):
		InfoHeaderDataHead = CharPtr(Daumen->InfoHeader.BitmapCore);
		break;
	case (InfoHeaderType::IHT_BITMAPINFOHEADER):
		InfoHeaderDataHead = CharPtr(Daumen->InfoHeader.BitmapInfo);
		break;
	case (InfoHeaderType::IHT_BITMAPV4HEADER):
		InfoHeaderDataHead = CharPtr(Daumen->InfoHeader.BitmapV4);
		break;
	case (InfoHeaderType::IHT_BITMAPV5HEADER):
		InfoHeaderDataHead = CharPtr(Daumen->InfoHeader.BitmapV5);
		break;
	default:
	case (InfoHeaderType::IHT_INVALID):
		assert(false && "Unknown header type!");
		return 0;
	}
	memcpy(ExportRawFileDataHead, InfoHeaderDataHead, InfoHeaderSize);
	ExportRawFileDataHead += InfoHeaderSize;

	memcpy(ExportRawFileDataHead, Daumen->Bitmap.bmBits, ExportData->ImageDataSize);

	return 1;
}

int DiscardDaumenExport(DaumenExportData* ExportData) {
	if (nullptr == ExportData) {
		return 0;
	}

	free(ExportData->RawFileData);
	return 1;
}