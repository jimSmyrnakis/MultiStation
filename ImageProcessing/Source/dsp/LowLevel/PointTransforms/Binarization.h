#ifndef BINARIZATION_H
#define BINARIZATION_H

	#include "Core.hpp"

	#ifdef __cplusplus
		extern "C" {
	#endif 
			bool image_binarize(
				struct bmpImage* dest,
				const struct bmpImage* source,
				uint8_t threshold ,
				struct Allocator allocator
				);

			bool BinarizationOperation(
				struct PixelBufferObject* dest,
				const struct PixelBufferObject* src,
				struct Params* parms);

			bool BinarizationResolve(enum PBOType type, struct PBOAttributes* attrs);

	#ifdef __cplusplus
			
		}
	#endif 

#endif 