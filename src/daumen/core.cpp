// 2022 - ∞ (c) blurryroots innovation qanat OÜ. All rights reserved.

#include <daumen/core.h>

#include "windows-common-internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

DaumenBitmap* NewDaumen() {
	DaumenBitmap* Daumen = new DaumenBitmap{};
	ZeroMemory(Daumen, sizeof(DaumenBitmap));

	return Daumen;
}

void FreeDaumenBitmap(BITMAP* Bitmap, bool bIsObject = false) {
	if (bIsObject) {
		DeleteObject(Bitmap);
	}
	else {
		free(Bitmap->bmBits);
	}
}

void FreeDaumen(DaumenBitmap* Daumen) {
	if (nullptr != Daumen) {
		if (nullptr != Daumen->Bitmap.bmBits) {
			FreeDaumenBitmap(&(Daumen->Bitmap), Daumen->bBitmapIsObject);
		}
		ZeroMemory(Daumen, sizeof(*Daumen));
	}
}

int AreDaumenEqual(const DaumenBitmap* A, const DaumenBitmap* B) {
	if (nullptr == A || nullptr == B) {
		return 0;
	}

	int FileHeaderEqual = memcmp(
		CharPtr(A->FileHeader),
		CharPtr(B->FileHeader),
		sizeof(BITMAPFILEHEADER)
	);
	if (0 != FileHeaderEqual) return 0;

	int InfoHeaderEqual = memcmp(
		CharPtr(A->InfoHeader),
		CharPtr(B->InfoHeader),
		sizeof(BITMAPINFOHEADER)
	);
	if (0 != InfoHeaderEqual) return 0;

	int BitsCountEqual =
		GetDaumenBitmapImageDataSize(A) == GetDaumenBitmapImageDataSize(B);
	if (BitsCountEqual) {
		int BitsEqual = memcmp(
			CharPtr(A->Bitmap.bmBits),
			CharPtr(B->Bitmap.bmBits),
			GetDaumenBitmapImageDataSize(A)
		);

		return 0 == BitsEqual;
	}

	return 0;
}

long GetDaumenBitmapWidth(const DaumenBitmap* Thumbnail) {
	switch (Thumbnail->InfoHeaderType) {
	case (InfoHeaderType::IHT_INVALID):
		return 0;
	case (InfoHeaderType::IHT_BITMAPCOREHEADER):
		return Thumbnail->InfoHeader.BitmapCore.bcWidth;
	case (InfoHeaderType::IHT_BITMAPINFOHEADER):
		return Thumbnail->InfoHeader.BitmapInfo.biWidth;
	case (InfoHeaderType::IHT_BITMAPV4HEADER):
		return Thumbnail->InfoHeader.BitmapV4.bV4Width;
	case (InfoHeaderType::IHT_BITMAPV5HEADER):
		return Thumbnail->InfoHeader.BitmapV5.bV5Width;
	}

	assert(false && "Unknown header type!");
	return 0;
}

long GetDaumenBitmapHeight(const DaumenBitmap* Thumbnail) {
	switch (Thumbnail->InfoHeaderType) {
	case (InfoHeaderType::IHT_INVALID):
		return 0;
	case (InfoHeaderType::IHT_BITMAPCOREHEADER):
		return Thumbnail->InfoHeader.BitmapCore.bcHeight;
		break;
	case (InfoHeaderType::IHT_BITMAPINFOHEADER):
		return Thumbnail->InfoHeader.BitmapInfo.biHeight;
		break;
	case (InfoHeaderType::IHT_BITMAPV4HEADER):
		return Thumbnail->InfoHeader.BitmapV4.bV4Height;
		break;
	case (InfoHeaderType::IHT_BITMAPV5HEADER):
		return Thumbnail->InfoHeader.BitmapV5.bV5Height;
		break;
	}

	assert(false && "Unknown header type!");
	return 0;
}

long GetDaumenBitmapFileSize(const DaumenBitmap* Thumbnail) {
	return Thumbnail->FileHeader.bfSize;
}

long GetDaumenBitmapHeaderSize(const DaumenBitmap* Thumbnail) {
	return Thumbnail->FileHeader.bfOffBits;
}

long GetDaumenBitmapImageDataSize(const DaumenBitmap* Thumbnail) {
	return GetDaumenBitmapFileSize(Thumbnail) - GetDaumenBitmapHeaderSize(Thumbnail);
}