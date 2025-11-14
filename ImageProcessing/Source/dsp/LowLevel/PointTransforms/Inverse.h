#pragma once
#include "Core.hpp"

extern "C" bool image_inverse(
	struct bmpImage* dest,
	const struct bmpImage* source);
