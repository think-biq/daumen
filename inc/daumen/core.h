// 2022 - ∞ (c) blurryroots innovation qanat OÜ. All rights reserved.

#pragma once

#ifndef _H_DAUMEN_CORE_H_
#define _H_DAUMEN_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#ifdef __cplusplus
}
#endif

struct DaumenBitmap;

DaumenBitmap* NewDaumen();

void FreeDaumen(DaumenBitmap* Daumen);

long GetDaumenBitmapWidth(const DaumenBitmap* Daumen);

long GetDaumenBitmapHeight(const DaumenBitmap* Daumen);

long GetDaumenBitmapFileSize(const DaumenBitmap* Daumen);

long GetDaumenBitmapHeaderSize(const DaumenBitmap* Daumen);

long GetDaumenBitmapImageDataSize(const DaumenBitmap* Daumen);

int AreDaumenEqual(const DaumenBitmap* A, const DaumenBitmap* B);

#endif