// 2022 - ∞ (c) blurryroots innovation qanat OÜ. All rights reserved.

#pragma once

#ifndef _H_DAUMEN_COMMON_INTERNAL_H_
#define _H_DAUMEN_COMMON_INTERNAL_H_

// NOTE: Needs the following libraries: Shlwapi.lib, oleaut32.lib

#include <windows.h>
#include <shlobj.h>
#include <thumbcache.h>
#include <comdef.h>

#define CharPtr(Var) ((char*)&(Var))
#define UCharPtr(Var) ((unsigned char*)&(Var))
#define ConstCharPtr(Var) ((const char*)&(Var))
#define ZeroStructureRaw(VarPtr) ZeroMemory(VarPtr, sizeof(*VarPtr))
#define ZeroStructure(Var) ZeroMemory(CharPtr(Var), sizeof(Var))
#define CopyStructure(Dst, Src) { \
	ZeroMemory(CharPtr(Dst), sizeof(Dst)); \
	memcpy(CharPtr(Dst), CharPtr(Src), sizeof(Dst)); \
}

enum InfoHeaderType {
	IHT_INVALID = 0,
	IHT_BITMAPCOREHEADER = sizeof(BITMAPCOREHEADER),
	IHT_BITMAPINFOHEADER = sizeof(BITMAPINFOHEADER),
	IHT_BITMAPV4HEADER = sizeof(BITMAPV4HEADER),
	IHT_BITMAPV5HEADER = sizeof(BITMAPV5HEADER)
};

struct DaumenBitmap {
	BITMAPFILEHEADER FileHeader;
	InfoHeaderType InfoHeaderType;
	union {
		BITMAPCOREHEADER BitmapCore;
		BITMAPINFOHEADER BitmapInfo;
		BITMAPV4HEADER BitmapV4;
		BITMAPV5HEADER BitmapV5;
	} InfoHeader;
	BITMAP Bitmap;
	bool bBitmapIsObject;
};

#endif