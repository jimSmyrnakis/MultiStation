#include "Convolution.h"
#include <math.h>
#include <memory.h>
extern "C" {

	bool image_convolve(
		struct MaskAttributes* attrs,bool flipMask ,
		struct bmpImage* input, struct bmpImage* output) {
		if (!attrs->height || !attrs->width || !attrs->mask || !input || !output) {
			return false;
		}

		output->header = input->header;
		output->channels = input->channels;
		output->height = input->height;
		output->width = input->width;
		output->colorTable = NULL;
		if (input->colorTable) {
			output->colorTable = (uint8_t*)malloc(input->header.dataOffset - sizeof(struct BMPHeader));
			if (output->colorTable == NULL) {
				return false;
			}

			memcpy_s(output->colorTable,
				input->header.dataOffset - sizeof(struct BMPHeader),
				input->colorTable,
				input->header.dataOffset - sizeof(struct BMPHeader));


		}
		uint32_t bytesPerPixel = input->channels;
		uint32_t stride = (input->width * bytesPerPixel + 3) & ~3;
		output->pixels = (uint8_t*)malloc(stride * output->height * sizeof(uint8_t));
		if (!output->pixels) {
			return false;
		}

		return convolve(
			attrs, flipMask ,
			input->height, input->width, stride, input->channels,
			input->pixels, output->pixels);

	}




	static float clamp(float val, float start, float end) {
		
		if (start > end)
			return clamp(val, end, start);

		if (val < start)
			val = start;
		if (val > end)
			val = end;
		return val;
	}

	bool convolve(
		struct MaskAttributes* attrs , bool flipMask,
		uint32_t ImgRows, uint32_t ImgCols, uint32_t ImgSride, uint8_t ImgChannels,
		void* input_buf, void* output_buf) {
		
		if (
			!attrs->height || !attrs->width || 
			!attrs->mask || !input_buf || 
			!output_buf || !ImgChannels ||
			!ImgSride || !ImgCols || !ImgRows) {
			return false;
		}

		float* mask = (float*)attrs->mask;
		uint8_t* ipixels = (uint8_t*)input_buf;
		uint8_t* opixels = (uint8_t*)output_buf;

		for (uint32_t y = 0; y < ImgRows; y++) {
			for (uint32_t x = 0; x < ImgCols; x++) {
				for (int c = 0; c < ImgChannels; c++) {
					float sum = 0;
					uint32_t n2 = 0, m2 = 0;
					m2 = attrs->height / 2;
					n2 = attrs->width / 2;
					for (int m = 0; m < attrs->height; m++) {
						for (int n = 0; n < attrs->width; n++) {
							float MaskVal = 
								mask[(attrs->height - 1 - m) * attrs->width 
								+ (attrs->width - 1 - n)];
							if (!flipMask)
								MaskVal = *(mask + m * attrs->width + n);

							int idx = x + (n - n2); // στήλη (x)
							int idy = y + (m - m2); // γραμμή (y)
							uint8_t PixlVal = 0;
							if (idx < 0 || idx >= ImgCols || idy < 0 || idy >= ImgRows)
								continue;

							PixlVal = ipixels[idy * ImgSride + idx * ImgChannels + c];
							;
							sum += PixlVal * MaskVal;
						}
					}
					opixels[y * ImgSride + x * ImgChannels + c] = 
						(uint8_t)(clamp(roundf(sum), 0, 255));

				}
			}
		}


		return true;
	}
}