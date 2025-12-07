#include "Converters.h"

extern "C" bool convert_rgb_to_grayscale_avarage(struct bmpImage* dest,
    const struct bmpImage* source)
{
    if (!dest || !source || !source->pixels || source->channels < 3)
        return false;

    uint32_t width = source->width;
    uint32_t height = source->height;

    dest->width = width;
    dest->height = height;
    dest->channels = 1;

    uint32_t rowSize = (width + 3) & ~3;     // padding σε τετράδα
    uint32_t pixelDataSize = rowSize * height;

    // allocate pixel buffer
    dest->pixels = (uint8_t*)malloc(pixelDataSize);
    if (!dest->pixels) return false;
    memset(dest->pixels, 0, pixelDataSize);

    // ---- GRAYSCALE CONVERSION ----
    uint32_t srcStride = (width * source->channels + 3) & ~3;

    for (uint32_t y = 0; y < height; y++)
    {
        const uint8_t* srow = source->pixels + y * srcStride;
        uint8_t* drow = dest->pixels + y * rowSize;

        for (uint32_t x = 0; x < width; x++)
        {
            uint8_t b = srow[3 * x + 0];
            uint8_t g = srow[3 * x + 1];
            uint8_t r = srow[3 * x + 2];

            drow[x] = (uint8_t)((r + g + b) / 3);
        }
    }

    // ---- BUILD VALID BMP HEADER ----
    dest->header.signature = 0x4D42;  // 'BM'
    dest->header.dataOffset = 54 + 1024; // file header + info header + palette

    dest->header.fileSize_unreleable = dest->header.dataOffset + pixelDataSize;

    dest->header.bfReserved1 = 0;
    dest->header.bfReserved2 = 0;
    dest->header.size = 40;                  // BITMAPINFOHEADER size
    dest->header.width = width;
    dest->header.height = height;
    dest->header.planes = 1;
    dest->header.bitCount = 8;               // grayscale
    dest->header.compression = 0;
    dest->header.sizeImage = pixelDataSize;
    dest->header.xPelsPerMeter_unreleable = 2835;
    dest->header.yPelsPerMeter_unreleable = 2835;
    dest->header.clrUsed_unreleable = 256;
    dest->header.clrImportant_unreleable = 256;

    // allocate palette
    dest->colorTable = (uint8_t*)malloc(1024);
    if (!dest->colorTable) return false;

    for (int i = 0; i < 256; i++)
    {
        dest->colorTable[4 * i + 0] = i; // B
        dest->colorTable[4 * i + 1] = i; // G
        dest->colorTable[4 * i + 2] = i; // R
        dest->colorTable[4 * i + 3] = 0;
    }

    return true;
}


extern "C" bool convert_rgb_to_grayscale_luminosity(struct bmpImage* dest, const struct bmpImage* source , float r_s, float g_s, float b_s) {
	if (!dest || !source || !source->pixels) {
		return false;
	}

	if (source->channels < 3) {
		// Source image must have at least 3 channels (RGB)
		return false;
	}

	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = 1; // Grayscale
	uint32_t bytesPerPixelSrc = source->channels;
	uint32_t rowSizeSrc = (source->width * bytesPerPixelSrc + 3) & ~3;
	uint32_t rowSizeDest = (dest->width * dest->channels + 3) & ~3;
	uint32_t totalBytesDest = rowSizeDest * dest->height;

	dest->pixels = (uint8_t*)malloc(totalBytesDest);
	if (!dest->pixels) {
		return false;
	}

	for (uint32_t i = 0; i < dest->height; i++) {
		for (uint32_t j = 0; j < dest->width; j++) {
			uint32_t indexSrc = i * rowSizeSrc + j * bytesPerPixelSrc;
			uint32_t indexDest = i * rowSizeDest + j * dest->channels;
			uint8_t r = source->pixels[indexSrc + 2];
			uint8_t g = source->pixels[indexSrc + 1];
			uint8_t b = source->pixels[indexSrc + 0];
			// Convert to grayscale using luminosity method
			uint16_t gray = static_cast<uint16_t>(r_s * r + g_s * g + b_s * b);
			if (gray > 255)
				gray = 255;
			dest->pixels[indexDest] = gray;
		}
	}

	dest->colorTable = default_color_table;
	dest->header.bitCount = 8;
	dest->header.dataOffset = sizeof(struct BMPHeader) + 1024;

	return true;
}