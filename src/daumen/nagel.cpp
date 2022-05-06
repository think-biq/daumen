
#include <daumen/nagel.h>

#include "windows-common-internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

int ExportThumbnailFromFile(DaumenBitmap* Thumbnail, const char* FilePath, const uint32_t Resolution) {
	if (nullptr == Thumbnail) {
		return 0;
	}

	_bstr_t ItemFilePath(FilePath);
	int AllMessedUp = 1;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr)) {
		IShellItem* psi;
		hr = SHCreateItemFromParsingName(ItemFilePath, NULL, IID_PPV_ARGS(&psi));
		if (SUCCEEDED(hr)) {
			IThumbnailProvider* pThumbProvider;
			hr = psi->BindToHandler(NULL, BHID_ThumbnailHandler, IID_PPV_ARGS(&pThumbProvider));
			if (SUCCEEDED(hr)) {
				// Thumbnail to create
				HBITMAP g_hThumbnail;
				WTS_ALPHATYPE wtsAlpha;
				hr = pThumbProvider->GetThumbnail(Resolution, &g_hThumbnail, &wtsAlpha);
				if (SUCCEEDED(hr)) {
					DIBSECTION ds; ZeroStructure(ds);
					const int SectionTypeSize = sizeof(ds);
					const int BitmapTypeSize = sizeof(ds.dsBm);

					int nSizeDS;
					// hBitmap is a DIB
					if (SectionTypeSize == (nSizeDS = ::GetObject(g_hThumbnail, SectionTypeSize, &ds))) {
					}
					else if (BitmapTypeSize == (nSizeDS = ::GetObject(g_hThumbnail, BitmapTypeSize, &(ds.dsBm)))) {
						ZeroStructure(ds.dsBmih);
						ZeroStructure(ds.dsBitfields);
						ZeroStructure(ds.dshSection);
						ZeroStructure(ds.dsOffset);
					}
					else {
						assert("SOMETHING IS FISHY HERE!");
					}
					assert(nullptr != ds.dsBm.bmBits && "Could not read bitmap image data!");

					// This should not be necessary.
					// But many MSDN examples perform these calculations.
					if (0 == ds.dsBm.bmWidthBytes) {
						ds.dsBm.bmWidthBytes = ((ds.dsBm.bmWidth * ds.dsBm.bmBitsPixel + 31) & ~31);
						ds.dsBm.bmWidthBytes /= 8;
					}
					if (0 == ds.dsBmih.biSizeImage) {
						ds.dsBmih.biSizeImage = ds.dsBm.bmHeight * ds.dsBm.bmWidthBytes;
					}
					// Image data at ds.dsBm.bmBits
					// Data size in ds.dsBmih.biSizeImage

					ZeroStructure(Thumbnail->FileHeader);
					Thumbnail->FileHeader.bfType = 0x4D42; // Bitmap magic constant.
					Thumbnail->InfoHeaderType = (InfoHeaderType)sizeof(ds.dsBmih);

					ZeroStructure(Thumbnail->InfoHeader);
					if (SectionTypeSize == nSizeDS) {
						CopyStructure(Thumbnail->InfoHeader.BitmapInfo, ds.dsBmih);
					}
					else {
						Thumbnail->InfoHeader.BitmapInfo.biSize = sizeof(Thumbnail->InfoHeader.BitmapInfo);
						Thumbnail->InfoHeader.BitmapInfo.biWidth = ds.dsBm.bmWidth;
						Thumbnail->InfoHeader.BitmapInfo.biHeight = ds.dsBm.bmHeight;
						Thumbnail->InfoHeader.BitmapInfo.biBitCount = ds.dsBm.bmBitsPixel;
						Thumbnail->InfoHeader.BitmapInfo.biPlanes = 1;
						Thumbnail->InfoHeader.BitmapInfo.biSizeImage = ds.dsBmih.biSizeImage;
					}

					Thumbnail->FileHeader.bfOffBits = sizeof(Thumbnail->FileHeader) + Thumbnail->InfoHeader.BitmapInfo.biSize;
					Thumbnail->FileHeader.bfSize = Thumbnail->FileHeader.bfOffBits + Thumbnail->InfoHeader.BitmapInfo.biSizeImage;

					CopyStructure(Thumbnail->Bitmap, ds.dsBm);
					Thumbnail->Bitmap.bmBits = malloc(Thumbnail->InfoHeader.BitmapInfo.biSizeImage);
					assert(Thumbnail->Bitmap.bmBits && "This is some grade A BULLSHIT!");

					// Scan top to bottom, because for some reason, the thumbnail is upside down.
					char* RawBitsSrc = (char*)ds.dsBm.bmBits;
					char* RawBitsDst = (char*)Thumbnail->Bitmap.bmBits;
					const DWORD LineSize = Thumbnail->Bitmap.bmWidthBytes;
					for (DWORD LineIndex = Thumbnail->InfoHeader.BitmapInfo.biHeight - 1; 0 < LineIndex; --LineIndex) {
						DWORD ByteIndexDst = (Thumbnail->InfoHeader.BitmapInfo.biHeight - 1 - LineIndex) * LineSize;
						DWORD ByteIndexSrc = LineIndex * LineSize;
						memcpy(RawBitsDst + ByteIndexDst, RawBitsSrc + ByteIndexSrc, LineSize);
					}

					AllMessedUp = 0;

					if (SectionTypeSize == nSizeDS)
					{
						DeleteObject(&ds);
					}
					else
					{
						DeleteObject(&(ds.dsBm));
					}
					DeleteObject(g_hThumbnail);
				}
				pThumbProvider->Release();
			}
			psi->Release();
		}
		CoUninitialize();
	}

	return AllMessedUp;
}