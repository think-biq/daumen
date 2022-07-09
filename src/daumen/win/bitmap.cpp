// 2022 - ∞ (c) blurryroots innovation qanat OÜ. All rights reserved.

#include <daumen/bitmap.h>

#include "windows-common-internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable : 4189)
#pragma warning(disable : 4100)
int ReadDaumenBitmap(DaumenBitmap* Thumbnail, const char* FilePath) {
	if (nullptr == Thumbnail) {
		return 0;
	}

	ZeroStructure(Thumbnail);

	if (FILE* ThumbnailFile = fopen(FilePath, "rb")) {
		char* FileHeaderHead = CharPtr(Thumbnail->FileHeader);
		size_t FileHeaderSize = sizeof(Thumbnail->FileHeader);
		fread(FileHeaderHead, FileHeaderSize, 1, ThumbnailFile);

		DWORD RemainingHeaderSize = Thumbnail->FileHeader.bfOffBits - ftell(ThumbnailFile);
		switch (RemainingHeaderSize) {
		case (InfoHeaderType::IHT_BITMAPCOREHEADER):
			Thumbnail->InfoHeaderType = InfoHeaderType::IHT_BITMAPCOREHEADER;
			fread(CharPtr(Thumbnail->InfoHeader.BitmapCore), InfoHeaderType::IHT_BITMAPCOREHEADER, 1, ThumbnailFile);
			break;
		case (InfoHeaderType::IHT_BITMAPINFOHEADER):
			Thumbnail->InfoHeaderType = InfoHeaderType::IHT_BITMAPINFOHEADER;
			fread(CharPtr(Thumbnail->InfoHeader.BitmapInfo), InfoHeaderType::IHT_BITMAPINFOHEADER, 1, ThumbnailFile);
			break;
		case (InfoHeaderType::IHT_BITMAPV4HEADER):
			Thumbnail->InfoHeaderType = InfoHeaderType::IHT_BITMAPV4HEADER;
			fread(CharPtr(Thumbnail->InfoHeader.BitmapV4), InfoHeaderType::IHT_BITMAPV4HEADER, 1, ThumbnailFile);
			break;
		case (InfoHeaderType::IHT_BITMAPV5HEADER):
			Thumbnail->InfoHeaderType = InfoHeaderType::IHT_BITMAPV5HEADER;
			fread(CharPtr(Thumbnail->InfoHeader.BitmapV5), InfoHeaderType::IHT_BITMAPV5HEADER, 1, ThumbnailFile);
			break;
		default:
		case (InfoHeaderType::IHT_INVALID):
			assert(false && "Unknown header type!");
			return 0;
		}

		DWORD ImageSize = Thumbnail->FileHeader.bfSize - Thumbnail->FileHeader.bfOffBits;
		Thumbnail->Bitmap.bmBits = malloc(ImageSize);
		if (char* ImageDataHead = (char*)Thumbnail->Bitmap.bmBits) {
			assert(ImageDataHead && "Could not allocated image data!");
			for (DWORD ByteIndex = 0; ByteIndex < ImageSize; ++ByteIndex) {
				fread(ImageDataHead + ByteIndex, 1, 1, ThumbnailFile);
			}
		}

		fclose(ThumbnailFile);
	}

	return 1;
}

int WriteDaumenBitmap(const char* FilePath, const DaumenBitmap* Thumbnail) {
	if (nullptr == Thumbnail || InfoHeaderType::IHT_INVALID == Thumbnail->InfoHeaderType) {
		return 0;
	}

	int Pitch = GetDaumenBitmapWidth(Thumbnail) * 3;
	if (0 != (Pitch % 4)) {
		Pitch += 4 - (Pitch % 4);
	}
	const int Padding = Pitch - (GetDaumenBitmapWidth(Thumbnail) * 3);

	const DWORD HeaderSize = Thumbnail->FileHeader.bfOffBits;
	const DWORD ImageSize = Thumbnail->FileHeader.bfSize - HeaderSize;
	const DWORD FileSize = HeaderSize + ImageSize;
	assert(FileSize == Thumbnail->FileHeader.bfSize && "Sizes are all messed up :/");

	if (FILE* BitmapFilePtr = fopen(FilePath, "wb"))
	{
		fwrite(CharPtr(Thumbnail->FileHeader), sizeof(Thumbnail->FileHeader), 1, BitmapFilePtr);
		switch (Thumbnail->InfoHeaderType) {
		case (InfoHeaderType::IHT_BITMAPCOREHEADER):
			fwrite(CharPtr(Thumbnail->InfoHeader.BitmapCore), InfoHeaderType::IHT_BITMAPCOREHEADER, 1, BitmapFilePtr);
			break;
		case (InfoHeaderType::IHT_BITMAPINFOHEADER):
			fwrite(CharPtr(Thumbnail->InfoHeader.BitmapInfo), InfoHeaderType::IHT_BITMAPINFOHEADER, 1, BitmapFilePtr);
			break;
		case (InfoHeaderType::IHT_BITMAPV4HEADER):
			fwrite(CharPtr(Thumbnail->InfoHeader.BitmapV4), InfoHeaderType::IHT_BITMAPV4HEADER, 1, BitmapFilePtr);
			break;
		case (InfoHeaderType::IHT_BITMAPV5HEADER):
			fwrite(CharPtr(Thumbnail->InfoHeader.BitmapV5), InfoHeaderType::IHT_BITMAPV5HEADER, 1, BitmapFilePtr);
			break;
		default:
		case (InfoHeaderType::IHT_INVALID):
			assert(false && "Unknown header type!");
			return 0;
		}

		DWORD At = 0;
		assert((At = ftell(BitmapFilePtr)) == HeaderSize);

		char* RawData = (char*)Thumbnail->Bitmap.bmBits;
		for (DWORD ByteIndex = 0; ByteIndex < ImageSize; ++ByteIndex) {
			char Value = RawData[ByteIndex];
			char* RawDataHead = RawData + ByteIndex;
			fwrite(RawDataHead, 1, 1, BitmapFilePtr);
		}
		assert((At = ftell(BitmapFilePtr)) == FileSize);

		fclose(BitmapFilePtr);
	}
	else
	{
		return 0;
	}

	return 1;
}
#pragma warning(pop)