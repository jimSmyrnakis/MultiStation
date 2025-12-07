#include "SaltAndPepperNoise.h"


extern "C" {

	bool image_salt_papper_noise(struct bmpImage* dest, const struct bmpImage* source, float prob) {
		if (!dest || !source || !source->pixels) {
			return false;
		}

		dest->header = source->header;
		dest->width = source->width;
		dest->height = source->height;
		dest->channels = source->channels;
		dest->colorTable = (uint8_t*)malloc(source->header.dataOffset - sizeof(struct BMPHeader));
		if (
			!dest->colorTable) {
			return false;
		}
		memcpy_s(
			dest->colorTable,
			source->header.dataOffset - sizeof(struct BMPHeader),
			source->colorTable,
			source->header.dataOffset - sizeof(struct BMPHeader)
		);

		uint32_t bytesPerPixel = source->channels;
		uint32_t rowSize = (source->width * bytesPerPixel + 3) & ~3;
		uint32_t totalBytes = rowSize * dest->height;
		dest->pixels = (uint8_t*)malloc(totalBytes);
		if (!dest->pixels) {
			return false;
		}


		int data, data1, data2;
		data = (float)(prob * 32768 / 2);
		data1 = data + 16384;
		data2 = 16384 - data;
		for (uint32_t i = 0; i < dest->height; i++) {
			// for each row
			for (uint32_t j = 0; j < dest->width; j++) {
				// for each col
				data = rand();
				// for each channel common random value
				for (uint32_t c = 0; c < dest->channels; c++) {
					// for each channel
					uint32_t index = i * rowSize + j * bytesPerPixel + c;
					// rowSize or Stride means the same
					//
					float Val = 0.0f;
					
					if (data >= 16384 && data < data1)
						Val = 0; // Pepper
					else if (data >= data2 && data < 16384)
						Val = 255; // Salt
					else
						Val = source->pixels[index]; // image
					dest->pixels[index] = static_cast<uint8_t>(Val);
				}
			}
		}


		return true;
	}
}
