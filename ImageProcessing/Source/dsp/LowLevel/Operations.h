#pragma once
#include "Core.hpp"

extern "C" {

	bool add_images_truncate(
		struct bmpImage* dest,
		uint8_t value,
		const struct bmpImage* source);

	bool subtract_images_truncate(
		struct bmpImage* dest,
		uint8_t value,
		const struct bmpImage* source);
}