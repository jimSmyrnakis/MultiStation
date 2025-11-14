#include "Other.h"

extern "C" {
	
	static float blurKernel[3][3];
	static bool isInit = false;

	void initBlur(void) {
		if (isInit)	return;

		uint32_t size = 3 * 3;
		uint32_t middle = size / 2;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if ((i == j) && (j == middle))
					blurKernel[i][j] = 1;
				else
					blurKernel[i][j] = 1.0f / size;
			}
		}

		isInit = true;
	}


	bool lowPassSpatialMask(enum MaskType type, struct MaskAttributes* attrs) {
		ASSERT(attrs, "Bad Parameter !");
		initBlur();
		if (type != FILTER_LOW_PASS_SPATIAL)
			return false;

		attrs->mask = blurKernel;
		attrs->height = attrs->width = 3;
		attrs->type = type;

		return true;
	}
}
