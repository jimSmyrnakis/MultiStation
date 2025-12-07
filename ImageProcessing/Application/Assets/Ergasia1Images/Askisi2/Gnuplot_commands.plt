set terminal pngcairo size 1600,1000
set output "original_histograms.png"

set style fill solid 1.0 border -1
set boxwidth 0.8 relative
set tics font ",10"

set multiplot layout 2,2 title "Histograms (Multiplot)" font ",14"

# ---------- Plot 1 ----------
set title "File 1: dark_road_1.bmp"
plot "histogram_dark_road_1.txt" using 1:2 with boxes lc rgb "#e41a1c" notitle

# ---------- Plot 2 ----------
set title "File 2: dark_road_2.bmp"
plot "histogram_dark_road_2.txt" using 1:2 with boxes lc rgb "#e41a1c" notitle

# ---------- Plot 3 ----------
set title "File 3: dark_road_3.bmp"
plot "histogram_dark_road_3.txt" using 1:2 with boxes lc rgb "#e41a1c" notitle



unset multiplot
unset output

set terminal pngcairo size 1600,1000
set output "equallized_tile_histograms.png"

set style fill solid 1.0 border -1
set boxwidth 0.8 relative
set tics font ",10"

set multiplot layout 2,2 title "Histograms (Multiplot)" font ",14"

# ---------- Plot 1 ----------
set title "File 1: dark_equalized_tile_road_1.bmp"
plot "histogram_equalized_dark_road_1.txt" using 1:2 with boxes lc rgb "#e41a1c" notitle

# ---------- Plot 2 ----------
set title "File 2: dark_equalized_tile_road_2.bmp"
plot "histogram_equalized_dark_road_2.txt" using 1:2 with boxes lc rgb "#e41a1c" notitle

# ---------- Plot 3 ----------
set title "File 3: dark_equalized_tile_road_3.bmp"
plot "histogram_equalized_dark_road_3.txt" using 1:2 with boxes lc rgb "#e41a1c" notitle



unset multiplot
unset output



unset multiplot
unset output

set terminal pngcairo size 1600,1000
set output "equallized_histograms.png"

set style fill solid 1.0 border -1
set boxwidth 0.8 relative
set tics font ",10"

set multiplot layout 2,2 title "Histograms (Multiplot)" font ",14"

# ---------- Plot 1 ----------
set title "File 1: dark_equalized_road_1.bmp"
plot "histogram_equalized_no_tile_dark_road_1.txt" using 1:2 with boxes lc rgb "#e41a1c" notitle

# ---------- Plot 2 ----------
set title "File 2: dark_equalized_road_2.bmp"
plot "histogram_equalized_no_tile_dark_road_2.txt" using 1:2 with boxes lc rgb "#e41a1c" notitle

# ---------- Plot 3 ----------
set title "File 3: dark_equalized_road_3.bmp"
plot "histogram_equalized_no_tile_dark_road_3.txt" using 1:2 with boxes lc rgb "#e41a1c" notitle



unset multiplot
unset output