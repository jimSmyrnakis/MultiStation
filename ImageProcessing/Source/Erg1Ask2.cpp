#include "Core.hpp"
#include "Test/inc.h"
#include "dsp/LowLevel/Histogram/Histogram.h"

#define FOLDER(name) "./Application/Assets/Ergasia1Images/Askisi2/"##name
#define CHECK(func) if (!(func)){ ASSERT(0 , "Failed !!!"); return 1;}
int Erg1Ask2(void) {
	

	// load the image
	bmpImage OriginalDark1;
	bmpImage OriginalDark2;
	bmpImage OriginalDark3;
	
	CHECK(bmp_load(FOLDER("dark_road_1.bmp"), &OriginalDark1));
	CHECK(bmp_load(FOLDER("dark_road_2.bmp"), &OriginalDark2));
	CHECK(bmp_load(FOLDER("dark_road_3.bmp"), &OriginalDark3));

	









	
	float clipFactor = 2;

	// Δημιουργία Αρχείου Ισοστάθμισης
	float* histDark1 = image_histogram_init_full(&OriginalDark1, true);
	CHECK(histDark1 != nullptr);
	float* histDark2 = image_histogram_init_full(&OriginalDark2, true);
	CHECK(histDark2 != nullptr);
	float* histDark3 = image_histogram_init_full(&OriginalDark3, true);
	CHECK(histDark3 != nullptr);


	// Αποθήκευση Ιστογραμμάτων σε αρχεία κειμένου
	CHECK(histogram_write_to_file(FOLDER("histogram_dark_road_1.txt"), histDark1 , 256));
	CHECK(histogram_write_to_file(FOLDER("histogram_dark_road_2.txt"), histDark2 , 256));
	CHECK(histogram_write_to_file(FOLDER("histogram_dark_road_3.txt"), histDark3 , 256));

	// Απελευθέρωση μνήμης Ιστογραμμάτων
	free(histDark1);
	free(histDark2);
	free(histDark3);















	// Δημιουργία νέων εικόνων για την αποθήκευση των αποτελεσμάτων
	bmpImage EqualizedTileDark1;
	bmpImage EqualizedTileDark2;
	bmpImage EqualizedTileDark3;
	uint32_t dark1_tiles = 20;
	uint32_t dark2_tiles = 20;
	uint32_t dark3_tiles = 20;

	CHECK(image_histogram_equalization2(&EqualizedTileDark1, dark1_tiles, clipFactor, &OriginalDark1));
	CHECK(image_histogram_equalization2(&EqualizedTileDark2, dark2_tiles, clipFactor, &OriginalDark2));
	CHECK(image_histogram_equalization2(&EqualizedTileDark3, dark3_tiles, clipFactor, &OriginalDark3));

	// Αποθήκευση των ισοσταθμισμένων tiled εικόνων
	CHECK(bmp_store(FOLDER("equalized_tile_dark_road_1.bmp"), &EqualizedTileDark1));
	CHECK(bmp_store(FOLDER("equalized_tile_dark_road_2.bmp"), &EqualizedTileDark2));
	CHECK(bmp_store(FOLDER("equalized_tile_dark_road_3.bmp"), &EqualizedTileDark3));

	// Αποθήκευση Ιστογραμμάτων των tiled ισοσταθμισμένων εικόνων
	float* histEqualizedTileDark1 = image_histogram_init_full(&EqualizedTileDark1, true);
	CHECK(histEqualizedTileDark1 != nullptr);
	float* histEqualizedTileDark2 = image_histogram_init_full(&EqualizedTileDark2, true);
	CHECK(histEqualizedTileDark2 != nullptr);
	float* histEqualizedTileDark3 = image_histogram_init_full(&EqualizedTileDark3, true);
	CHECK(histEqualizedTileDark3 != nullptr);

	// Αποθήκευση tiled Ιστογραμμάτων σε αρχεία κειμένου
	CHECK(histogram_write_to_file(FOLDER("histogram_equalized_dark_road_1.txt"), histEqualizedTileDark1, 256));
	CHECK(histogram_write_to_file(FOLDER("histogram_equalized_dark_road_2.txt"), histEqualizedTileDark2, 256));
	CHECK(histogram_write_to_file(FOLDER("histogram_equalized_dark_road_3.txt"), histEqualizedTileDark3, 256));

	// Απελευθέρωση μνήμης Ιστογραμμάτων ισοσταθμισμένων εικόνων
	free(histEqualizedTileDark1);
	free(histEqualizedTileDark2);
	free(histEqualizedTileDark3);












	// Ισοστάθμιση Χωρίς Tiling
	
	bmpImage Dark1Equalized;
	bmpImage Dark2Equalized;
	bmpImage Dark3Equalized;
	// Ισοστάθμιση Χωρίς Tiling
	CHECK(image_histogram_equalization(&Dark1Equalized, 1, clipFactor, &OriginalDark1));
	CHECK(image_histogram_equalization(&Dark2Equalized, 1, clipFactor, &OriginalDark2));
	CHECK(image_histogram_equalization(&Dark3Equalized, 1, clipFactor, &OriginalDark3));

	// Αποθήκευση Ιστογραμμάτων των ισοσταθμισμένων εικόνων χωρίς tiling 
	float* histEqualizedDark1NoTile = image_histogram_init_full(&Dark1Equalized, true);
	CHECK(histEqualizedDark1NoTile != nullptr);
	float* histEqualizedDark2NoTile = image_histogram_init_full(&Dark2Equalized, true);
	CHECK(histEqualizedDark2NoTile != nullptr);
	float* histEqualizedDark3NoTile = image_histogram_init_full(&Dark3Equalized, true);
	CHECK(histEqualizedDark3NoTile != nullptr);

	// Αποθήκευση Ιστογραμμάτων σε αρχεία κειμένου
	CHECK(histogram_write_to_file(FOLDER("histogram_equalized_no_tile_dark_road_1.txt"), histEqualizedDark1NoTile, 256));
	CHECK(histogram_write_to_file(FOLDER("histogram_equalized_no_tile_dark_road_2.txt"), histEqualizedDark2NoTile, 256));
	CHECK(histogram_write_to_file(FOLDER("histogram_equalized_no_tile_dark_road_3.txt"), histEqualizedDark3NoTile, 256));


	// Αποθήκευση των ισοσταθμισμένων εικόνων χωρίς tiling
	CHECK(bmp_store(FOLDER("equalized_dark_road_1.bmp"), &Dark1Equalized));
	CHECK(bmp_store(FOLDER("equalized_dark_road_2.bmp"), &Dark2Equalized));
	CHECK(bmp_store(FOLDER("equalized_dark_road_3.bmp"), &Dark3Equalized));
	
	
	

	

	


	return 0;
}
