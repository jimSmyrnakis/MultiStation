#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Core.hpp"
extern "C" {


	/**
	 * @param[in] image The input image and its data
	 * @param[in] tile_x tile/sub-image x base coordinate
	 * @param[in] tile_y tile/sub-image y base coordinate
	 * @param[in] tile_width tile/sub-image number of collums
	 * @param[in] tile_height tile/sub-image number of rows
	 * @param[in] clipFactor A Factor that defines a limit ammount of 
	 * pixels per histogram value (Factor * (ImageSize / 256=histoSize)
	 * @param[in] normallize At the end normalize histogram values
	 * @return NULL if something goes wrong or a float array of 256  floats
	 * @details Caculates the local/global histogram of the image/sub-image/tile , carefull
	 * this function does not return the actuall histogram of the image , but caculates a better
	 * version of it . For allocating memory malloc is used so call free to free the array that this
	 * function return's .
	 */
	float* image_histogram_init(
		const struct bmpImage* image , 
		uint32_t tile_x ,
		uint32_t tile_y ,
		uint32_t tile_width ,
		uint32_t tile_height,
		float clipFactor,
		bool normallize);

	float* image_histogram_init_full(
		const struct bmpImage* image,
		bool normallize);


	extern "C" bool image_histogram_tile_cdf(
		uint32_t tile_x,
		uint32_t tile_y,
		uint32_t tile_w,
		uint32_t tile_h,
		uint32_t tile_base_w,
		uint32_t tile_base_h,
		float*** cdfs,
		float clipFactor,
		const struct bmpImage* src
	);

	extern "C" bool image_histogram_tile_equalization2(
		struct bmpImage* dest,
		uint32_t tile_x,
		uint32_t tile_y,
		uint32_t tile_w,
		uint32_t tile_h,
		uint32_t tile_base_w,
		uint32_t tile_base_h,
		float*** cdfs,
		const struct bmpImage* src);

	extern "C" bool image_histogram_equalization2(
		struct bmpImage* dest,
		uint16_t tiles,
		float clipFactor,
		const struct bmpImage* source);


	
	

	
	bool image_histogram_equalization(
		struct bmpImage* dest,
		uint16_t tiles ,
		float clipFactor,
		const struct bmpImage* source);






	bool histogram_write_to_file(const char* filename, float* histogram, size_t size);
}

