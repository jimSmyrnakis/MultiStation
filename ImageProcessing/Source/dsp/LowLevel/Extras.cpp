#include "Extras.h"
#include "Filters/Filters.h"
#define CHECK(func) if (!(func)) { ASSERT(0, "Failed !!!"); return 1; }
#include "Convolution/Convolution.h"

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "PointTransforms/Magnitute.h"
extern "C" uint64_t mapp_value(
	uint64_t f,
	uint64_t Lmax, uint64_t Lmin,
	uint64_t Pmax, uint64_t Pmin) {

	return ((f - Lmax) / (Lmax - Lmin)) * (Pmax - Pmin) + Pmin;
}


extern "C" bool ImageNonMaximumSuppression(
	struct bmpImage* dest,
	const struct bmpImage* magnitudeImage,
	const struct bmpImage* directionImage)
{
	// allocate dest image
	dest->width = magnitudeImage->width;
	dest->height = magnitudeImage->height;
	dest->channels = 1; // single channel for NMS
	dest->colorTable = magnitudeImage->colorTable;
	dest->header = magnitudeImage->header;
	uint32_t stride = (dest->width * dest->channels + 3) & ~3;
	dest->pixels = (uint8_t*)malloc(dest->height * stride);
	if (!dest->pixels) {
		ASSERT(0, "No memory !!!");
		return false;
	}
	memset(dest->pixels, 0, dest->height * stride);

	// for convenience
	uint32_t magStride = (magnitudeImage->width * magnitudeImage->channels + 3) & ~3;
	uint32_t dirStride = (directionImage->width * directionImage->channels + 3) & ~3;

	for (uint32_t y = 1; y < dest->height - 1; y++) {
		for (uint32_t x = 1; x < dest->width - 1; x++) {
			uint32_t magIdx = y * magStride + x * magnitudeImage->channels;
			uint32_t dirIdx = y * dirStride + x * directionImage->channels;

			float mag = (float)magnitudeImage->pixels[magIdx];
			float angle = ((float)directionImage->pixels[dirIdx] / 255.0f) * 2.0f * 3.14159265f - 3.14159265f; // map back to [-pi, pi]

			// convert angle to degrees 0-180
			float angleDeg = angle * 180.0f / 3.14159265f;
			if (angleDeg < 0) angleDeg += 180.0f;

			// determine neighboring pixels to compare
			float neighbor1 = 0.0f;
			float neighbor2 = 0.0f;

			if ((angleDeg >= 0 && angleDeg < 22.5) || (angleDeg >= 157.5 && angleDeg <= 180)) {
				// horizontal edge (compare left and right)
				neighbor1 = (float)magnitudeImage->pixels[magIdx - 1];
				neighbor2 = (float)magnitudeImage->pixels[magIdx + 1];
			}
			else if (angleDeg >= 22.5 && angleDeg < 67.5) {
				// 45 degree edge (compare diagonal top-right and bottom-left)
				neighbor1 = (float)magnitudeImage->pixels[(y - 1) * magStride + (x + 1) * magnitudeImage->channels];
				neighbor2 = (float)magnitudeImage->pixels[(y + 1) * magStride + (x - 1) * magnitudeImage->channels];
			}
			else if (angleDeg >= 67.5 && angleDeg < 112.5) {
				// vertical edge (compare top and bottom)
				neighbor1 = (float)magnitudeImage->pixels[(y - 1) * magStride + x * magnitudeImage->channels];
				neighbor2 = (float)magnitudeImage->pixels[(y + 1) * magStride + x * magnitudeImage->channels];
			}
			else if (angleDeg >= 112.5 && angleDeg < 157.5) {
				// 135 degree edge (compare diagonal top-left and bottom-right)
				neighbor1 = (float)magnitudeImage->pixels[(y - 1) * magStride + (x - 1) * magnitudeImage->channels];
				neighbor2 = (float)magnitudeImage->pixels[(y + 1) * magStride + (x + 1) * magnitudeImage->channels];
			}

			// suppress non-maximum
			if (mag >= neighbor1 && mag >= neighbor2) {
				dest->pixels[y * stride + x * dest->channels] = (uint8_t)mag;
			}
			else {
				dest->pixels[y * stride + x * dest->channels] = 0;
			}
		}
	}

	return true;
}


extern "C" bool ImageDirection(
	struct bmpImage* dest,
	const struct bmpImage* Gx,
	const struct bmpImage* Gy
) {
	// allocate dest image
	dest->width = Gx->width;
	dest->height = Gx->height;
	dest->channels = 1; // single channel for direction
	dest->colorTable = Gx->colorTable;
	dest->header = Gx->header;
	uint32_t stride = (dest->width * dest->channels + 3) & ~3;
	dest->pixels = (uint8_t*)malloc(dest->height * stride);
	if (!dest->pixels) {
		ASSERT(0, "No memory !!!");
		return false;
	}
	memset(dest->pixels, 0, dest->height * stride);
	for (uint32_t y = 0; y < dest->height; y++) {
		for (uint32_t x = 0; x < dest->width; x++) {

			uint32_t idxGx = y * ((Gx->width * Gx->channels + 3) & ~3) + x * Gx->channels;
			uint32_t idxGy = y * ((Gy->width * Gy->channels + 3) & ~3) + x * Gy->channels;
			float gx = (float)Gx->pixels[idxGx];
			float gy = (float)Gy->pixels[idxGy];
			float angle = atan2f(gy, gx); // angle in radians
			// map angle from [-pi, pi] to [0, 255]
			uint8_t mappedAngle = (uint8_t)(((angle + 3.14159265f) / (2.0f * 3.14159265f)) * 255.0f);
			uint32_t destIdx = y * stride + x * dest->channels;
			dest->pixels[destIdx] = mappedAngle;
		}
	}
	return true;
}

extern "C" bool imageDoubleThresholding(
	struct bmpImage* dest,
	const struct bmpImage* magnitudeImage,
	float lowThreshold,
	float highThreshold) {
	// allocate dest image
	dest->width = magnitudeImage->width;
	dest->height = magnitudeImage->height;
	dest->channels = 1; // single channel for edges
	dest->colorTable = magnitudeImage->colorTable;
	dest->header = magnitudeImage->header;
	uint32_t stride = (dest->width * dest->channels + 3) & ~3;
	dest->pixels = (uint8_t*)malloc(dest->height * stride);
	if (!dest->pixels) {
		ASSERT(0, "No memory !!!");
		return false;
	}
	memset(dest->pixels, 0, dest->height * stride);
	for (uint32_t y = 0; y < dest->height; y++) {
		for (uint32_t x = 0; x < dest->width; x++) {
			uint32_t idxMag = y * ((magnitudeImage->width * magnitudeImage->channels + 3) & ~3) + x * magnitudeImage->channels;
			uint8_t magValue = magnitudeImage->pixels[idxMag];
			uint8_t edgeValue = 0;
			if (magValue >= highThreshold) {
				edgeValue = 255; // strong edge
			}
			else if (magValue >= lowThreshold) {
				edgeValue = 64; // weak edge
			}
			else {
				edgeValue = 0; // non-edge
			}
			uint32_t destIdx = y * stride + x * dest->channels;
			dest->pixels[destIdx] = edgeValue;
		}
	}
	return true;
}



#define STRONG_EDGE 255
#define WEAK_EDGE 64
#define NON_EDGE 0

extern "C" bool ImageHysteresis(
	struct bmpImage* dest,
	const struct bmpImage* input)  // input = NMS + double threshold
{
	// allocate dest image
	dest->width = input->width;
	dest->height = input->height;
	dest->channels = 1;
	dest->colorTable = input->colorTable;
	dest->header = input->header;
	uint32_t stride = (dest->width * dest->channels + 3) & ~3;
	dest->pixels = (uint8_t*)malloc(dest->height * stride);
	if (!dest->pixels) {
		ASSERT(0, "No memory !!!");
		return false;
	}
	memcpy(dest->pixels, input->pixels, dest->height * stride); // copy input

	// Iterate over the image
	for (uint32_t y = 1; y < dest->height - 1; y++) {
		for (uint32_t x = 1; x < dest->width - 1; x++) {
			uint32_t idx = y * stride + x * dest->channels;
			if (dest->pixels[idx] == WEAK_EDGE) {
				// Check 8 neighbors
				bool connectedToStrong = false;
				for (int j = -1; j <= 1 && !connectedToStrong; j++) {
					for (int i = -1; i <= 1 && !connectedToStrong; i++) {
						if (i == 0 && j == 0) continue;
						uint32_t neighborIdx = (y + j) * stride + (x + i) * dest->channels;
						if (dest->pixels[neighborIdx] == STRONG_EDGE) {
							connectedToStrong = true;
						}
					}
				}

				if (connectedToStrong) {
					dest->pixels[idx] = STRONG_EDGE;
				}
				else {
					dest->pixels[idx] = NON_EDGE;
				}
			}
		}
	}

	return true;
}


extern "C" bool imageCannyEdgeDetect(
	struct bmpImage* dest,
	const struct bmpImage* source,
	float sigma ,
	float lowThreshold,
	float highThreshold){

	// Step 1 : Gaussian Filter
	bmpImage smoothedImage;
	ImageGausianFilter(&smoothedImage, source, 5, sigma);

	// Step 2 : Gradient Calculation (Sobel)
	MaskAttributes* SobelxMask = MaskGet(MaskType::EDGE_DETECTION_SOBEL_VERTICAL);
	MaskAttributes* SobelyMask = MaskGet(MaskType::EDGE_DETECTION_SOBEL_HORIZONTAL);
	bmpImage GX;
	bmpImage GY;
	image_convolve(SobelxMask, false, &smoothedImage, &GX);
	image_convolve(SobelyMask, false, &smoothedImage, &GY);

	// Step 3 : Gradient Magnitude and Direction Images
	bmpImage magnitudeImage;
	ImageMagnitute(&magnitudeImage, &GX, &GY);
	bmpImage directionImage;
	ImageDirection(&directionImage, &GX, &GY);

	// Step 4 : Non-Maximum Suppression
	bmpImage nonMaxSuppressedImage;
	ImageNonMaximumSuppression(&nonMaxSuppressedImage, &magnitudeImage, &directionImage);

	// Step 5 : Double Thresholding
	bmpImage thresholdedImage;
	imageDoubleThresholding(&thresholdedImage, &nonMaxSuppressedImage, lowThreshold, highThreshold);


	// Step 6 : Hysteresis
	ImageHysteresis(dest, &thresholdedImage);

	free(smoothedImage.pixels);
	free(GX.pixels);
	free(GY.pixels);
	free(magnitudeImage.pixels);
	free(directionImage.pixels);
	free(nonMaxSuppressedImage.pixels);
	free(thresholdedImage.pixels);
	
	return true;
}
