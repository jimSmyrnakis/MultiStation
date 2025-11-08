#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Core.hpp"
extern "C" {

	float* image_histogram_init(const struct bmpImage* image , bool normallize);

	bool histogram_write_to_file(const char* filename, float* histogram, size_t size);

	bool image_histogram_equalization(struct bmpImage* dest, const struct bmpImage* source);
}