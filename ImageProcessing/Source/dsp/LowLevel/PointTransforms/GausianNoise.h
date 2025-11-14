#ifndef GAUSIAN_NOISE_H
#define GAUSIAN_NOISE_H

#include "Core.hpp"

#ifdef __cplusplus
extern "C" {
#endif 
	bool image_GausianNoise(
		struct bmpImage* dest,
		const struct bmpImage* source,
		float mean,
		float variance ,
		struct Allocator allocator
	);

	bool GausianNoiseOperation(
		struct PixelBufferObject* dest,
		const struct PixelBufferObject* src,
		struct Params* parms);

	bool GausianNoiseResolve(enum PBOType type, struct PBOAttributes* attrs);

#ifdef __cplusplus

}
#endif 

#endif 