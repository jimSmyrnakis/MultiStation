#pragma once
#include <stdint.h>
#include <stddef.h>
namespace MultiStation {

	enum CompressionType : uint32_t {
		COMPRESSION_NONE = 0
	};

	enum PixelOrder : uint32_t {
		PIXEL_ORDER_RGBA = 0,
		PIXEL_ORDER_BGRA = 1,
		PIXEL_ORDER_ARGB = 2,
		PIXEL_ORDER_ABGR = 3,
		PIXEL_ORDER_RGB = 4,
		PIXEL_ORDER_BGR = 5
	};


	extern "C" struct PixelBuffer {
		uint32_t				width;
		uint32_t				stride;
		uint32_t				height;
		void*					buffer;
		uint8_t					channels;
		uint8_t					bitsPerChannel[4];
		enum CompressionType	compression; // 0 = none
		enum PixelOrder			pixelOrder;
		uint32_t				planes;
		uint32_t 				mipLevels;
		
	};
}
