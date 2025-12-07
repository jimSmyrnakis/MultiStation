#include "GausianNoise.h"
#include "../PixelBuffer.h"
#include <math.h>
#ifdef __cplusplus
extern "C" {
#endif 

	void get_noise_from_snr(float snr_db, float signal_power,
		float* mean, float* variance)
	{
		*mean = 0.0f;

		// SNR(dB) → linear
		float snr_linear = powf(10.0f, snr_db / 10.0f);

		// Pnoise = Psignal / SNR
		*variance = signal_power / snr_linear;
	}


	// Υπολογισμός δύναμης σήματος της εικόνας
	float compute_signal_power(const unsigned char* img,
		int width, int height,
		int channels, int stride)
	{
		double sum[4] = { 0 };   // υποστηρίζει μέχρι 4 κανάλια
		sum[0] = sum[1] = sum[2] = sum[3] = 0;
		int pixel_count = width * height;

		for (int y = 0; y < height; y++) {
			const unsigned char* row = img + y * stride;

			for (int x = 0; x < width; x++) {

				for (int c = 0; c < channels; c++) {

					unsigned char p = row[x * channels + c];
					sum[c] += (double)p * (double)p;
				}
			}
		}

		// μέση ισχύς ανά κανάλι
		double power_mean = 0.0;

		for (int c = 0; c < channels; c++) {
			double Pc = sum[c] / pixel_count;
			power_mean += Pc;
		}

		// τελική power είναι ο μέσος όρος μεταξύ όλων των καναλιών
		return (float)(power_mean / channels);
	}



	bool image_GausianNoise(
		struct bmpImage* dest,
		const struct bmpImage* source,
		float SNRdb
	) {

		struct PBOAttributes attrs;
		if (!getPBOAttributeMap(PBOT_GAUSIAN_NOISE, &attrs)) return false;

		struct Params params;
		params.count = 1;
		params.parameters = (void**)malloc(1 * sizeof(void*));
		params.allocator = { malloc , free };
		if (!params.parameters) return false;
		
		params.parameters[0] = &SNRdb;

		bool res = ImageProcess(dest, source, &attrs, &params);

		free(params.parameters);
		return res;
	}

	

	static bool GausianNoise(
		struct PixelBufferObject* dest,
		const struct PixelBufferObject* src,
		float snrdb 
	) {
		float mean, var;
		get_noise_from_snr(snrdb, 
			compute_signal_power((uint8_t*)src->buffer , 
			src->width , src->height , src->channels 
			, src->stride )
			, &mean, &var);
		int x, y;
		float noise, theta;
		uint8_t* dst = (uint8_t*)dest->buffer;
		uint8_t* sr = (uint8_t*)src->buffer;
		for (y = 0; y < dest->height; y++) {
			for (x = 0; x < dest->width; x++) {
				for (uint8_t c = 0; c < dest->channels; c++) {

					float U1 = (float)rand() / (float)RAND_MAX;
					float U2 = (float)rand() / (float)RAND_MAX;

					if (U1 < 1e-12) U1 = 1e-12;   // αποφυγή log(0)

					float Z0 = sqrtf(-2.0f * logf(U1)) * cosf(2.0f * 3.14159265f * U2);

					// Z0 έχει mean = 0, stddev = 1
					float noise = Z0 * sqrtf(var) + mean;

					uint32_t index = y * dest->stride + x * dest->channels + c;
					float pixel = sr[index] + noise;

					if (pixel < 0) pixel = 0;
					if (pixel > 255) pixel = 255;

					dst[index] = (unsigned char)(pixel + 0.5f);
				}
			}
		}


		return true;
	}

	bool GausianNoiseOperation(
		struct PixelBufferObject* dest,
		const struct PixelBufferObject* src,
		struct Params* parms) {
		
		dest->channels = src->channels;
		dest->stride = src->stride;
		dest->width = src->width;
		dest->height = src->height;
		size_t sz = src->height * src->stride;
		dest->buffer = parms->allocator.memalloc(sz);
		if (!dest->buffer) {
			ASSERT(0, "No memory !");
			return false;
		}

		return GausianNoise(dest, src, *((float*)parms->parameters[0]));
	}

	

	bool GausianNoiseResolve(enum PBOType type, struct PBOAttributes* attrs) {
		ASSERT(attrs, "Bad Parameter !");

		if (type != PBOT_GAUSIAN_NOISE) return false;

		attrs->PBOO_Func = GausianNoiseOperation;
		attrs->type = type;

		return true;
	}

#ifdef __cplusplus

}
#endif 