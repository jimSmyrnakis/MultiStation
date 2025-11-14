#include "Binarization.h"
#include "../PixelBuffer.h"

bool BinarizationResolve(enum PBOType type, struct PBOAttributes* attrs) {
	ASSERT(attrs, "Bad Parameter !");

	if (type != PBOT_BINARIZATION) return false;

	attrs->type = type;
	attrs->PBOO_Func = BinarizationOperation;
	return true;
}



bool image_binarize(
	struct bmpImage* dest,
	const struct bmpImage* source,
	uint8_t threshold,
	struct Allocator allocator ) {

	struct PBOAttributes attrs;
	if (!getPBOAttributeMap(PBOT_BINARIZATION, &attrs)) return false;
	
	struct Params params;
	params.allocator = allocator;
	params.count = 1;
	params.parameters = (void**)allocator.memalloc(sizeof(void*));
	if (!params.parameters) return false;

	params.parameters[0] = &threshold;

	bool res = ImageProcess(dest, source, &attrs, &params);


	allocator.memfree(params.parameters);
	return res;
}

static bool binarize_grayscale(
	uint32_t height, uint32_t stride, uint32_t width, 
	void* in_buf, void* out_buf, uint8_t threshold
) {
	if (!height || !width || stride < width || !in_buf || !out_buf) {
		return false;
	}

	uint8_t* iPixels = (uint8_t*)in_buf;
	uint8_t* oPixels = (uint8_t*)out_buf;

	for (uint32_t y = 0; y < height; y++) {
		for (uint32_t x = 0; x < width; x++) {
			uint32_t index = y * stride + x;
			uint8_t iVal = iPixels[index];
			oPixels[index] = (iVal < threshold) ? 0 : 255;
		}
	}

	return true;
}

bool BinarizationOperation(
	struct PixelBufferObject* dest,
	const struct PixelBufferObject* src,
	struct Params* parms) {

	if (!dest || !src || !parms) return false;

	if (src->channels != 1) return false;

	if (src->buffer == NULL) return false;

	if (!src->stride || !src->height || !src->width || (src->stride < src->width * src->channels)) return false;

	if (parms->count != 1) return false;


	dest->width = src->width;
	dest->height = src->height;
	dest->channels = src->channels;
	dest->stride = (dest->width * dest->channels + 3) & ~3;
	(*dest).buffer = parms->allocator.memalloc((size_t)dest->stride * dest->height);
	if (!dest->buffer) {
		return false;
	}

	return binarize_grayscale(
		dest->height, dest->stride, dest->width, 
		src->buffer, dest->buffer, *((uint8_t*)parms->parameters[0])
	);
}