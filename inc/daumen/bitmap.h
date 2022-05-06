// 2022 - ∞ (c) blurryroots innovation qanat OÜ. All rights reserved.

#pragma once

#ifndef _H_DAUMEN_BITMAP_H_
#define _H_DAUMEN_BITMAP_H_

#include <daumen/core.h>

int ReadDaumenBitmap(DaumenBitmap* Daumen, const char* FilePath);

int WriteDaumenBitmap(const char* FilePath, const DaumenBitmap* Thumbnail);

#endif