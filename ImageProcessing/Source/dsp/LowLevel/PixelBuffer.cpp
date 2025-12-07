#include "PixelBuffer.h"
#include "PointTransforms/Binarization.h"
#include "PointTransforms/GausianNoise.h"
#include "Filters/Filters.h"
extern "C" {

	// This is just for checking , this code will not called yet , first i have to make compatible the rest 
	// methods and add them to this each one of them , as well use some wrappers around them so the rest 
	// code see's no change 
	static PBO_Resolve Map[] = {
		BinarizationResolve,
		GausianNoiseResolve,
		MedianFilterResolve
	};

	bool getPBOAttributeMap(enum PBOType type, struct PBOAttributes* attrs) {
		
		for (int i = 0; i < (sizeof(Map) / sizeof(PBO_Resolve)); i++) {
			if (Map[i] == NULL) continue;
			if (Map[i](type , attrs))
				return true;
		}

		return false;
	}

	bool ImageProcess(struct bmpImage* dest,const struct bmpImage* source, struct PBOAttributes* attrs , struct Params* params) {

		// check parameters values
		if (!dest || !source || !source->pixels || !attrs || !params) {
			ASSERT(0 , "Bad Parameter !");
			return false;
		}
		if (!attrs->PBOO_Func ) {
			ASSERT(0, "No Function Defined !!!");
			return false;
		}
		if (source->channels > 4 || !source->height || !source->width) {
			ASSERT(0, "Bad Parameters !");
			return false;
		}

		// find the hole size of the source buffer and it's stride
		uint32_t bytesPerPixel = source->channels;
		uint32_t SourceStride = (source->width * bytesPerPixel + 3) & ~3;

		// Create Pixel Buffer Objects for the representation of this info
		struct PixelBufferObject PBO_dest, PBO_src;
		// fill in the source PBO , dest PBO will be filled by the Callback function
		PBO_src.channels = source->channels;
		PBO_src.height = source->height;
		PBO_src.width = source->width;
		PBO_src.buffer = source->pixels;
		PBO_src.stride = SourceStride;
		memset(&PBO_dest, 0, sizeof(PBO_dest));

		// Callback the operation function
		if (!attrs->PBOO_Func(&PBO_dest, &PBO_src, params)) {
			ASSERT(0, "Function Failed :(");
			return false;
		}
		// check PBO destination
		if (!PBO_dest.channels || !PBO_dest.buffer || !PBO_dest.height || !PBO_dest.width || !PBO_dest.stride || (PBO_dest.stride < PBO_dest.width * PBO_dest.channels)) {
			ASSERT(0, "Bad PBO returned !");
			if (PBO_dest.buffer)
				params->allocator.memfree(PBO_dest.buffer);
			return false;
		}


		dest->width = PBO_dest.width;
		dest->height = PBO_dest.height;
		dest->channels = PBO_dest.channels;
		dest->pixels = (uint8_t*)PBO_dest.buffer;
		uint32_t dataOffs = sizeof(struct BMPHeader);
		if (dest->channels == 1) {
			dest->colorTable = default_color_table;
			dataOffs += 256 * 4;
		}
		else {
			dest->colorTable = NULL;
		}
		// if everything is ok then filled in the data and info for the dest image
		dest->header = source->header;
		dest->header.bitCount = PBO_dest.channels * 8;
		dest->header.planes = 1;
		dest->header.signature = BMP_SIGNATURE;
		dest->header.width = dest->width;
		dest->header.height = dest->height;
		//dest->header.dataOffset = dataOffs;
		//dest->header.sizeImage = dest->width * dest->height ;
		//dest->header.size = 54;
		//dest->header.fileSize_unreleable = dest->header.size + dest->header.sizeImage;*/
		return true;

	}
}
