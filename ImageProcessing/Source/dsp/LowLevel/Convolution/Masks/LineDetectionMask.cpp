#include "LineDetectionMask.h"
static bool isInitiallized = false;



static struct LineDetectorMasks lineMask;

static void initLineDetectorMask(void) {
	if (isInitiallized)
		return;
	float ver[3][3] = {
		{-1 , 2 , -1},
		{-1 , 2 , -1},
		{-1 , 2 , -1}
	};
	float hor[3][3] = {
		{-1 ,-1 , -1},
		{ 2 , 2 ,  2},
		{-1 ,-1 , -1}
	};
	float ldia[3][3] = {
		{ 2 ,-1 , -1},
		{-1 , 2 , -1},
		{-1 ,-1 ,  2}
	};
	float rdia[3][3] = {
		{-1 ,-1 ,  2},
		{-1 , 2 , -1},
		{ 2 ,-1 , -1}
	};

	memcpy_s(lineMask.ver, 3 * 3 * sizeof(float), ver, 3 * 3 * sizeof(float));
	memcpy_s(lineMask.hor, 3 * 3 * sizeof(float), hor, 3 * 3 * sizeof(float));
	memcpy_s(lineMask.ldia, 3 * 3 * sizeof(float), ldia, 3 * 3 * sizeof(float));
	memcpy_s(lineMask.rdia, 3 * 3 * sizeof(float), rdia, 3 * 3 * sizeof(float));

	isInitiallized = true;
}
extern "C" bool LineDetectionMask(enum MaskType type, struct MaskAttributes* attrs) {
	ASSERT(attrs, "Bad Parameter !");
	initLineDetectorMask();

	switch (type) {
	case LINE_DETECTION_HORIZONTAL: attrs->mask = lineMask.hor; break;
	case LINE_DETECTION_LEFT_DIAGONAL: attrs->mask = lineMask.ldia; break;
	case LINE_DETECTION_RIGHT_DIAGONAL: attrs->mask = lineMask.rdia; break;
	case LINE_DETECTION_VERTICAL: attrs->mask = lineMask.ver; break;
	default: return false;
	}

	attrs->height = attrs->width = 3;
	attrs->type = type;

	return true;
}
