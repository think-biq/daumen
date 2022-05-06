
#include "windows-common-internal.h"

InfoHeaderType GetHeaderTypeFromSize(uint64_t HeaderSize) {
	switch (HeaderSize) {
	case (InfoHeaderType::IHT_BITMAPCOREHEADER):
		return InfoHeaderType::IHT_BITMAPCOREHEADER;
	case (InfoHeaderType::IHT_BITMAPINFOHEADER):
		return InfoHeaderType::IHT_BITMAPINFOHEADER;
	case (InfoHeaderType::IHT_BITMAPV4HEADER):
		return InfoHeaderType::IHT_BITMAPV4HEADER;
	case (InfoHeaderType::IHT_BITMAPV5HEADER):
		return InfoHeaderType::IHT_BITMAPV5HEADER;
	case (InfoHeaderType::IHT_INVALID):
		return InfoHeaderType::IHT_INVALID;
	}

	return InfoHeaderType::IHT_INVALID;
}