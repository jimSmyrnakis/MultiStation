#include "Histogram.h"
#include <memory.h>
#define GRAYSCALE_HISTOGRAM_SIZE 256
#define GRAYSCALE_L_MAX 255

extern "C" float* image_histogram_init(
	const struct bmpImage* image,
	uint32_t tile_x,
	uint32_t tile_y,
	uint32_t tile_w,
	uint32_t tile_h,
	float clipFactor,
	bool normalize)
{
	// έλεγχος παραμέτρων
	if (!image || !image->pixels || image->channels != 1)
		return nullptr;

	// FIXED  stride
	uint32_t stride = (image->width * image->channels + 3) & ~3;

	// Clip tileς
	if ((tile_x + tile_w) > image->width)  tile_w = image->width - tile_x;
	if ((tile_y + tile_h) > image->height) tile_h = image->height - tile_y;

	// δέσμευση μνήμης για τοπικο/ολικο ιστόγραμμα
	float* hist = (float*)malloc(256 * sizeof(float));
	if (!hist) return nullptr;

	// αρχικοποιήση σε τιμή 0
	memset(hist, 0, 256 * sizeof(float));

	
	uint32_t total = tile_w * tile_h;
	
	// Για κάθε γραμμή της τοπικής εικόνας
	for (uint32_t y = tile_y; y < tile_y + tile_h; y++)
	{
		// αναφορά προς την γραμμή αυτή
		const uint8_t* row = image->pixels + y * stride;

		// για κάθε στήλη της τοπικής εικόνας
		for (uint32_t x = tile_x; x < tile_x + tile_w; x++) {
			hist[row[x]]++;
			// υπολογισέ τοπικό ιστόγραμμα
		}
			
	}

	// με χρήση ρυθμηστή clipFactor καθόρισε ένα όριο
	// στο πλήθος πίξελς κάθε φωτηνοτήτας στο τοπικο 
	// ινστόγραμμα .
	float clip = clipFactor * ((float)total / 256.0f);
	float excess = 0; //Πάρε το υπόλοιπο να το μοιράσεις
	// ισάξια , θέλουμε όταν η εικόνα έχει πολλά πιξέλ σκοτείνα
	// να μήν τόυς δείνει πολλή μεγάλη διαφορα φωτήσμου 
	for (int i = 0; i < 256; i++) {
		if (hist[i] > clip) {

			excess += hist[i] - clip;
			hist[i] = clip;
		}
	}

	// Τώρα μοίρασε τα 
	float remainingExcess = (float)excess;
	int max_counts = 10, count =0; // επίσης βάλε ένα όριο επαναλήψεων
	while ((remainingExcess > 0.1f) && (count < max_counts)) {
		int countFreeBins = 0;
		// για κάθε επάναληψή βρές τα τρέχων πίξελς που δεν 
		// έχουν φτάσει στο μέγιστο πεδίο 
		for (int i = 0; i < 256; i++)
			if (hist[i] < clip) countFreeBins++;
		// αν δεν υπάρχουν άλλα βγές 
		if (countFreeBins == 0)
			break; // όλα τα bins έχουν φτάσει το clip

		// μοίρασε άνα επανάληψη το υπόλοιπο ισάξια
		float distribute = remainingExcess / countFreeBins;

		// δώσε το σέ όλα που δεν έχουν φτάσει το κατόφλη ακόμα
		// αλλά φρώντησε να μήν ξεπεράσουν το κατόφλη
		for (int i = 0; i < 256; i++) {
			if (hist[i] < clip) {
				float add = (hist[i] + distribute > clip) ? (clip - hist[i]) : distribute;
				hist[i] += add;
				remainingExcess -= add;
			}
		}
		count++;
	}




	if (normalize) // άν πρέπει να κανονικοποιηθή το ιστόγραμμα
	{
		float sum = 0; // βρές το συνολικό άθροισμα
		for (int i = 0; i < 256; i++)
			sum += hist[i];

		if (sum > 0.0f)
			for (int i = 0; i < 256; i++)
				hist[i] /= sum; // κανονικόποιήσε την τιμή
	}

	return hist;
}


extern "C" bool image_histogram_tile_cdf(
	uint32_t tile_x,
	uint32_t tile_y,
	uint32_t tile_w,
	uint32_t tile_h,
	uint32_t tile_base_w,
	uint32_t tile_base_h,
	float*** cdfs ,
	float clipFactor,
	const struct bmpImage* src
) {
	float* hist = image_histogram_init(
		src,
		tile_x, tile_y,
		tile_w, tile_h,
		clipFactor, true
	);
	uint32_t numCols = (src->width + tile_base_w - 1) / tile_base_w;
	uint32_t numRows = (src->height + tile_base_h - 1) / tile_base_h;

	uint32_t cdf_col = tile_x / tile_base_w;
	uint32_t cdf_row = tile_y / tile_base_h;

	if (cdf_col >= numCols || cdf_row >= numRows) return false;

	float* cdf = cdfs[cdf_row][cdf_col];
	cdf[0] = hist[0];
	for (int i = 1; i < 256; i++) {
		cdf[i] = hist[i] + cdf[i - 1];
	}
	free(hist);

	return true;
}


extern "C" bool image_histogram_tile_equalization2(
	struct bmpImage* dest,
	uint32_t tile_x,
	uint32_t tile_y,
	uint32_t tile_w,
	uint32_t tile_h,
	uint32_t tile_base_w,
	uint32_t tile_base_h,
	float*** cdfs,
	const struct bmpImage* src)
{
	if (!dest || !src || !src->pixels)
		return false;

	uint32_t stride = (src->width * src->channels + 3) & ~3;

	uint32_t numCols = (src->width + tile_base_w - 1) / tile_base_w; // now equals tiles
	uint32_t numRows = (src->height + tile_base_h - 1) / tile_base_h; // equals tiles
	

	for (uint32_t j = 0; j < tile_h; j++)
	{
		uint32_t y = tile_y + j;
		if (y >= src->height) continue;

		const uint8_t* srow = src->pixels + y * stride;
		uint8_t* drow = dest->pixels + y * stride;

		// tile index row
		uint32_t row0 = y / tile_base_h;
		uint32_t row1 = (row0 + 1 < numRows) ? row0 + 1 : row0;
		float wy = float(y % tile_base_h) / float(tile_base_h);

		for (uint32_t i = 0; i < tile_w; i++)
		{
			uint32_t x = tile_x + i;
			if (x >= src->width) continue;

			// tile index col
			uint32_t col0 = x / tile_base_w;
			uint32_t col1 = (col0 + 1 < numCols) ? col0 + 1 : col0;

			float wx = float(x % tile_base_w) / tile_base_w;

			int v = srow[x];

			float* cdfTL = cdfs[row0][col0];
			float* cdfTR = cdfs[row0][col1];
			float* cdfBL = cdfs[row1][col0];
			float* cdfBR = cdfs[row1][col1];

			float newVal =
				(1 - wx) * (1 - wy) * cdfTL[v] +
				wx * (1 - wy) * cdfTR[v] +
				(1 - wx) * wy * cdfBL[v] +
				wx * wy * cdfBR[v];

			drow[x] = (uint8_t)(newVal * 255.0f);
		}
	}

	return true;
}


extern "C" bool image_histogram_equalization2(
	struct bmpImage* dest,
	uint16_t tiles,
	float clipFactor,
	const struct bmpImage* source) {

	if (!tiles || !source || !dest)
		return false;

	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = source->channels;
	uint32_t stride = (source->width * source->channels + 3) & ~3;
	// base tile size
	uint32_t base_tile_w = (source->width + tiles - 1) / tiles;  // ceil(width/tiles)
	uint32_t base_tile_h = (source->height + tiles - 1) / tiles;  // ceil(height/tiles)

	dest->pixels = (uint8_t*)malloc(stride * dest->height);
	if (!dest->pixels)
		return false;

	//allocate and initialize cdfs
	float*** cdfs = (float***)malloc(tiles * sizeof(float**));
	if (!cdfs)
		return false;
	for (int i = 0;i < tiles; i++) {
		cdfs[i] = (float**)malloc(tiles * sizeof(float*));
		if (!cdfs[i]) return false;
		for (int j = 0; j < tiles; j++) {
			cdfs[i][j] = (float*)malloc(256 * sizeof(float));
			if (!cdfs[i][j]) return false;
			image_histogram_tile_cdf(
				j * base_tile_w, i * base_tile_h,
				base_tile_w, base_tile_h, base_tile_w, base_tile_h,
				cdfs, clipFactor, source);

		}
	}

	
	


	

	// apply equalization tile-by-tile
	for (uint32_t ty = 0; ty < tiles; ty++)
	{
		for (uint32_t tx = 0; tx < tiles; tx++)
		{
			uint32_t tile_x = tx * base_tile_w;
			uint32_t tile_y = ty * base_tile_h;

			// correct tile size to avoid missing border pixels
			uint32_t tile_w = base_tile_w;
			uint32_t tile_h = base_tile_h;

			if (tx == (tiles - 1)) tile_w = source->width - tile_x;
			if (ty == (tiles - 1)) tile_h = source->height - tile_y;

			image_histogram_tile_equalization2(
				dest, tile_x, tile_y, tile_w, tile_h,base_tile_w ,base_tile_h , cdfs , source);
		}
	}


	// copy color table if exists
	if (source->colorTable) {
		dest->colorTable = (uint8_t*)malloc(source->header.dataOffset - sizeof(struct BMPHeader));
		if (!dest->colorTable) {
			free(dest->pixels);
			return false;
		}
		memcpy_s(
			dest->colorTable,
			source->header.dataOffset - sizeof(struct BMPHeader),
			source->colorTable,
			source->header.dataOffset - sizeof(struct BMPHeader)
		);
	}
	else {
		dest->colorTable = nullptr;
	}

	// free space 
	for (int i = 0;i < tiles; i++) {
		
		for (int j = 0; j < tiles; j++) {
			free(cdfs[i][j]);
		}

		free(cdfs[i]);
	}
	free(cdfs);

	return true;
}




























extern "C" bool image_histogram_tile_equalization(
	struct bmpImage* dest,
	uint32_t tile_x,
	uint32_t tile_y,
	uint32_t tile_w,
	uint32_t tile_h,
	float clipFactor,
	const struct bmpImage* src)
{
	if (!dest || !src || !src->pixels)
		return false;

	uint32_t stride = (src->width * src->channels + 3) & ~3;

	// Clip borders
	if ((tile_x + tile_w) > src->width)  tile_w = src->width - tile_x;
	if ((tile_y + tile_h) > src->height) tile_h = src->height - tile_y;

	float* hist = image_histogram_init(src, tile_x, tile_y, tile_w, tile_h, clipFactor, true);
	if (!hist) return false;

	float cdf[256];
	cdf[0] = hist[0];
	for (int i = 1; i < 256; i++)
		cdf[i] = cdf[i - 1] + hist[i];

	free(hist);

	// find first non-zero CDF
	float cdf_min = 0.0f;
	for (int i = 0; i < 256; i++)
		if (cdf[i] > 0.0f) { cdf_min = cdf[i]; break; }

	float denom = 1.0f - cdf_min;
	if (denom < 1e-8f) denom = 1.0f; // avoid division by zero

	// Apply pixel mapping
	for (uint32_t y = tile_y; y < (tile_y + tile_h); y++)
	{
		const uint8_t* srow = src->pixels + y * stride;
		uint8_t* drow = dest->pixels + y * stride;

		for (uint32_t x = tile_x; x < (tile_x + tile_w); x++)
		{
			uint8_t old = srow[x];

			float v = (cdf[old] - cdf_min) / denom;
			if (v < 0) v = 0;

			drow[x] = (uint8_t)(v * 255 + 0.5f);
		}
	}

	return true;
}


extern "C" bool image_histogram_equalization(
	struct bmpImage* dest,
	uint16_t tiles,
	float clipFactor,
	const struct bmpImage* source) {

	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = source->channels;
	uint32_t stride = (source->width * source->channels + 3) & ~3;

	dest->pixels = (uint8_t*)malloc(stride * dest->height);
	if (!dest->pixels)
		return false;

	// base tile size
	uint32_t base_tile_w = source->width / tiles;
	uint32_t base_tile_h = source->height / tiles;

	// apply equalization tile-by-tile
	for (uint32_t ty = 0; ty < tiles; ty++)
	{
		for (uint32_t tx = 0; tx < tiles; tx++)
		{
			uint32_t tile_x = tx * base_tile_w;
			uint32_t tile_y = ty * base_tile_h;

			// correct tile size to avoid missing border pixels
			uint32_t tile_w = base_tile_w;
			uint32_t tile_h = base_tile_h;

			if (tx == (tiles - 1)) tile_w = source->width - tile_x;
			if (ty == (tiles - 1)) tile_h = source->height - tile_y;

			image_histogram_tile_equalization(
				dest, tile_x, tile_y, tile_w, tile_h, clipFactor , source);
		}
	}


	// copy color table if exists
	if (source->colorTable) {
		dest->colorTable = (uint8_t*)malloc(source->header.dataOffset - sizeof(struct BMPHeader));
		if (!dest->colorTable) {
			free(dest->pixels);
			return false;
		}
		memcpy_s(
			dest->colorTable,
			source->header.dataOffset - sizeof(struct BMPHeader),
			source->colorTable,
			source->header.dataOffset - sizeof(struct BMPHeader)
		);
	}
	else {
		dest->colorTable = nullptr;
	}

	return true;
}


float* image_histogram_init_full(
	const struct bmpImage* image,
	bool normallize) {
	if (image->channels != 1 || !image->pixels)
		return nullptr;
	float* hist = (float*)malloc(256 * sizeof(float));

	if (!hist) return nullptr;

	// αρχικοποιήση σε τιμή 0
	memset(hist, 0, 256 * sizeof(float));

	uint32_t stride = (image->width * image->channels + 3) & ~3;
	uint32_t total = image->width * image->height;

	for (uint32_t y = 0; y < image->height; y++)
	{
		// αναφορά προς την γραμμή αυτή
		const uint8_t* row = image->pixels + y * stride;
		// για κάθε στήλη της τοπικής εικόνας
		for (uint32_t x = 0; x < image->width; x++) {
			hist[row[x]]++;
			// υπολογισέ τοπικό ιστόγραμμα
		}
	}

	if (normallize) {
		
		
		for (int i = 0; i < 256; i++)
			hist[i] /= (float)total; // κανονικόποιήσε την τιμή
	}


	return hist;

}




























extern "C" bool histogram_write_to_file(const char* filename, float* histogram, size_t size) {
	if (!filename || !histogram || size == 0) {
		return false;
	}

	FILE* file = fopen(filename, "w");
	if (!file) {
		return false;
	}

	for (size_t i = 0; i < size; ++i) {
		fprintf(file, "%zu %f\n", i, histogram[i]);
	}

	fclose(file);

	return true;
}