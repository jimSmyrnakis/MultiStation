#include "GausianNoise.h"
#include "../PixelBuffer.h"
#include <math.h>
#ifdef __cplusplus
extern "C" {
#endif 
	bool image_GausianNoise(
		struct bmpImage* dest,
		const struct bmpImage* source,
		float mean,
		float variance,
		struct Allocator allocator
	) {

		struct PBOAttributes attrs;
		if (!getPBOAttributeMap(PBOT_GAUSIAN_NOISE, &attrs)) return false;

		struct Params params;
		params.allocator = allocator;
		params.count = 2;
		params.parameters = (void**)allocator.memalloc(2 * sizeof(void*));
		if (!params.parameters) return false;

		params.parameters[0] = &mean;
		params.parameters[1] = &variance;

		bool res = ImageProcess(dest, source, &attrs, &params);


		allocator.memfree(params.parameters);
		return res;
	}

	static bool GausianNoise(
		struct PixelBufferObject* dest,
		const struct PixelBufferObject* src,
		float mean,
		float var
	) {
		int x, y;
		float noise, theta;
		uint8_t* dst = (uint8_t*)dest->buffer;
		uint8_t* sr = (uint8_t*)src->buffer;
		for (y = 0;y < dest->height;y++)
			for (x = 0;x < dest->width;x++)
			{
				for (uint8_t c = 0; c < dest->channels; c++) {
					uint32_t index = y * dest->stride + x * dest->channels + c;
					noise = sqrt(-2 * var * log(1.0 - (float)rand() / 32767.1));
					theta = rand() * 1.9175345e-4 - 3.14159265;
					noise = noise * cos(theta);
					noise = noise + mean;
					if (noise > 255)noise = 255;
					if (noise < 0) noise = 0;
					dst[index] = sr[index] + (unsigned char)(noise + 0.5);
				}
				
			}
		return true;
	}

	bool GausianNoiseOperation(
		struct PixelBufferObject* dest,
		const struct PixelBufferObject* src,
		struct Params* parms) {
		
		dest->channels = src->channels;
		dest->stride = src->stride;
		dest->width = src->width;
		dest->height = src->height;
		size_t sz = src->height * src->stride;
		dest->buffer = parms->allocator.memalloc(sz);
		if (!dest->buffer) {
			ASSERT(0, "No memory !");
			return false;
		}

		return GausianNoise(dest, src, *((float*)parms->parameters[0]), *((float*)parms->parameters[1]));
	}

	

	bool GausianNoiseResolve(enum PBOType type, struct PBOAttributes* attrs) {
		ASSERT(attrs, "Bad Parameter !");

		if (type != PBOT_GAUSIAN_NOISE) return false;

		attrs->PBOO_Func = GausianNoiseOperation;
		attrs->type = type;

		return true;
	}

#ifdef __cplusplus

}
#endif 