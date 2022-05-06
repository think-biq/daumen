// 2022 - ∞ (c) blurryroots innovation qanat OÜ. All rights reserved.

#pragma once

#ifndef _H_DAUMEN_EXPORT_H_
#define _H_DAUMEN_EXPORT_H_

#include <daumen/core.h>

struct DaumenExportData {
	uint64_t FileSize;
	uint64_t HeaderSize;
	uint64_t ImageDataSize;
	uint8_t* RawFileData;
};

int PerpareDaumenForExternalUse(DaumenExportData* ExportData, const DaumenBitmap* Daumen);

int DiscardDaumenExport(DaumenExportData* ExportData);

#endif