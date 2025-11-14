#include "Rotation.h"
#include <math.h>
#include <memory.h>
extern "C" bool Image_rotate(const struct bmpImage* srcImage, struct bmpImage* dstImage, float rads) {
	//check parameters
	if (!srcImage || !dstImage || !srcImage->pixels) {
		return false;
	}

	// copy header
	dstImage->header = srcImage->header;
	dstImage->width = srcImage->width;
	dstImage->height = srcImage->height;
	dstImage->channels = srcImage->channels;

	// allocate memory for destination image
	uint32_t bytesPerPixel = dstImage->channels;
	uint32_t rowSize = (dstImage->width * bytesPerPixel + 3) & ~3;
	uint32_t totalBytes = rowSize * dstImage->height;
	dstImage->pixels = (uint8_t*)malloc(totalBytes);
	if (!dstImage->pixels) {
		return false;
	}


	// calculate center of the image
	float centerX = (float)(srcImage->width) / 2.0f ;
	float centerY = (float)(srcImage->height) / 2.0f ;
	float cosTheta = cosf(rads);
	float sinTheta = sinf(rads);
	
	// perform rotation
	for (int x = 0; x < (int)dstImage->width; x++) {
		for (int y = 0; y < (int)dstImage->height; y++) {

			// calculate source coordinates
			float srcX = cosTheta * (x - centerX) + sinTheta * (y - centerY) + centerX;
			float srcY = -sinTheta * (x - centerX) + cosTheta * (y - centerY) + centerY;

			// Round and floor to nearest integer
			int srcXInt = (int)(srcX + 0.5f);
			int srcYInt = (int)(srcY + 0.5f);

			// check bounds
			if (srcXInt >= 0 && srcXInt < (int)srcImage->width && srcYInt >= 0 && srcYInt < (int)srcImage->height) {
				// copy pixel data
				for (uint32_t c = 0; c < dstImage->channels; c++) {
					uint32_t srcIndex = srcYInt * rowSize + srcXInt * bytesPerPixel + c;
					uint32_t dstIndex = y * rowSize + x * bytesPerPixel + c;
					dstImage->pixels[dstIndex] = srcImage->pixels[srcIndex];
				}
			}
			else {
				// set background color (black)
				for (uint32_t c = 0; c < dstImage->channels; c++) {
					uint32_t dstIndex = y * rowSize + x * bytesPerPixel + c;
					dstImage->pixels[dstIndex] = 0;
				}
			}
		}
	}

	//copy src color table
	if (srcImage->colorTable != NULL) {
		dstImage->colorTable = (uint8_t*)malloc(srcImage->header.dataOffset - sizeof(struct BMPHeader));
		if (srcImage->colorTable && dstImage->colorTable) {
			size_t colorTableSize = srcImage->header.dataOffset - sizeof(struct BMPHeader);
			memcpy(dstImage->colorTable, srcImage->colorTable, colorTableSize);
		}
	}



	return true;
}