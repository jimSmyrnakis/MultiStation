#pragma once
#include "Core.hpp"


extern "C" {
	struct PixelBufferObject {
		uint32_t width;
		uint32_t stride;
		uint32_t height;
		void*	 buffer;
		uint8_t  channels;
	};

	struct Params {
		void** parameters;
		uint32_t count;
		struct Allocator allocator;
	};

	typedef bool (*PixelBufferOperation)(
		struct PixelBufferObject* dest,
		const struct PixelBufferObject* src ,
		struct Params* parms);

	enum PBOType {
		PBOT_BINARIZATION ,
		PBOT_GAUSIAN_NOISE
	};

	struct PBOAttributes {
		enum PBOType type;
		PixelBufferOperation PBOO_Func;

	};

	typedef bool (*PBO_Resolve)(enum PBOType type, struct PBOAttributes* attrs);

	bool getPBOAttributeMap(enum PBOType type , struct PBOAttributes* attrs );

	bool ImageProcess(
		struct bmpImage* dest, 
		const struct bmpImage* src, 
		struct PBOAttributes* attrs, 
		struct Params* params);



}
